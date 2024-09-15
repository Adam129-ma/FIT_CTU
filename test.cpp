#ifndef __PROGTEST__

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <climits>
#include <cfloat>
#include <cassert>
#include <cmath>
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <string>
#include <array>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <queue>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <functional>
#include <iterator>
#include <stdexcept>
#include <variant>
#include <optional>
#include <compare>
#include <charconv>
#include <span>
#include <utility>
#include "expression.h"

using namespace std::literals;
using CValue = std::variant<std::monostate, double, std::string>;


constexpr unsigned SPREADSHEET_CYCLIC_DEPS = 0x01;
constexpr unsigned SPREADSHEET_FUNCTIONS = 0x02;
constexpr unsigned SPREADSHEET_FILE_IO = 0x04;
constexpr unsigned SPREADSHEET_SPEED = 0x08;
constexpr unsigned SPREADSHEET_PARSER = 0x10;
#endif /* __PROGTEST__ */

// Forward declaration of ASTNode and CPos
class ASTNode;
class CPos;


using ASTNodePtr = std::shared_ptr<ASTNode>;


// Abstract base class for all AST nodes
class ASTNode {
public:
    virtual ~ASTNode() = default;

    // Function to return value of the Node
    virtual CValue evaluate(std::map<CPos, bool> haveBeenVisited={}) const = 0;

    // Function to clone nodes
    virtual std::shared_ptr<ASTNode> clone() {
        return nullptr;
    }

    //  Function to copy ReferenceNode in CopyRect
    virtual void copyReference(size_t move_x = 0, size_t move_y = 0) {}

    // Function to save Nodes in form of string
    virtual std::string toString() const {
        return "";
    }

};

// Class representing positions of cells in a Spreadsheet
class CPos {
public:
    CPos(std::string_view str) {

        size_t pos = str.find_first_of("0123456789");

        // Check for absolute column reference
        if (!str.empty() && str.front() == '$') {
            m_columnIsAbsolute = true;
            str.remove_prefix(1);
            pos--;
        }
        // Check for absolute row reference
        if (pos > 0 && str[pos - 1] == '$') {
            m_rowIsAbsolute = true;
            m_column = parseColumn(str.substr(0, pos - 1)); // Parse column directly
            m_row = parseRow(str.substr(pos));
        } else {
            m_column = parseColumn(str.substr(0, pos)); // Parse column directly
            m_row = parseRow(str.substr(pos));
        }
    }

    CPos(size_t row, size_t column, bool rowIsAbsolute, bool columnIsAbsolute)
            : m_column(column), m_row(row), m_columnIsAbsolute(columnIsAbsolute), m_rowIsAbsolute(rowIsAbsolute) {}

    CPos(int row, int column)
            : m_column(column), m_row(row), m_columnIsAbsolute(false), m_rowIsAbsolute(false) {}

    int getColumn() const { return m_column; }

    int getRow() const { return m_row; }

    bool isColumnAbsolute() const { return m_columnIsAbsolute; }

    bool isRowAbsolute() const { return m_rowIsAbsolute; }

    bool operator<(const CPos &other) const {
        return std::tie(m_row, m_column) < std::tie(other.m_row, other.m_column);
    }

private:
    // From string representing column calculates int to represent given column
    int parseColumn(std::string_view str) const {
        int column = 0;
        for (char c : str) {
            if (isalpha(c)) {
                column = column * 26 + (toupper(c) - 'A' + 1);
            } else {
                break;
            }
        }
        return column - 1;
    }

    // From string representing column calculates int to represent given column
    int parseRow(std::string_view str) const {
        int result = 0;
        for (char c : str) {
            if (isdigit(c)) {
                result = result * 10 + (c - '0');
            } else {
                break; // Stop parsing at the first non-digit character
            }
        }
        return result;
    }

    int m_column;
    int m_row;
    bool m_columnIsAbsolute = false;
    bool m_rowIsAbsolute = false;
};

// Function to calculate string back from columnIndex
std::string columnIndexToString(int columnIndex) {
    std::string result;
    while (columnIndex >= 0) {
        result = static_cast<char>('A' + (columnIndex % 26)) + result;
        columnIndex = columnIndex / 26 - 1;
    }
    return result;
}

// Class to represent a cell in the spreadsheet
class CCell {
public:
    CCell() {}

    // Returns value of the cell
    CValue getValue(std::map<CPos, bool> haveBeenVisited={}) const {
        if (m_astNode) {
            // Evaluate AST if it exists
            return m_astNode->evaluate(haveBeenVisited);
        } else {
            return " ";
        }
    }

    CCell(const CCell &other) : originalValue(other.originalValue){
        if (other.m_astNode) {
            m_astNode = other.m_astNode->clone(); // clone ASTNode if exists
        } else {
            m_astNode = nullptr; // otherwise it doesn't exist
        }
    }

    CCell &operator=(const CCell &other) {
        if (this != &other) {
            m_astNode = other.m_astNode;
            if (other.m_astNode) {
                m_astNode = other.m_astNode->clone();
            } else {
                m_astNode = nullptr;
            }
            originalValue = other.originalValue;
        }
        return *this;
    }

    ASTNodePtr m_astNode;
    std::string originalValue;

private:
};

// AST node for numeric values
class ASTNumber : public ASTNode {
public:
    explicit ASTNumber(double val) : val(val) {}

    CValue evaluate(std::map<CPos, bool> haveBeenVisited={}) const override {
        return val;
    }

    std::shared_ptr<ASTNode> clone() override {
        return std::make_shared<ASTNumber>(*this);
    }

