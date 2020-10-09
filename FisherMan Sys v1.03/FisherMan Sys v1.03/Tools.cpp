/*
 * File: Tools.cpp
 * ---------------
 * This file implements the q2.h interface,
 * which contains necessary tools while running.
 */

#include <algorithm>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ostream>
#include <cassert>
#include <cstdio>
#include <string>
#include <vector>
#include <cctype>
#include <ctime>
#include <cmath>

#include "Tools.h"

using namespace std;

/*
 * Implementation notes: conert
 * -----------------------------
 * Convert integer number to string value.
 */
string convert(int n) {
    string ans = "";
    while(n != 0) {
        ans = (char)('0'+n%10)+ans;
        n /= 10;
    }
    return ans;
}

/*
 * Implementation notes: intConvert
 * --------------------------------
 * Convert number string to int or float value.
 */
int intConvert(const string &s) {
    int ans=0;
    for(int i=0; i<(int)s.length(); i++)
        ans = ans*10+(s[i]-'0');
    return ans;
}

/*
 * Implementation notes: floatConvert
 * ----------------------------------
 * Convert number string to int or float value.
 */
float floatConvert(const string &s) {
    float ans=0, base=1;
    bool fraction = false;
    for(int i=0; i<(int)s.length(); i++)	{
        if(s[i]=='.') {
            fraction = true;
            continue;
        }
        if(!fraction)
            ans = ans*10+(s[i]-'0');
        else {
            base *= 0.1;
            ans = ans+base*(s[i]-'0');
        }
    }
    return ans;
}

/*
 * Implementation notes: getLocalTime
 * ----------------------------------
 * Get access to the local time of the system.
 */

int getLocalTime(const string &s) {
    struct tm *local;
    time_t t = time(NULL);
    local = localtime(&t);
    if(s == "year")
        return local->tm_year;
    if(s == "month")
        return local->tm_mon;
    if(s == "monthday")
        return local->tm_mday;
    if(s == "weekday")
        return local->tm_wday;
    if(s == "hour")
        return local->tm_hour;
    if(s == "minute")
        return local->tm_min;
    if(s == "second")
        return local->tm_sec;
    return -1;
}

/*
 * Constant values.
 */
const BigInteger BigInteger::ZERO = BigInteger(0);
const BigInteger BigInteger::ONE = BigInteger(1);
const BigInteger BigInteger::TWO = BigInteger(2);
const BigInteger BigInteger::TEN = BigInteger(10);

/*
 * Implementation notes: BigInteger
 * --------------------------------
 * Initializes a new BigInteger value according to the previous BigInteger value.
 */
BigInteger::BigInteger(const BigInteger & val) {
    *this = val;
}

/*
 * Implementation notes: BigInteger
 * --------------------------------
 * Initializes a new BigInteger value according to the HEXADECIMAL string value.
 */
BigInteger::BigInteger(const string & str): is_negative(false) {
    string t(str);
    // negative judgement
    if (t.size() && t.at(0)=='-') {
        if (t.size() > 1)
            is_negative = true;
        t = t.substr(1);
    }
    // complement the string while the length is not multiple of 8.
    int cnt = (8-(t.size()%8))%8;
    string temp;
    for (int i=0; i<cnt; ++i)
        temp.push_back('0');
    t = temp+t;
    // push bit value into vector
    for (int i=0; i<(int)t.size(); i+=base_char) {
        unsigned sum = 0;
        for (int j=0; j<base_char; ++j) {
            char ch = t[i+j];
            int num = hexToNum(ch);
            sum = ((sum<<4) | (num));
        }
        data.push_back(sum);
    }
    // reverse the order of bits
    reverse(data.begin(), data.end());
    // eliminate all MSB insignificant "0"
    *this = trim();
}

/*
 * Implementation notes: BigInteger
 * --------------------------------
 * Initializes a new BigInteger value according to long long variable "num".
 */
BigInteger::BigInteger(const long long & num): is_negative(false) {
    long long t = num;
    // negative judgement
    if (t < 0) {
        is_negative = true;
        t = -t;
    }
    // push bit value into vector by binary logic right shift operator >>
    do {
        unsigned temp = (t&base_num);
        data.push_back(temp);
        t >>= base_int;
    } while (t);
}

/*
 * Implementation notes: add
 * -------------------------
 * Adds two BigInteger value bit by bit.
 */
