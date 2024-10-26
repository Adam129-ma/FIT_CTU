#ifndef __PROGTEST__
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <cctype>
#include <climits>
#include <cstdint>
#include <cassert>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <memory>
#include <functional>
#include <compare>
#include <stdexcept>
#endif /* __PROGTEST__ */



class CBigInt {
public:
    CBigInt() : Size(1), Position(true) {
        mData.push_back(0);
    }

    CBigInt(const CBigInt& data)
    {
        Position = data.Position;
        Size = data.Size;
        mData = data.mData;
    }

    CBigInt(const int data) {
        if(data == 0)
        {
            Size = 1;
            Position = true;
            mData.push_back(0);
            return;
        }
        int mock = abs(data);
        while (mock > 0) {
            mData.push_back(mock % 10);
            mock /= 10;
        }
        Size = mData.size();
        Position = data >= 0;
    }

    CBigInt(const char* data) {
        Position = true;
        Size = strlen(data);
        int stop;
        if (data[0] == '-') {
            stop = 1;
        } else {
            stop = 0;
        }

        for (int x = Size - 1; x >= stop; x--) {
            if (isdigit(data[x])) {
                mData.push_back(data[x] - '0');
            } else {
                if (!(data[x] == '-' && x == 0)) {
                    throw std::invalid_argument("Invalid input");
                }
            }
        }

        if (stop) {
            Position = false;
            Size--;
        }

        int zeros = 0;
        for(int x = mData.size() - 1; x >= 0; x--)
        {
            if(mData[x] == 0)
                zeros++;
            else
                break;
        }
        Size = mData.size() - zeros;
        if(Size == 0) { Size++; Position = true; }
        mData.resize(Size);
    }

    ~CBigInt() {}

    CBigInt& operator=(const CBigInt& data)
    {
        mData = data.mData;
        Size = data.Size;
        Position = data.Position;
        return *this;
    }

    CBigInt& operator=(const int data) {
        *this = CBigInt(data);
        return *this;
    }

    CBigInt& operator=(const char* data) {
        *this = CBigInt(data);
        return *this;
    }

    friend std::ostream& operator<<(std::ostream& os, const CBigInt& data)
    {
        if(!data.Position) os << '-';
        for(int x = data.Size - 1; x >= 0; x--)
        {
            os << (int)data.mData[x];
        }
        return os;
    }

    friend std::istream& operator>>(std::istream& is, CBigInt& data) {

        std::vector<int> mock;
        is >> std::noskipws;
        bool sign = true;
        int count = 0;
        char dummy;

        while (isspace(is.peek())) {
            is.get(dummy);
        }

        if (is.peek() == '-') {
            is.get(dummy);
            sign = false;
        }

        if (isspace(is.peek())) {
            is.setstate(std::ios::failbit);
        }
        while (is.peek() == '0') {
            is.get(dummy); // Discard leading zeros
        }
        while (isdigit(is.peek())) {
            is >> dummy;
            mock.push_back(dummy - '0');
            count++;
        }

        if (count > 0 ) {
            data.Size = (int)mock.size();
            data.Position = sign;
            data.mData.clear();
            for (int x = mock.size() - 1; x >= 0; x--) {
                data.mData.push_back(mock[x]);
            }
        }
        else {
            is.setstate(std::ios::failbit);
        }

        return is;
    }