    std::string toString() const override {
        return std::to_string(val);
    }

private:
    double val;
};

// AST node for string values
class ASTString : public ASTNode {
public:
    explicit ASTString(std::string val) : val(std::move(val)) {}

    CValue evaluate(std::map<CPos, bool> haveBeenVisited={}) const override {
        return val;
    }

    std::shared_ptr<ASTNode> clone() override {
        return std::make_shared<ASTString>(*this);
    }

    std::string toString() const override {
        std::string modifiedVal = "\"";
        for (char c : val) {
            if (c == '\\') {
                modifiedVal += "\\\"";
            } else {
                modifiedVal += c;
            }
        }
        modifiedVal += "\"";
        return modifiedVal;
    }

private:
    std::string val;
};

// AST node for references to other cells
class ASTReference : public ASTNode {
public:
    ASTReference(std::string val, std::map<CPos, CCell> *table_ptr)
            : position(val), table_ptr(table_ptr), stringValue(val) {}

    CValue evaluate(std::map<CPos, bool> haveBeenVisited={}) const override {
        auto &cell = (*table_ptr)[position];
        auto visitIt = haveBeenVisited.find(position);

        if (visitIt != haveBeenVisited.end()){
            return CValue ();
        }
        haveBeenVisited[position] = true;
        if (cell.m_astNode ) {
            return cell.m_astNode->evaluate(haveBeenVisited);
        }
        return CValue();
    }

    void copyReference(size_t move_x, size_t move_y) override {
        size_t newX = (position.isRowAbsolute() ? position.getRow() : position.getRow() + move_x);
        size_t newY = (position.isColumnAbsolute() ? position.getColumn() : position.getColumn() + move_y);
        position = CPos(newX, newY, position.isRowAbsolute(), position.isColumnAbsolute());
    }

    std::shared_ptr<ASTNode> clone() override {
        return std::make_shared<ASTReference>(*this);
    }

    std::string toString() const override {
        std::string value;
        if(position.isColumnAbsolute()){value += "$";}
        value += columnIndexToString(position.getColumn());
        if(position.isRowAbsolute()){value += "$";}
        value += std::to_string(position.getRow());
        return  value;
    }

private:
    CPos position; // Position of the referenced cell
    std::map<CPos, CCell> *table_ptr; // Pointer to the table of cells
    std::string stringValue; // Stored string value
};

// AST node for binary operations
class ASTBinaryOperation : public ASTNode {
public:
    ASTBinaryOperation(ASTNodePtr left, ASTNodePtr right)
            : left(std::move(left)), right(std::move(right)) {}


    void copyReference(size_t move_x = 0, size_t move_y = 0) override {
        left->copyReference(move_x, move_y);
        right->copyReference(move_x, move_y);
    }


protected:
    ASTNodePtr left;
    ASTNodePtr right;
};

// AST Node representing addition operation
class ASTAddition : public ASTBinaryOperation {
public:
    using ASTBinaryOperation::ASTBinaryOperation;

    CValue evaluate(std::map<CPos, bool> haveBeenVisited={}) const override {
        auto lVal = left->evaluate(haveBeenVisited);
        auto rVal = right->evaluate(haveBeenVisited);

        if (std::holds_alternative<double>(lVal) && std::holds_alternative<double>(rVal)) {
            return std::get<double>(lVal) + std::get<double>(rVal);
        }
        if (std::holds_alternative<double>(lVal) && std::holds_alternative<std::string>(rVal)) {
            if (std::get<std::string>(rVal).empty()) { return CValue(); }
            return std::to_string(std::get<double>(lVal)) + std::get<std::string>(rVal);
        }
        if (std::holds_alternative<std::string>(lVal) && std::holds_alternative<double>(rVal)) {
            if (std::get<std::string>(lVal).empty()) { return CValue(); }
            return std::get<std::string>(lVal) + std::to_string(std::get<double>(rVal));
        }

        if (std::holds_alternative<std::string>(lVal) && std::holds_alternative<std::string>(rVal)) {
            if (std::get<std::string>(lVal).empty() || std::get<std::string>(rVal).empty()) { return CValue(); }
            return std::get<std::string>(lVal) + std::get<std::string>(rVal);
        } else {

            return CValue();
        }

    }

    std::shared_ptr<ASTNode> clone() override {
        return std::make_shared<ASTAddition>(left->clone(), right->clone());
    }

    std::string toString() const override {
        std::string result;
        if (left && right) {
            result += "(" + left->toString() + ")" + " + " + "(" + right->toString() + ")";
        }
        return result;
    }
};

// AST Node representing subtraction operation
class ASTSubtraction : public ASTBinaryOperation {
public:
    using ASTBinaryOperation::ASTBinaryOperation;

    CValue evaluate(std::map<CPos, bool> haveBeenVisited={}) const override {
        try {
            return std::get<double>(left->evaluate()) - std::get<double>(right->evaluate());
        }
        catch(...){return CValue(); }
    }

    std::shared_ptr<ASTNode> clone() override {
        return std::make_shared<ASTSubtraction>(left->clone(), right->clone());
    }

    std::string toString() const override {
        std::string result;
        if (left && right) {
            result += "(" + left->toString() + ")" + " - " + "(" + right->toString() + ")";
        }
        return result;
    }
};

// AST Node representing multiplication operation
class ASTMultiplication : public ASTBinaryOperation {
public:
    using ASTBinaryOperation::ASTBinaryOperation;

    CValue evaluate(std::map<CPos, bool> haveBeenVisited={}) const override {
        try {
            return std::get<double>(left->evaluate()) * std::get<double>(right->evaluate());
        }
        catch(...){return CValue(); }
    }