BigInteger BigInteger::add(const BigInteger & val) {
    BigInteger ans(*this);
    // both two BigInteger are positive or both two BigInteger are negative
    if (ans.is_negative == val.is_negative) {
        int len = val.data.size()-ans.data.size();
        while ((len--) > 0)     // complement "0" to ensure the same number of bit
            ans.data.push_back(0);
        int carry = 0;  // carrier
        for (int i=0; i<(int)val.data.size(); ++i) {
            unsigned temp = ans.data[i];
            ans.data[i] += val.data[i]+carry;
            carry = (temp>ans.data[i] ? 1 : (temp>(temp+val.data[i]) ? 1 : 0));
        }
        for (int i=val.data.size(); i<(int)ans.data.size() && carry!=0; ++i) {
            unsigned temp = ans.data[i];
            ans.data[i] += carry;
            carry = temp > ans.data[i];
        }
        if (carry)
            ans.data.push_back(carry);
    }
    // these two BigInteger is one positive and the other negative
    else {
        BigInteger a = abs();
        BigInteger b = val.abs();
        int flag = a.compareTo(b);
        if (flag == -1) {
            ans = b.subtract(a);
            ans.is_negative = val.is_negative;
        }
        else if (flag == 0)
            ans = ZERO;
        else {
            ans = a.subtract(b);
            ans.is_negative = is_negative;
        }
    }
    return ans;
}

/*
 * Implementation notes: subtract
 * ------------------------------
 * Subtracts two BigInteger value bit by bit.
 */
BigInteger BigInteger::subtract(const BigInteger & val) {
    BigInteger ans(*this);
    BigInteger a = abs();
    BigInteger b = val.abs();
    // both two BigInteger are positive or both two BigInteger are negative
    if (ans.is_negative == val.is_negative) {
        int flag = a.compareTo(b);
        if (flag == 1) {
            int borrow = 0;     // borrower
            for (int i=0; i<(int)val.data.size(); ++i) {
                unsigned temp = ans.data[i];
                ans.data[i] -= val.data[i]+borrow;
                borrow = temp<ans.data[i] ? 1 : (temp-borrow<val.data[i] ? 1 : 0);
            }
            for (int i=val.data.size(); i<(int)ans.data.size()&&borrow!=0; ++i) {
                unsigned temp = ans.data[i];
                ans.data[i] -= borrow;
                borrow = temp < (unsigned)borrow;
            }
            ans = ans.trim();
        }
        else if (flag == 0)
            ans = ZERO;
        else {
            ans = b.subtract(a);
            ans.is_negative = !is_negative;
        }
    }
    // these two BigInteger is one positive and the other negative
    else {
        ans = a.add(b);
        ans.is_negative = is_negative;
    }
    return ans;
}

/*
 * Implementation notes: multiply
 * ------------------------------
 * Multiplies two BigInteger value by shift left method.
 */
BigInteger BigInteger::multiply(const BigInteger & val) const {
    if (equals(ZERO) || val.equals(ZERO))
        return ZERO;
    // Set the BigInteger whose number of bit is less.
    const BigInteger & big = data.size()>val.data.size() ? (*this) : val;
    const BigInteger & small = (&big)==(this) ? val : (*this);
    BigInteger ans;
    bit t(small);
    for (int i=t.size()-1; i>=0; --i)
        if (t.at(i)) {
            BigInteger temp(big);
            temp.is_negative = false;
            temp = temp.shiftLeft(i);
            ans = ans.add(temp);
        }
    ans.is_negative = !(is_negative == val.is_negative);
    return ans;
}

/*
 * Implementation notes: division
 * ------------------------------
 * Divides two BigInteger value and returns the quotient.
 */
BigInteger BigInteger::divide(const BigInteger & val) {
    BigInteger temp;
    return divideAndRemainder(val, temp);
}

/*
 * Implementation notes: remainder
 * -------------------------------
 * Divides two BigInteger value and returns the remainder.
 */
BigInteger BigInteger::remainder(const BigInteger & val) {
    BigInteger ans;
    divideAndRemainder(val, ans);
    return ans;
}

/*
 * Implementation notes: mod
 * -------------------------
 * Calculate bign1 % bign2.
 */
BigInteger BigInteger::mod(const BigInteger & m) {
    BigInteger ans = remainder(m);
    if (ans.is_negative)    ans = ans.add(m);   // ensure ans is positive
    return ans;
}