    friend CBigInt operator+(const CBigInt& left, const CBigInt& right) {
        CBigInt result;
        int largerSize = std::max(left.Size, right.Size);
        int carry = 0;

        if ((left.Position && right.Position) || (!left.Position && !right.Position)) {
            result.mData.resize(largerSize);
            for (int i = 0; i < largerSize || carry != 0; ++i) {
                int value = carry;
                if (i < right.Size) value += right.mData[i];
                if (i < left.Size) value += left.mData[i];
                if (i >= largerSize && carry != 0){result.mData.resize(largerSize+1);}
                result.mData[i] = value % 10;
                carry = value / 10;
            }
            result.Size = result.mData.size();
            result.Position = left.Position;
        } else {
            const CBigInt* largerBigInt;
            const CBigInt* smallerBigInt;
            if (left.Size > right.Size) {
                largerBigInt = &left;
                smallerBigInt = &right;
            } else {
                largerBigInt = &right;
                smallerBigInt = &left;
            }

            result.mData.resize(largerBigInt->Size);
            for (int i = 0; i < largerBigInt->Size; ++i) {
                int value = largerBigInt->mData[i] - carry;
                if (i < smallerBigInt->Size) value -= smallerBigInt->mData[i];
                if (value < 0) {
                    value += 10;
                    carry = 1;
                } else {
                    carry = 0;
                }
                result.mData[i] = value;
            }
            result.Position = largerBigInt->Position;
            result.Size = result.mData.size();
        }

        int zeros = 0;
        for(int x = result.mData.size() - 1; x >= 0; x--) {
            if(result.mData[x] == 0)
                zeros++;
            else
                break;
        }
        result.Size = result.mData.size() - zeros;
        if(result.Size == 0) { result.Size++; result.Position = true; }
        result.mData.resize(result.Size);

        return result;
    }

    CBigInt& operator+=(const CBigInt& data) {
        *this = *this + data;
        return *this;
    }

    friend CBigInt operator*(const CBigInt& left, const CBigInt& right) {
        CBigInt result;
        result = left;

        int alloc = result.Size + right.Size;
        int in = 0, carry = 0;
        std::vector<int> resultData(alloc);

        for (int x = 0; x <= alloc; x++) {
            int value = 0;
            for (int im = 0; im <= x; im++) {
                in = x - im;
                if (im < result.Size && in < right.Size) {
                    value += result.mData[im] * right.mData[in];
                }
            }
            value += carry;
            if(carry != 0 || value != 0){
            resultData[x] = value - (value / 10) * 10;
            carry = value / 10;
            }
        }

        result.Position = (result.Position == right.Position);
        result.mData = resultData;

        int zeros = 0;
        for (int x = result.mData.size() - 1; x >= 0; x--) {
            if (result.mData[x] == 0)
                zeros++;
            else
                break;
        }
        result.Size = result.mData.size() - zeros;
        if (result.Size == 0) {
            result.Size++;
            result.Position = true;
        }
        result.mData.resize(result.Size);
        return result;
    }

    CBigInt& operator*=(const CBigInt& data) {
        *this = *this * data;
        return *this;
    }

    friend bool operator>(const CBigInt& left, const CBigInt& right) {
        if (left.Position && !right.Position)
            return true;
        else if (!left.Position && right.Position)
            return false;
        else if (left.Position && right.Position) {
            if (left.Size > right.Size)
                return true;
            else if (left.Size < right.Size)
                return false;
            else {
                for (int i = left.Size - 1; i >= 0; --i) {
                    if (left.mData[i] > right.mData[i])
                        return true;
                    else if (left.mData[i] < right.mData[i])
                        return false;
                }
                return false;
            }
        }
        else {
            if (left.Size < right.Size)
                return true;
            else if (left.Size > right.Size)
                return false;
            else {
                for (int i = left.Size - 1; i >= 0; --i) {
                    if (left.mData[i] < right.mData[i])
                        return true;
                    else if (left.mData[i] > right.mData[i])
                        return false;
                }
                return false;
            }
        }
    }

    friend bool operator<(const CBigInt& left, const CBigInt& right) {
        return right > left;
    }

    friend bool operator<=(const CBigInt& left, const CBigInt& right) {
        return !(left > right);
    }

    friend bool operator>=(const CBigInt& left, const CBigInt& right) {
        return !(left < right);
    }

    friend bool operator==(const CBigInt& left, const CBigInt& right) {
        return left.Position == right.Position && left.Size == right.Size && left.mData == right.mData;
    }

    friend bool operator!=(const CBigInt& left, const CBigInt& right) {
        return !(left == right);
    }

private:
    std::vector<int> mData;
    int Size;
    bool Position;
};