    std::shared_ptr<ASTNode> clone() override {
        return std::make_shared<ASTMultiplication>(left->clone(), right->clone());
    }

    std::string toString() const override {
        std::string result;
        if (left && right) {
            result += "(" + left->toString() + ")" + " * " + "(" + right->toString() + ")";
        }
        return result;
    }
};

// AST Node representing division operation
class ASTDivision : public ASTBinaryOperation {
public:
    using ASTBinaryOperation::ASTBinaryOperation;

    CValue evaluate(std::map<CPos, bool> haveBeenVisited={}) const override {
        try {
            auto lVal = left->evaluate(haveBeenVisited);
            auto rVal = right->evaluate(haveBeenVisited);
            if (std::get<double>(rVal) == 0) { return CValue(); }
            return std::get<double>(lVal) / std::get<double>(rVal);
        }
        catch(...){return CValue(); }

    }

    std::shared_ptr<ASTNode> clone() override {
        return std::make_shared<ASTDivision>(left->clone(), right->clone());
    }

    std::string toString() const override {
        std::string result;
        if (left && right) {
            result += "(" + left->toString() + ")" + " / " + "(" + right->toString() + ")";
        }
        return result;
    }
};

// AST Node representing pow operation

class ASTPow : public ASTNode {
public:
    ASTPow(ASTNodePtr base, ASTNodePtr exponent) : base(base), exponent(exponent) {}

    CValue evaluate(std::map<CPos, bool> haveBeenVisited={}) const override {
        // Evaluate the base and exponent nodes
        CValue baseValue = base->evaluate(haveBeenVisited);
        CValue exponentValue = exponent->evaluate(haveBeenVisited);

        // Check if both base and exponent values are numeric
        if (std::holds_alternative<double>(baseValue) && std::holds_alternative<double>(exponentValue)) {
            double baseNumber = std::get<double>(baseValue);
            double exponentNumber = std::get<double>(exponentValue);
            // Calculate the power
            return std::pow(baseNumber, exponentNumber);
        } else {
            // Return an empty value if either base or exponent is not numeric
            return CValue();
        }
    }

    std::shared_ptr<ASTNode> clone() override {
        return std::make_shared<ASTPow>(base->clone(), exponent->clone());
    }

    std::string toString() const override {
        std::string result;
        if (base && exponent) {
            result += "(" + base->toString() + ")" + " ^ " + "(" + exponent->toString() + ")";
        }
        return result;
    }

private:
    ASTNodePtr base;      // Pointer to the base node
    ASTNodePtr exponent;  // Pointer to the exponent node
};


// AST Node representing Negation operation
class ASTNodeNeg : public ASTNode {
public:
    explicit ASTNodeNeg(ASTNodePtr operand) : operand(operand) {}

    CValue evaluate(std::map<CPos, bool> haveBeenVisited={}) const override {
        // Evaluate the operand node
        CValue operandValue = operand->evaluate(haveBeenVisited);

        // Check if the operand value is numeric
        if (std::holds_alternative<double>(operandValue)) {
            double value = std::get<double>(operandValue);
            // Negate the numeric value
            return -value;
        } else {
            // Return an empty value if the operand is not numeric
            return CValue();
        }
    }

    void copyReference(size_t move_x = 0, size_t move_y = 0) override {
        operand->copyReference(move_x, move_y);
    }

    std::shared_ptr<ASTNode> clone() override {
        return std::make_shared<ASTNodeNeg>(operand->clone());
    }

    std::string toString() const override {
        std::string result;
        if (operand) {
            result += " - " + operand->toString();
        }
        return result;
    }

private:
    ASTNodePtr operand; // Pointer to the operand node
};

// AST Node representing Is Equal operation

class ASTNodeEq : public ASTNode {
public:
    ASTNodeEq(ASTNodePtr left, ASTNodePtr right) : left(left), right(right) {}

    CValue evaluate(std::map<CPos, bool> haveBeenVisited={}) const override {
        CValue leftValue = left->evaluate(haveBeenVisited);
        CValue rightValue = right->evaluate(haveBeenVisited);

        if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
            // Both operands are doubles, perform numerical comparison
            return (std::get<double>(leftValue) == std::get<double>(rightValue)) ? 1.0 : 0.0;
        } else if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)) {
            // Both operands are strings, perform string comparison
            return (std::get<std::string>(leftValue) == std::get<std::string>(rightValue)) ? 1.0 : 0.0;
        } else {
            // Operands are not comparable, return an invalid comparison result
            return CValue(); // or any other representation of invalid comparison
        }
    }

    std::shared_ptr<ASTNode> clone() override {
        return std::make_shared<ASTNodeEq>(left->clone(), right->clone());
    }

    std::string toString() const override {
        std::string result;
        if (left && right) {
            result += "(" + left->toString() + ")" + " = " + "(" + right->toString() + ")";
        }
        return result;
    }

private:
    ASTNodePtr left;  // Pointer to the left operand node
    ASTNodePtr right; // Pointer to the right operand node
};

// AST Node representing Is Not Equal operation

class ASTNodeNe : public ASTNode {
public:
    ASTNodeNe(ASTNodePtr left, ASTNodePtr right) : left(left), right(right) {}

