#ifndef __PROGTEST__

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <vector>
#include <set>
#include <list>
#include <map>
#include <array>
#include <deque>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <compare>
#include <algorithm>
#include <cassert>
#include <memory>
#include <iterator>
#include <functional>
#include <stdexcept>

using namespace std::literals;

class CDummy {
public:
    CDummy(char c)
            : m_C(c) {
    }

    bool operator==(const CDummy &other) const = default;

    friend std::ostream &operator<<(std::ostream &os,
                                    const CDummy &x) {
        return os << '\'' << x.m_C << '\'';
    }

private:
    char m_C;
};

#endif /* __PROGTEST__ */

// #define TEST_EXTRA_INTERFACE

template<typename T_>
class CSelfMatch {
public:

    CSelfMatch(std::initializer_list<T_> initList)
            : m_Data(initList) {}


    template<typename Iterator>
    CSelfMatch(Iterator begin, Iterator end)
            : m_Data(begin, end) {}

    template<typename Container>
    CSelfMatch(const Container &container)
            : m_Data(container.begin(), container.end()) {}


    template<typename... Args>
    void push_back(const Args &... args) {
        (m_Data.push_back(args), ...);
    }


    size_t sequenceLen(size_t n) const {
        if (n == 0)
            throw std::invalid_argument("Invalid argument: n must be greater than 0.");

        size_t listLen = m_Data.size();
        size_t maxLen = 0;

        for (size_t subLen = listLen; subLen >= 1; --subLen) {
            for (size_t start = 0; start <= listLen - subLen; ++start) {
                auto it = m_Data.begin();
                std::advance(it, start);

                size_t count = 1;
                for (size_t nextStart = start + 1; nextStart <= listLen - subLen; ++nextStart) {
                    auto nextIt = m_Data.begin();
                    std::advance(nextIt, nextStart);

                    bool match = true;
                    for (size_t i = 0; i < subLen; ++i) {
                        if (*std::next(it, i) != *std::next(nextIt, i)) {
                            match = false;
                            break;
                        }
                    }

                    if (match) {
                        ++count;
                    }
                }

                if (count >= n) {
                    maxLen = subLen;
                    break;
                }
            }

            if (maxLen > 0) {
                break;
            }
        }

        return maxLen;
    }

    template<size_t N_>
    std::vector<std::array<size_t, N_>> findSequences() const {
        if (N_ == 0)
            throw std::invalid_argument("Invalid argument: N must be greater than 0.");

        std::vector<std::array<size_t, N_>> result;

        size_t listLen = m_Data.size();

        for (size_t subLen = listLen; subLen >= 1; --subLen) {
            for (size_t start = 0; start <= listLen - subLen; ++start) {
                auto it = std::next(m_Data.begin(), start);

                size_t count = 1;
                std::vector<size_t> positions = { start };

                for (size_t nextStart = start + 1; nextStart <= listLen - subLen; ++nextStart) {
                    auto nextIt = std::next(m_Data.begin(), nextStart);

                    bool match = true;
                    for (size_t i = 0; i < subLen; ++i) {
                        if (*std::next(it, i) != *std::next(nextIt, i)) {
                            match = false;
                            break;
                        }
                    }

                    if (match) {
                        ++count;
                        positions.push_back(nextStart);
                    }
                }

                if (count >= N_) {
                    generateCombinations(result, positions, N_);
                }
            }

            if (!result.empty()) {
                return result;
            }
        }

        return result;
    }




private:
    std::list<T_> m_Data;


    template<size_t N_>
    void generateCombinations(std::vector<std::array<size_t, N_>> &result,
                              const std::vector<size_t> &positions, size_t remaining,
                              size_t startIndex = 0, std::array<size_t, N_> current = {}) const {
        if (remaining == 0) {
            if (current[0] == positions[0]){
            result.push_back(current);
            }
            return;
        }

        for (size_t i = startIndex; i <= positions.size() - remaining; ++i) {
            auto nextCurrent = current;
            nextCurrent[N_ - remaining] = positions[i];
            generateCombinations(result, positions, remaining - 1, i + 1, nextCurrent);
        }
    }
};


#ifndef __PROGTEST__

template<size_t N_>
bool positionMatch(std::vector<std::array<size_t, N_>> a,
                   std::vector<std::array<size_t, N_>> b) {
    std::sort(a.begin(), a.end());
    std::sort(b.begin(), b.end());
    return a == b;
}