/*
 * Implementation notes: divideAndRemainder
 * ----------------------------------------
 * Realizes division operation by shift bits.
 * Inputs divisor as "val".
 * Returns the quotient as the result of this function and
 * returns the remainder as the reference variable "m".
 */
BigInteger BigInteger::divideAndRemainder(const BigInteger & val, BigInteger & m) {
    // avoid 0 divisor
    assert(!val.equals(ZERO));
    BigInteger a = abs();
    BigInteger b = val.abs();
    int flag = a.compareTo(b);
    // bign1 == bign2 <=> bign1/bign2 == 1 ... 0
    if (flag == 0)
        return (is_negative==val.is_negative) ? BigInteger(1) : BigInteger(-1);
    // bign1 < bign2 <=> bign1/bign2 == 0 ... bign1
    if (flag == -1) {
        m = *this;
        return ZERO;
    }
    BigInteger ans;
    // bit shift
    bit bit_b(b);
    while (true) {
        bit bit_a(a);
        int len = bit_a.size()-bit_b.size();
        BigInteger temp;
        while (len >= 0) {
            temp = b.shiftLeft(len);
            if (temp.compareTo(a) != 1)
                break;
            --len;
        }
        if (len < 0)
            break;
        unsigned num = 0;
        while (temp.compareTo(a) != 1) {
            a = a.subtract(temp);
            ++num;
        }
        temp = BigInteger(num);
        if (len)
            temp = temp.shiftLeft(len);
        ans = ans.add(temp);
    }
    ans.is_negative = !(is_negative==val.is_negative);
    m.data = a.data;
    m.is_negative = is_negative;
    return ans;
}

/*
 * Implementation notes: pow
 * -------------------------
 * Calculate pow(bign1, bign2) by fast exponentiation method.
 */
BigInteger BigInteger::pow(const BigInteger & exponent) {
    BigInteger ans(1);
    bit t(exponent);
    for (int i=t.size()-1; i>=0; --i) {
        ans = ans.multiply(ans);
        if (t.at(i))    // if even
            ans = multiply(ans);
    }
    return ans;
}

/*
 * Implementation notes: modPow
 * ----------------------------
 * Calculate pow(bign1, bign2) int %m condition by fast exponentiation method.
 */
BigInteger BigInteger::modPow(const BigInteger & exponent, const BigInteger & m) const {
    // avoid mod 0
    assert(!m.equals(ZERO));
    BigInteger ans(1);
    bit t(exponent);
    for (int i=t.size()-1; i>=0; --i) {
        ans = ans.multiply(ans).mod(m);
        if (t.at(i))    // if even
            ans = multiply(ans).mod(m);
    }
    return ans;
}

/*
 * Implementation notes: modInverse
 * --------------------------------
 * Calculates the inverse value (%m) in the sense of number theory using the extension of Euclidean algorithm.
 */
BigInteger BigInteger::modInverse(const BigInteger & m) {
    // cannot calculate inverse of negative value
    assert(!is_negative);
    // cannot mod 0
    assert(!m.is_negative);
    if (equals(ZERO) || m.equals(ZERO))
        return ZERO;
    BigInteger a[3], b[3], t[3];
    a[0] = 0; a[1] = 1; a[2] = *this;
    b[0] = 1; b[1] = 0; b[2] = m;
    // t[2] is remainder of a[2]%b[2], if t[2]==0, b[2] is the GCD.
    for (t[2]=a[2].mod(b[2]); !t[2].equals(ZERO); t[2]=a[2].mod(b[2])) {
        BigInteger temp = a[2].divide(b[2]);
        for (int i=0; i<3; ++i) {
            t[i] = a[i].subtract(temp.multiply(b[i]));
            // nwe_a = b
            a[i] = b[i];
            // new_b = a-floor(a/b) - b
            b[i] = t[i];
        }
    }
    if (b[2].equals(ONE)) {
        if (b[1].is_negative)
            b[1] = b[1].add(m); // ensure the result positive
        return b[1];
    }
    return ZERO;    // no inverse exists
}

/*
 * Implementation notes: shiftLeft
 * -------------------------------
 * Realizes logic left shit of binary operation.
 */