    CValue evaluate(std::map<CPos, bool> haveBeenVisited={}) const override {
        CValue leftValue = left->evaluate(haveBeenVisited);
        CValue rightValue = right->evaluate(haveBeenVisited);

        if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
            // Both operands are doubles, perform numerical comparison
            return (std::get<double>(leftValue) != std::get<double>(rightValue)) ? 1.0 : 0.0;
        } else if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)) {
            // Both operands are strings, perform string comparison
            return (std::get<std::string>(leftValue) != std::get<std::string>(rightValue)) ? 1.0 : 0.0;
        } else {
            // Operands are not comparable, return an invalid comparison result
            return CValue(); // or any other representation of invalid comparison
        }
    }

    std::shared_ptr<ASTNode> clone() override {
        return std::make_shared<ASTNodeNe>(left->clone(), right->clone());
    }

    std::string toString() const override {
        std::string result;
        if (left && right) {
            result += "(" + left->toString() + ")" + " <> " + "(" + right->toString() + ")";
        }
        return result;
    }

private:
    ASTNodePtr left;  // Pointer to the left operand node
    ASTNodePtr right; // Pointer to the right operand node
};

// AST Node representing Is Less Than operation
class ASTNodeLt : public ASTNode {
public:
    ASTNodeLt(ASTNodePtr left, ASTNodePtr right) : left(left), right(right) {}

    CValue evaluate(std::map<CPos, bool> haveBeenVisited={}) const override {
        CValue leftValue = left->evaluate(haveBeenVisited);
        CValue rightValue = right->evaluate(haveBeenVisited);

        if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
            // Both operands are doubles, perform numerical comparison
            return (std::get<double>(leftValue) < std::get<double>(rightValue)) ? 1.0 : 0.0;
        } else if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)) {
            // Both operands are strings, perform string comparison
            return (std::get<std::string>(leftValue) < std::get<std::string>(rightValue)) ? 1.0 : 0.0;
        } else {
            // Operands are not comparable, return an invalid comparison result
            return CValue(); // or any other representation of invalid comparison
        }
    }

    std::shared_ptr<ASTNode> clone() override {
        return std::make_shared<ASTNodeLt>(left->clone(), right->clone());
    }

    std::string toString() const override {
        std::string result;
        if (left && right) {
            result += "(" + left->toString() + ")" + " < " + "(" + right->toString() + ")";
        }
        return result;
    }

private:
    ASTNodePtr left;  // Pointer to the left operand node
    ASTNodePtr right; // Pointer to the right operand node
};

// AST Node representing Is Less Than or Equal operation

class ASTNodeLe : public ASTNode {
public:
    ASTNodeLe(ASTNodePtr left, ASTNodePtr right) : left(left), right(right) {}

    CValue evaluate(std::map<CPos, bool> haveBeenVisited={}) const override {
        CValue leftValue = left->evaluate(haveBeenVisited);
        CValue rightValue = right->evaluate(haveBeenVisited);

        if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
            // Both operands are doubles, perform numerical comparison
            return (std::get<double>(leftValue) <= std::get<double>(rightValue)) ? 1.0 : 0.0;
        } else if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)) {
            // Both operands are strings, perform string comparison
            return (std::get<std::string>(leftValue) <= std::get<std::string>(rightValue)) ? 1.0 : 0.0;
        } else {
            // Operands are not comparable, return an invalid comparison result
            return CValue(); // or any other representation of invalid comparison
        }
    }

    std::shared_ptr<ASTNode> clone() override {
        return std::make_shared<ASTNodeLe>(left->clone(), right->clone());
    }

    std::string toString() const override {
        std::string result;
        if (left && right) {
            result += "(" + left->toString() + ")" + " <= " + "(" + right->toString() + ")";
        }
        return result;
    }

private:
    ASTNodePtr left;  // Pointer to the left operand node
    ASTNodePtr right; // Pointer to the right operand node
};

// AST Node representing Is Greater Than operation
class ASTNodeGt : public ASTNode {
public:
    ASTNodeGt(ASTNodePtr left, ASTNodePtr right) : left(left), right(right) {}

    CValue evaluate(std::map<CPos, bool> haveBeenVisited={}) const override {
        CValue leftValue = left->evaluate(haveBeenVisited);
        CValue rightValue = right->evaluate(haveBeenVisited);

        if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
            // Both operands are doubles, perform numerical comparison
            return (std::get<double>(leftValue) > std::get<double>(rightValue)) ? 1.0 : 0.0;
        } else if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)) {
            // Both operands are strings, perform string comparison
            return (std::get<std::string>(leftValue) > std::get<std::string>(rightValue)) ? 1.0 : 0.0;
        } else {
            // Operands are not comparable, return an invalid comparison result
            return CValue(); // or any other representation of invalid comparison
        }
    }

    std::shared_ptr<ASTNode> clone() override {
        return std::make_shared<ASTNodeGt>(left->clone(), right->clone());
    }

    std::string toString() const override {
        std::string result;
        if (left && right) {
            result += "(" + left->toString() + ")" + " > " + "(" + right->toString() + ")";
        }
        return result;
    }

private:
    ASTNodePtr left;  // Pointer to the left operand node
    ASTNodePtr right; // Pointer to the right operand node
};

// AST Node representing Is Greater Than or Equal operation
class ASTNodeGe : public ASTNode {
public:
    ASTNodeGe(ASTNodePtr left, ASTNodePtr right) : left(left), right(right) {}