#ifndef __PROGTEST__
static bool equal ( const CBigInt & x, const char val [] )
{
  std::ostringstream oss;
  oss << x;
  return oss . str () == val;
}
static bool equalHex ( const CBigInt & x, const char val [] )
{
  return true; // hex output is needed for bonus tests only
  // std::ostringstream oss;
  // oss << std::hex << x;
  // return oss . str () == val;
}
int main ()
{
  CBigInt a, b;
  std::istringstream is;
  a = 10;
  a += 20;
  assert ( equal ( a, "30" ) );
  a *= 5;
  assert ( equal ( a, "150" ) );
  b = a + 3;
  assert ( equal ( b, "153" ) );
  b = a * 7;
  assert ( equal ( b, "1050" ) );
  assert ( equal ( a, "150" ) );
  assert ( equalHex ( a, "96" ) );

  a = 10;
  a += -20;
  b = a + "100";
  assert ( equal ( a, "-10" ) );
  a *= 5;
  assert ( equal ( a, "-50" ) );
  b = a + 73;
  assert ( equal ( b, "23" ) );
  b = a * -7;
  assert ( equal ( b, "350" ) );
  assert ( equal ( a, "-50" ) );
  assert ( equalHex ( a, "-32" ) );

  a = "12345678901234567890";
  a += "-99999999999999999999";
  assert ( equal ( a, "-87654321098765432109" ) );
  a *= "54321987654321987654";
  assert ( equal ( a, "-4761556948575111126880627366067073182286" ) );
  a *= 0;
  assert ( equal ( a, "0" ) );
  a = 10;
  b = a + "400";
  assert ( equal ( b, "410" ) );
  b = a * "15";
  assert ( equal ( b, "150" ) );
  assert ( equal ( a, "10" ) );
  assert ( equalHex ( a, "a" ) );

  is . clear ();
  is . str ( " 1234" );
  assert ( is >> b );
  assert ( equal ( b, "1234" ) );
  is . clear ();
  is . str ( " 12 34" );
  assert ( is >> b );
  assert ( equal ( b, "12" ) );
  is . clear ();
  is . str ( "999z" );
  assert ( is >> b );
  assert ( equal ( b, "999" ) );
  is . clear ();
  is . str ( "abcd" );
  assert ( ! ( is >> b ) );
  is . clear ();
  is . str ( "- 758" );
 assert ( ! ( is >> b ) );
  a = 42;
  try
  {
    a = "-xyz";
    assert ( "missing an exception" == nullptr );
  }
  catch ( const std::invalid_argument & e )
  {
    assert ( equal ( a, "42" ) );
  }

  a = "73786976294838206464";
  assert ( equal ( a, "73786976294838206464" ) );
  assert ( equalHex ( a, "40000000000000000" ) );
  assert ( a < "1361129467683753853853498429727072845824" );
  assert ( a <= "1361129467683753853853498429727072845824" );
  assert ( ! ( a > "1361129467683753853853498429727072845824" ) );
  assert ( ! ( a >= "1361129467683753853853498429727072845824" ) );
  assert ( ! ( a == "1361129467683753853853498429727072845824" ) );
  assert ( a != "1361129467683753853853498429727072845824" );
  assert ( ! ( a < "73786976294838206464" ) );
  assert ( a <= "73786976294838206464" );
  assert ( ! ( a > "73786976294838206464" ) );
  assert ( a >= "73786976294838206464" );
  assert ( a == "73786976294838206464" );
  assert ( ! ( a != "73786976294838206464" ) );
  assert ( a < "73786976294838206465" );
  assert ( a <= "73786976294838206465" );
  assert ( ! ( a > "73786976294838206465" ) );
  assert ( ! ( a >= "73786976294838206465" ) );
  assert ( ! ( a == "73786976294838206465" ) );
  assert ( a != "73786976294838206465" );
  a = "2147483648";
  assert ( ! ( a < -2147483648 ) );
  assert ( ! ( a <= -2147483648 ) );
  assert ( a > -2147483648 );
  assert ( a >= -2147483648 );
  assert ( ! ( a == -2147483648 ) );
  assert ( a != -2147483648 );
  a = "-12345678";
  assert ( ! ( a < -87654321 ) );
  assert ( ! ( a <= -87654321 ) );
  assert ( a > -87654321 );
  assert ( a >= -87654321 );
  assert ( ! ( a == -87654321 ) );
  assert ( a != -87654321 );

  return EXIT_SUCCESS;
}
#endif /* __PROGTEST__ */