BigInteger BigInteger::shiftLeft(const unsigned len) {
    int index = len>>base_bit;
    int shift = len&base_temp;
    BigInteger ans(*this);
    int inc = (shift==0) ? index : index+1;
    for (int i=0; i<inc; ++i)
        ans.data.push_back(0);  // complement "0" at MSB
    if (index) {
        inc = (shift==0) ? 1 : 2;
        for (int i=ans.data.size()-inc; i>=index; --i)
            ans.data[i] = ans.data[i-index];
        for (int i=0; i<index; ++i)
            ans.data[i] = 0;
    }
    if (shift) {
        unsigned t = base_num;
        t <<= base_int-shift;   // shift left
        unsigned temp = 0;
        for (int i=0; i<(int)ans.data.size(); ++i) {
            unsigned tmp = ans.data[i];
            ans.data[i] = (tmp<<shift) | temp;
            temp = (tmp&t)>>(base_int-shift);
        }
    }
     ans = ans.trim();
    return ans;
}

/*
 * Implementation notes: shiftRight
 * --------------------------------
 * Realizes logic left right of binary operation.
 */
BigInteger BigInteger::shiftRight(const unsigned len) {
    bit val(*this);
    if (len >= (unsigned)val.size())
        return ZERO;
    int index = len>>base_bit;
    int shift = len&base_temp;
    BigInteger ans(*this);
    if (index) {
        for (int i=0; i<index; ++i)
            ans.data[i] = ans.data[i+index];
        for (int i=0; i<index; ++i)
            ans.data.pop_back();    // eliminate MSB
    }
    if (shift) {
        unsigned t = base_num;
        t >>= base_int-shift;   // shift right
        unsigned temp = 0;
        for (int i=ans.data.size()-1; i>=0; --i) {
            unsigned tmp = ans.data[i];
            ans.data[i] = (tmp>>shift) | temp;
            temp = (tmp&t)<<(base_int-shift);
        }
    }
    ans = ans.trim();
    return ans;
}

/*
 * Implementation notes: compareTo
 * --------------------------------
 * Compares values of bign1 and bign2.
 * Returns -1 <=> bign1<bign2.
 * Returns 0 <=> bign1==bign2.
 * Returns 1  <=> bign1>bign2
 */
int BigInteger::compareTo(const BigInteger & val) const {
    // negative value must be smaller than positive value
    if (is_negative != val.is_negative) {
        if (is_negative == true)
            return -1;
        return 1;
    }
    int flag = 0;
    // more bit value musst be larger than less bit value
    if (data.size() < val.data.size())
        flag = -1;
    else if (data.size() > val.data.size())
        flag = 1;
    else {
        for (vector<unsigned>::const_reverse_iterator it=data.rbegin(), ite=val.data.rbegin(); it!=data.rend(); ++it, ++ite)
            if ((*it) != (*ite)) {
                flag = (*it)<(*ite) ? -1 : 1;   // different MSB value leads to comparison
                break;
            }
    }
    if (is_negative)    // comparison of two negative value
        flag = -flag;
    return flag;
}

/*
 * Implementation notes: equals
 * ----------------------------
 * Judges whether two BigInteger is equal.
 */
bool BigInteger::equals(const BigInteger & val) const {
    return (is_negative==val.is_negative) && (data==val.data);
}

/*
 * Implementation notes: toString
 * ------------------------------
 * Return HEXADECIMAL string according to BigInteger value.
 */
string BigInteger::toString() const {
    string ans;
    unsigned t = base_num;
    t <<= base_int-4;
    for (int i=data.size()-1; i>=0; --i) {
        unsigned temp = data[i];
       for (int j=0; j<base_char; ++j) {
           unsigned num = t&temp;
            num >>= base_int-4; //
            temp <<= 4;
            if (num < 10)
                 ans.push_back((char)('0'+num));
            else
                ans.push_back((char)('A'+num-10));
        }
    }
    while (ans.size()>0 && ans.at(0)=='0')
        ans = ans.substr(1);
    if (ans.empty())    // empty string mean 0
        ans.push_back('0');
    if (is_negative)    // if negative, add "-".
        ans = "-"+ans;
    return ans;
}

/*
 * Implementation notes: abs
 * -------------------------
 * Return absolute value of BigInteger.
 */
BigInteger BigInteger::abs() const {
    BigInteger ans;
    ans.data = data;
    return ans;
}