    CValue evaluate(std::map<CPos, bool> haveBeenVisited={}) const override {
        CValue leftValue = left->evaluate(haveBeenVisited);
        CValue rightValue = right->evaluate(haveBeenVisited);

        if (std::holds_alternative<double>(leftValue) && std::holds_alternative<double>(rightValue)) {
            // Both operands are doubles, perform numerical comparison
            return (std::get<double>(leftValue) >= std::get<double>(rightValue)) ? 1.0 : 0.0;
        } else if (std::holds_alternative<std::string>(leftValue) && std::holds_alternative<std::string>(rightValue)) {
            // Both operands are strings, perform string comparison
            return (std::get<std::string>(leftValue) >= std::get<std::string>(rightValue)) ? 1.0 : 0.0;
        } else {
            // Operands are not comparable, return an invalid comparison result
            return CValue(); // or any other representation of invalid comparison
        }
    }

    std::shared_ptr<ASTNode> clone() override {
        return std::make_shared<ASTNodeGe>(left->clone(), right->clone());
    }

    std::string toString() const override {
        std::string result;
        if (left && right) {
            result += "(" + left->toString() + ")" + " >= " + "(" + right->toString() + ")";
        }
        return result;
    }

private:
    ASTNodePtr left;  // Pointer to the left operand node
    ASTNodePtr right; // Pointer to the right operand node
};

// Class handling given expression from parseExpression
class MyExpressionBuilder : public CExprBuilder {
public:
    // Constructor
    MyExpressionBuilder() : tableptr(nullptr) {}

    void opAdd() override {
        ASTNodePtr right = popOperand();
        ASTNodePtr left = popOperand();
        ASTNodePtr additionNode = std::make_shared<ASTAddition>(left, right);
        operandStack.push(additionNode); // Push the result back onto the stack
        rootNode = additionNode; // Update rootNode
    }

    void opSub() override {
        ASTNodePtr right = popOperand();
        ASTNodePtr left = popOperand();
        ASTNodePtr subtractionNode = std::make_shared<ASTSubtraction>(left, right);
        operandStack.push(subtractionNode); // Push the result back onto the stack
        rootNode = subtractionNode; // Update rootNode
    }

    void opMul() override {
        ASTNodePtr right = popOperand();
        ASTNodePtr left = popOperand();
        ASTNodePtr multiplicationNode = std::make_shared<ASTMultiplication>(left, right);
        operandStack.push(multiplicationNode); // Push the result back onto the stack
        rootNode = multiplicationNode; // Update rootNode
    }

    void opDiv() override {
        ASTNodePtr right = popOperand();
        ASTNodePtr left = popOperand();
        ASTNodePtr divisionNode = std::make_shared<ASTDivision>(left, right);
        operandStack.push(divisionNode); // Push the result back onto the stack
        rootNode = divisionNode; // Update rootNode
    }

    void valNumber(double val) override {
        ASTNodePtr numberNode = std::make_shared<ASTNumber>(val);
        operandStack.push(numberNode);
        rootNode = numberNode; // Update rootNode
    }

    void valString(std::string val) override {
        ASTNodePtr stringNode = std::make_shared<ASTString>(val);
        operandStack.push(stringNode);
        rootNode = stringNode; // Update rootNode
    }

    void opPow() override {
        ASTNodePtr exponent = popOperand();
        ASTNodePtr base = popOperand();
        ASTNodePtr powNode = std::make_shared<ASTPow>(base, exponent);
        operandStack.push(powNode); // Push the result back onto the stack
        rootNode = powNode; // Update rootNode
    }

    void opNeg() override {
        ASTNodePtr operand = popOperand();
        ASTNodePtr negNode = std::make_shared<ASTNodeNeg>(operand);
        operandStack.push(negNode); // Push the result back onto the stack
        rootNode = negNode; // Update rootNode
    }

    void opEq() override {
        ASTNodePtr right = popOperand();
        ASTNodePtr left = popOperand();
        ASTNodePtr eqNode = std::make_shared<ASTNodeEq>(left, right);
        operandStack.push(eqNode); // Push the result back onto the stack
        rootNode = eqNode; // Update rootNode
    }

    void opNe() override {
        ASTNodePtr right = popOperand();
        ASTNodePtr left = popOperand();
        ASTNodePtr neNode = std::make_shared<ASTNodeNe>(left, right);
        operandStack.push(neNode); // Push the result back onto the stack
        rootNode = neNode; // Update rootNode
    }

    void opLt() override {
        ASTNodePtr right = popOperand();
        ASTNodePtr left = popOperand();
        ASTNodePtr ltNode = std::make_shared<ASTNodeLt>(left, right);
        operandStack.push(ltNode); // Push the result back onto the stack
        rootNode = ltNode; // Update rootNode
    }

    void opLe() override {
        ASTNodePtr right = popOperand();
        ASTNodePtr left = popOperand();
        ASTNodePtr leNode = std::make_shared<ASTNodeLe>(left, right);
        operandStack.push(leNode); // Push the result back onto the stack
        rootNode = leNode; // Update rootNode
    }

    void opGt() override {
        ASTNodePtr right = popOperand();
        ASTNodePtr left = popOperand();
        ASTNodePtr gtNode = std::make_shared<ASTNodeGt>(left, right);
        operandStack.push(gtNode); // Push the result back onto the stack
        rootNode = gtNode; // Update rootNode
    }

    void opGe() override {
        ASTNodePtr right = popOperand();
        ASTNodePtr left = popOperand();
        ASTNodePtr geNode = std::make_shared<ASTNodeGe>(left, right);
        operandStack.push(geNode); // Push the result back onto the stack
        rootNode = geNode; // Update rootNode
    }

    void valReference(std::string val) override {
        ASTNodePtr referenceNode = std::make_shared<ASTReference>(val, tableptr);
        operandStack.push(referenceNode);
        rootNode = referenceNode;
    }