int main() {
    CSelfMatch<char> x0("aaaaaaaaaaa"s);
    assert (x0.sequenceLen(2) == 10);
    assert (positionMatch(x0.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 1}}));
    CSelfMatch<char> x1("abababababa"s);
    assert (x1.sequenceLen(2) == 9);
    assert (positionMatch(x1.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 2}}));
    CSelfMatch<char> x2("abababababab"s);
    assert (x2.sequenceLen(2) == 10);
    assert (positionMatch(x2.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 2}}));
    CSelfMatch<char> x3("aaaaaaaaaaa"s);
    assert (x3.sequenceLen(3) == 9);
    assert (positionMatch(x3.findSequences<3>(), std::vector<std::array<size_t, 3> >{{0, 1, 2}}));
    CSelfMatch<char> x4("abababababa"s);
    assert (x4.sequenceLen(3) == 7);
    assert (positionMatch(x4.findSequences<3>(), std::vector<std::array<size_t, 3> >{{0, 2, 4}}));
    CSelfMatch<char> x5("abababababab"s);
    assert (x5.sequenceLen(3) == 8);
    assert (positionMatch(x5.findSequences<3>(), std::vector<std::array<size_t, 3> >{{0, 2, 4}}));
    CSelfMatch<char> x6("abcdXabcd"s);
    assert (x6.sequenceLen(1) == 9);
    assert (positionMatch(x6.findSequences<1>(), std::vector<std::array<size_t, 1> >{{0}}));
    CSelfMatch<char> x7("abcdXabcd"s);
    assert (x7.sequenceLen(2) == 4);
    assert (positionMatch(x7.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 5}}));
    CSelfMatch<char> x8("abcdXabcdeYabcdZabcd"s);
    assert (x8.sequenceLen(2) == 4);
    assert (positionMatch(x8.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0,  5},
                                                                                     {0,  11},
                                                                                     {0,  16},
                                                                                     {5,  11},
                                                                                     {5,  16},
                                                                                     {11, 16}}));
    CSelfMatch<char> x9("abcdXabcdYabcd"s);
    assert (x9.sequenceLen(3) == 4);
    assert (positionMatch(x9.findSequences<3>(), std::vector<std::array<size_t, 3> >{{0, 5, 10}}));
    CSelfMatch<char> x10("abcdefghijklmn"s);
    assert (x10.sequenceLen(2) == 0);
    assert (positionMatch(x10.findSequences<2>(), std::vector<std::array<size_t, 2> >{}));
    CSelfMatch<char> x11("abcXabcYabcZdefXdef"s);
    assert (x11.sequenceLen(2) == 3);
    assert (positionMatch(x11.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0,  4},
                                                                                      {0,  8},
                                                                                      {4,  8},
                                                                                      {12, 16}}));
    CSelfMatch<int> x12{1, 2, 3, 1, 2, 4, 1, 2};
    assert (x12.sequenceLen(2) == 2);
    assert (positionMatch(x12.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 3},
                                                                                      {0, 6},
                                                                                      {3, 6}}));
    assert (x12.sequenceLen(3) == 2);
    assert (positionMatch(x12.findSequences<3>(), std::vector<std::array<size_t, 3> >{{0, 3, 6}}));
    std::initializer_list<CDummy> init13{'a', 'b', 'c', 'd', 'X', 'a', 'b', 'c', 'd', 'Y', 'a', 'b', 'c', 'd'};
    CSelfMatch<CDummy> x13(init13.begin(), init13.end());
    assert (x13.sequenceLen(2) == 4);
    assert (positionMatch(x13.findSequences<2>(), std::vector<std::array<size_t, 2> >{{0, 5},
                                                                                      {0, 10},
                                                                                      {5, 10}}));
    std::initializer_list<int> init14{1, 2, 1, 1, 2, 1, 0, 0, 1, 2, 1, 0, 1, 2, 0, 1, 2, 0, 1, 1, 1, 2, 0, 2, 0, 1, 2,
                                      1, 0};
    CSelfMatch<int> x14(init14.begin(), init14.end());
    assert (x14.sequenceLen(2) == 5);
    assert (positionMatch(x14.findSequences<2>(), std::vector<std::array<size_t, 2> >{{11, 14},
                                                                                      {7,  24}}));
    std::initializer_list<int> init15{1, 2, 1, 1, 2, 1, 0, 0, 1, 2, 1, 0, 1, 2, 0, 1, 2, 0, 1, 1, 1, 2, 0, 2, 0, 1, 2,
                                      1, 0};
    CSelfMatch<int> x15(init15.begin(), init15.end());
    assert (x15.sequenceLen(3) == 4);
    assert (positionMatch(x15.findSequences<3>(), std::vector<std::array<size_t, 3> >{{3, 8, 25}}));
#ifdef TEST_EXTRA_INTERFACE
    CSelfMatch y0 ( "aaaaaaaaaaa"s );
    assert ( y0 . sequenceLen ( 2 ) == 10 );

    std::string s1 ( "abcd" );
    CSelfMatch y1 ( s1 . begin (), s1 . end () );
    assert ( y1 . sequenceLen ( 2 ) == 0 );

    CSelfMatch y2 ( ""s );
    y2 . push_back ( 'a', 'b', 'c', 'X' );
    y2 . push_back ( 'a' );
    y2 . push_back ( 'b', 'c' );
    assert ( y2 . sequenceLen ( 2 ) == 3 );
#endif /* TEST_EXTRA_INTERFACE */
    return EXIT_SUCCESS;
}

#endif /* __PROGTEST__ */