/*
 * Implementation notes: abs
 * -------------------------
 * Returns the lowest int bit of BigInteger.
 */
int BigInteger::intTransfer() {
    return data[0];
}

BigInteger operator + (const BigInteger & a, const BigInteger & b) {
    BigInteger t(a);
    return t.add(b);
}
BigInteger operator - (const BigInteger & a, const BigInteger & b) {
    BigInteger t(a);
    return t.subtract(b);
}
BigInteger operator * (const BigInteger & a, const BigInteger & b) {
    BigInteger t(a);
    return t.multiply(b);
}
BigInteger operator / (const BigInteger & a, const BigInteger & b) {
    BigInteger t(a);
    return t.divide(b);
}
BigInteger operator % (const BigInteger & a, const BigInteger & b) {
    BigInteger t(a);
    return t.remainder(b);
}
bool operator < (const BigInteger & a, const BigInteger & b) {
    return a.compareTo(b) == -1;
}
bool operator > (const BigInteger & a, const BigInteger & b) {
    return b < a;
}
bool operator == (const BigInteger & a, const BigInteger & b) {
    return a.equals(b);
}
bool operator <= (const BigInteger & a, const BigInteger & b) {
    return !(a > b);
}

bool operator >= (const BigInteger & a, const BigInteger & b) {
    return !(a < b);
}
bool operator != (const BigInteger & a, const BigInteger & b) {
    return !(a == b);
}
BigInteger operator + (const BigInteger & a, const long long & b) {
    return a+BigInteger(b);
}
BigInteger operator - (const BigInteger & a, const long long & b) {
    return a-BigInteger(b);
}
BigInteger operator * (const BigInteger & a, const long long & b) {
    return a*BigInteger(b);
}
BigInteger operator / (const BigInteger & a, const long long & b) {
    return a/BigInteger(b);
}
BigInteger operator % (const BigInteger & a, const long long & b) {
    return a%BigInteger(b);
}
bool operator < (const BigInteger & a, const long long& b) {
    return a < BigInteger(b);
}
bool operator > (const BigInteger & a, const long long & b) {
    return a > BigInteger(b);
}
bool operator == (const BigInteger & a, const long long & b) {
    return a == BigInteger(b);
}
bool operator <= (const BigInteger & a, const long long & b) {
    return a <= BigInteger(b);
}
bool operator >= (const BigInteger & a, const long long & b) {
    return a >= BigInteger(b);
}
bool operator != (const BigInteger & a, const long long & b) {
    return a != BigInteger(b);
}
ostream & operator << (ostream & out, const BigInteger & val) {
    out << val.toString();
    return out;
}
/*
 * Implementation notes: trim
 * --------------------------
 * Eliminates "0" before the BigInteger vector<unsigned> data.
 */
BigInteger BigInteger::trim() {
    int cnt = 0;
    for (vector<unsigned>::const_reverse_iterator it=data.rbegin(); it!=data.rend(); ++it) {
        if((*it) == 0)  ++cnt;
        else    break;
    }
    if(cnt == (int)data.size())
        --cnt;
    BigInteger ans(*this);
    for(int i=0; i<cnt; ++i)
        ans.data.pop_back();
    return ans;
}

/*
 * Implementation notes: hexToNum
 * ------------------------------
 * Returns corresponding value according to HEXADECIMAL char.
 */
int BigInteger::hexToNum(char ch) {
    int ans = 0;
    if (isdigit(ch))
        ans = ch-'0';
    else if (islower(ch))
        ans = ch-'a'+10;
    else
        ans = ch-'A'+10;
    return ans;
}

/*
 * Implementation notes: bit
 * -------------------------
 * Constructor of private bit class.
 */
BigInteger::bit::bit(const BigInteger & val) {
    bit_vector = val.data;
    unsigned temp = bit_vector[bit_vector.size()-1];
    length = bit_vector.size()<<base_bit;
    unsigned t = 1<<(base_int-1);

    if (temp == 0)
        length -= base_int;
    else {
        while (!(temp & t)) {
            --length;
            t >>= 1;
        }
    }
}

/*
 * Implementation notes: at
 * ------------------------
 * Judge whether the id-th bit is 1.
 */
bool BigInteger::bit::at(int id) {
    int index = id>>base_bit;
    int shift = id&base_temp;
    unsigned t = bit_vector[index];
    return (t & (1<<shift));
}