    void valRange(std::string val) override {

    };

    void funcCall(std::string fnName, int paramCount) override {
    };

    void setTableRef(std::map<CPos, CCell> &tableRef) {
        tableptr = &tableRef;
    }

    // Additional method to access the output stack
    std::stack<ASTNodePtr> &getOperandStack() {
        return operandStack;
    }

    // Additional method to access the root of ASTTree
    std::shared_ptr<ASTNode> getASTRoot() {
        return rootNode;
    }

private:
    // Method to pop an operand from the stack
    ASTNodePtr popOperand() {
        if (operandStack.empty()) {
            throw std::runtime_error("Operand stack is empty");
        }
        ASTNodePtr operand = operandStack.top();
        operandStack.pop();
        return operand;
    }

    std::map<CPos, CCell> *tableptr;
    std::stack<ASTNodePtr> operandStack;
    ASTNodePtr rootNode;
};


// Definition of class CSpreadsheet
class CSpreadsheet {
public:
    static unsigned capabilities() {
        return SPREADSHEET_CYCLIC_DEPS | SPREADSHEET_FUNCTIONS | SPREADSHEET_FILE_IO | SPREADSHEET_SPEED;
    }

    bool save(std::ostream &os) const {
        try {
            // Calculate the size of the content
            std::ostringstream contentStream;
            for (const auto &cellPair: table) {
                if (cellPair.second.m_astNode) {
                    CPos pos = cellPair.first;
                    std::string resultPos;

                    // Save pos, check for absolute link
                    std::string columnStr = columnIndexToString(pos.getColumn());
                    resultPos = columnStr + std::to_string(pos.getRow());

                    // Save to content stream, add delimiter
                    contentStream << resultPos << "{!,!}";

                    // Save CValue
                    auto &cell = cellPair.second;
                    contentStream << "=" + cell.m_astNode->toString();

                    contentStream << "{!,!}";
                }
            }
            std::string content = contentStream.str();

            // Convert size to string
            std::string sizeStr = std::to_string(content.size());

            // Write the size information followed by the delimiter
            os << sizeStr << "{!,!}";

            // Write the content to the stream
            os.write(content.c_str(), content.size());

            return true;
        } catch (...) {
            return false;
        }
    }

    bool load(std::istream &is) {
        try {
            table.clear();
            std::string delimiter = "{!,!}";
            std::string posStr, valueStr;
            std::string input;
            std::stringstream buffer;
            buffer << is.rdbuf();
            input = buffer.str();

            // Find the position of the first delimiter
            size_t position = input.find(delimiter);
            if (position == std::string::npos) {
                // Delimiter not found, invalid format
                return false;
            }

            // Read the size value before the first delimiter as a string
            std::string sizeStr = input.substr(0, position);
            // Convert size string to integer
            size_t size = std::stoul(sizeStr);

            // Erase the size value and the first delimiter from the input
            input.erase(0, position + delimiter.length());

            // Check if the loaded size matches the size of the remaining input
            if (size != input.size()) {
                // Size mismatch, data may be corrupted
                return false;
            }

            // Load the sheet data
            while ((position = input.find(delimiter)) != std::string::npos) {
                posStr = input.substr(0, position);
                input.erase(0, position + delimiter.length());

                valueStr = input.substr(0, input.find(delimiter));
                input.erase(0, input.find(delimiter) + delimiter.length());

                CPos pos(posStr);
                CCell cell;
                cell.originalValue = valueStr;
                table[pos] = cell; // Populate table map
            }

            // Now that the entire sheet is loaded into table, set the values for each cell
            for (auto &pair: table) {
                const CPos pos = pair.first;
                const std::string valueStr = pair.second.originalValue;
                setCell(pos, valueStr);
            }

            return true;
        } catch (...) {
            return false;
        }
    }


    bool setCell(CPos pos, std::string contents);

    CValue getValue(CPos pos);

    void copyRect(CPos dst, CPos src, int w = 1, int h = 1) {
        if (w == 0 || h == 0) return;  // Nothing to copy if width or height is zero

        // If either w or h is negative, adjust source coordinates
        if (w < 0) {
            src = CPos(src.getRow() + w + 1, src.getColumn());
            w = -w;
        }
        if (h < 0) {
            src = CPos(src.getRow(), src.getColumn() + h + 1);
            h = -h;
        }


        std::map<CPos, CCell> temp;  // Temporary map to store the cells being copied

        // Copy cells from the source to temp map
        for (int i = 0; i < w; ++i) {
            for (int j = 0; j < h; ++j) {
                CPos sourcePos(src.getRow() + j, src.getColumn() + i);
                CPos targetPos(dst.getRow() + j, dst.getColumn() + i);

                if (table.find(sourcePos) != table.end()) {
                    temp[targetPos] = table[sourcePos];
                }
            }
        }

        // Now place them back into the main table at the destination
        for (auto &entry: temp) {
            table[entry.first] = entry.second;
            if (entry.second.m_astNode) {
                // Adjust AST nodes to account for the move
                table[entry.first].m_astNode->copyReference(dst.getRow() - src.getRow(),
                                                            dst.getColumn() - src.getColumn());
            }
        }
    }

    CSpreadsheet &operator=(const CSpreadsheet &other) {
        // Check for self-assignment
        if (this != &other) {
            // Clear the current spreadsheet
            table.clear();

            // Iterate through the cells of the other spreadsheet
            for (const auto &pair: other.table) {
                const CPos &pos = pair.first;
                const CCell &cell = pair.second;

                // Set the value directly
                std::string value = cell.originalValue;

                // Set the value in the current spreadsheet
                setCell(pos, value);
            }
        }

        return *this;
    }


private:

    std::map<CPos, CCell> table; // Map representing SpreadSheet
};

bool CSpreadsheet::setCell(CPos pos, std::string contents) {
    try {
        CCell &cell = table[pos];


        MyExpressionBuilder builder;
        builder.setTableRef(table);

        if (!contents.empty()) {
            // Parse expression and construct AST using the builder
            parseExpression(contents, builder);
            cell.m_astNode = builder.getASTRoot();
            cell.originalValue = contents;
        }
    }
    catch (...) {
        return false;
    }
    return true;
}

CValue CSpreadsheet::getValue(CPos pos) {
    std::map<CPos, bool> haveBeenVisited;

    auto it = table.find(pos);
    if (it != table.end()) {

        const CCell &cell = it->second;


        // Get the value stored in the cell
        CValue value = cell.getValue(haveBeenVisited);

        // Check the type of the value and return accordingly
        if (std::holds_alternative<double>(value)) {
            return std::get<double>(value);
        } else if (std::holds_alternative<std::string>(value)) {
            return std::get<std::string>(value);
        }
    }

    // Handle unknown types by returning an empty CValue
    return CValue();
}


#ifndef __PROGTEST__

bool valueMatch(const CValue &r,
                const CValue &s) {
    if (r.index() != s.index())
        return false;
    if (r.index() == 0)
        return true;
    if (r.index() == 2)
        return std::get<std::string>(r) == std::get<std::string>(s);
    if (std::isnan(std::get<double>(r)) && std::isnan(std::get<double>(s)))
        return true;
    if (std::isinf(std::get<double>(r)) && std::isinf(std::get<double>(s)))
        return (std::get<double>(r) < 0 && std::get<double>(s) < 0)
               || (std::get<double>(r) > 0 && std::get<double>(s) > 0);
    return fabs(std::get<double>(r) - std::get<double>(s)) <= 1e8 * DBL_EPSILON * fabs(std::get<double>(r));
}

int main() {

    CSpreadsheet x0, x1;
    std::ostringstream oss;
    std::istringstream iss;
    std::string data;


    assert (x0.setCell(CPos("A1"), "10"));
    assert (x0.setCell(CPos("A2"), "20.5"));
    assert (x0.setCell(CPos("A3"), "3e1"));
    assert (x0.setCell(CPos("A4"), "=40"));
    assert (x0.setCell(CPos("A5"), "=5e+1"));
    assert (x0.setCell(CPos("A6"), "=\"raw text with any characters, including a quote \"\" or a newline\n\""));
    assert (x0.setCell(CPos("A7"),
                       "=\"quoted string, quotes must be doubled: \"\". Moreover, backslashes are needed for C++.\""));
    assert (valueMatch(x0.getValue(CPos("A1")), CValue(10.0)));
    assert (valueMatch(x0.getValue(CPos("A2")), CValue(20.5)));
    assert (valueMatch(x0.getValue(CPos("A3")), CValue(30.0)));
    assert (valueMatch(x0.getValue(CPos("A4")), CValue(40.0)));
    assert (valueMatch(x0.getValue(CPos("A5")), CValue(50.0)));
    assert (valueMatch(x0.getValue(CPos("A6")),
                       CValue("raw text with any characters, including a quote \" or a newline\n")));
    assert (valueMatch(x0.getValue(CPos("A7")),
                       CValue("quoted string, quotes must be doubled: \". Moreover, backslashes are needed for C++.")));
    assert (valueMatch(x0.getValue(CPos("A8")), CValue()));
    assert (valueMatch(x0.getValue(CPos("AAAA9999")), CValue()));
    assert (x0.setCell(CPos("B1"), "=A1+A2*A3"));
    assert (x0.setCell(CPos("B2"), "= -A1 ^ 2 - A2 / 2   "));
    assert (x0.setCell(CPos("B3"), "= 2 ^ $A$1"));
    assert (x0.setCell(CPos("B4"), "=($A1+A$2)^2"));
    assert (x0.setCell(CPos("B5"), "=B1+B2+B3+B4"));
    assert (x0.setCell(CPos("B6"), "=B1+B2+B3+B4+B5"));
    assert (valueMatch(x0.getValue(CPos("B1")), CValue(625.0)));
    assert (valueMatch(x0.getValue(CPos("B2")), CValue(-110.25)));
    assert (valueMatch(x0.getValue(CPos("B3")), CValue(1024.0)));
    assert (valueMatch(x0.getValue(CPos("B4")), CValue(930.25)));
    assert (valueMatch(x0.getValue(CPos("B5")), CValue(2469.0)));
    assert (valueMatch(x0.getValue(CPos("B6")), CValue(4938.0)));
    assert (x0.setCell(CPos("A1"), "12"));
    assert (valueMatch(x0.getValue(CPos("B1")), CValue(627.0)));
    assert (valueMatch(x0.getValue(CPos("B2")), CValue(-154.25)));
    assert (valueMatch(x0.getValue(CPos("B3")), CValue(4096.0)));
    assert (valueMatch(x0.getValue(CPos("B4")), CValue(1056.25)));
    assert (valueMatch(x0.getValue(CPos("B5")), CValue(5625.0)));
    assert (valueMatch(x0.getValue(CPos("B6")), CValue(11250.0)));
    x1 = x0;
    assert (x0.setCell(CPos("A2"), "100"));
    assert (x1.setCell(CPos("A2"), "=A3+A5+A4"));
    assert (valueMatch(x0.getValue(CPos("B1")), CValue(3012.0)));
    assert (valueMatch(x0.getValue(CPos("B2")), CValue(-194.0)));
    assert (valueMatch(x0.getValue(CPos("B3")), CValue(4096.0)));
    assert (valueMatch(x0.getValue(CPos("B4")), CValue(12544.0)));
    assert (valueMatch(x0.getValue(CPos("B5")), CValue(19458.0)));
    assert (valueMatch(x0.getValue(CPos("B6")), CValue(38916.0)));
    assert (valueMatch(x1.getValue(CPos("B1")), CValue(3612.0)));
    assert (valueMatch(x1.getValue(CPos("B2")), CValue(-204.0)));
    assert (valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
    assert (valueMatch(x1.getValue(CPos("B4")), CValue(17424.0)));
    assert (valueMatch(x1.getValue(CPos("B5")), CValue(24928.0)));
    assert (valueMatch(x1.getValue(CPos("B6")), CValue(49856.0)));


    oss.clear();
    oss.str("");
    assert (x0.save(oss));
    data = oss.str();
    iss.clear();
    iss.str(data);
    assert (x1.load(iss));
    assert (valueMatch(x1.getValue(CPos("B1")), CValue(3012.0)));
    assert (valueMatch(x1.getValue(CPos("B2")), CValue(-194.0)));
    assert (valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
    assert (valueMatch(x1.getValue(CPos("B4")), CValue(12544.0)));
    assert (valueMatch(x1.getValue(CPos("B5")), CValue(19458.0)));
    assert (valueMatch(x1.getValue(CPos("B6")), CValue(38916.0)));
    assert (x0.setCell(CPos("A3"), "4e1"));
    assert (valueMatch(x1.getValue(CPos("B1")), CValue(3012.0)));
    assert (valueMatch(x1.getValue(CPos("B2")), CValue(-194.0)));
    assert (valueMatch(x1.getValue(CPos("B3")), CValue(4096.0)));
    assert (valueMatch(x1.getValue(CPos("B4")), CValue(12544.0)));
    assert (valueMatch(x1.getValue(CPos("B5")), CValue(19458.0)));
    assert (valueMatch(x1.getValue(CPos("B6")), CValue(38916.0)));
    oss.clear();
    oss.str("");
    assert (x0.save(oss));
    data = oss.str();
    for (size_t i = 0; i < std::min<size_t>(data.length(), 10); i++)
        data[i] ^= 0x5a;
    iss.clear();
    iss.str(data);
    assert (!x1.load(iss));
    assert (x0.setCell(CPos("D0"), "10"));
    assert (x0.setCell(CPos("D1"), "20"));
    assert (x0.setCell(CPos("D2"), "30"));
    assert (x0.setCell(CPos("D3"), "40"));
    assert (x0.setCell(CPos("D4"), "50"));
    assert (x0.setCell(CPos("E0"), "60"));
    assert (x0.setCell(CPos("E1"), "70"));
    assert (x0.setCell(CPos("E2"), "80"));
    assert (x0.setCell(CPos("E3"), "90"));
    assert (x0.setCell(CPos("E4"), "100"));
    assert (x0.setCell(CPos("F10"), "=D0+5"));
    assert (x0.setCell(CPos("F11"), "=$D0+5"));
    assert (x0.setCell(CPos("F12"), "=D$0+5"));
    assert (x0.setCell(CPos("F13"), "=$D$0+5"));
    x0.copyRect(CPos("G11"), CPos("F10"), 1, 4);
    assert (valueMatch(x0.getValue(CPos("F10")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F11")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F12")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F13")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F14")), CValue()));
    assert (valueMatch(x0.getValue(CPos("G10")), CValue()));
    assert (valueMatch(x0.getValue(CPos("G11")), CValue(75.0)));
    assert (valueMatch(x0.getValue(CPos("G12")), CValue(25.0)));
    assert (valueMatch(x0.getValue(CPos("G13")), CValue(65.0)));
    assert (valueMatch(x0.getValue(CPos("G14")), CValue(15.0)));
    x0.copyRect(CPos("G11"), CPos("F10"), 2, 4);
    assert (valueMatch(x0.getValue(CPos("F10")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F11")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F12")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F13")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("F14")), CValue()));
    assert (valueMatch(x0.getValue(CPos("G10")), CValue()));
    assert (valueMatch(x0.getValue(CPos("G11")), CValue(75.0)));
    assert (valueMatch(x0.getValue(CPos("G12")), CValue(25.0)));
    assert (valueMatch(x0.getValue(CPos("G13")), CValue(65.0)));
    assert (valueMatch(x0.getValue(CPos("G14")), CValue(15.0)));
    assert (valueMatch(x0.getValue(CPos("H10")), CValue()));
    assert (valueMatch(x0.getValue(CPos("H11")), CValue()));
    assert (valueMatch(x0.getValue(CPos("H12")), CValue()));
    assert (valueMatch(x0.getValue(CPos("H13")), CValue(35.0)));
    assert (valueMatch(x0.getValue(CPos("H14")), CValue()));
    assert (x0.setCell(CPos("F0"), "-27"));
    assert (valueMatch(x0.getValue(CPos("H14")), CValue(-22.0)));
    x0.copyRect(CPos("H12"), CPos("H13"), 1, 2);
    assert (valueMatch(x0.getValue(CPos("H12")), CValue(25.0)));
    assert (valueMatch(x0.getValue(CPos("H13")), CValue(-22.0)));
    assert (valueMatch(x0.getValue(CPos("H14")), CValue(-22.0)));
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
