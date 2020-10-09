/*
 * File: Tools.h
 * -------------
 * This interface exports necessary tools.
 */

#ifndef TOOLS_H
#define TOOLS_H

#include <algorithm>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ostream>
#include <cstdio>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

/*
* Type: convert
* Usage: s = convert(n);
* ----------------------
* This function is used to convert integer value n to string representation format.
*/
string convert(int n);

/*
* Type: intConvert
* Usage: ans = intConvert(s);
* ---------------------------
* This function is used to convert number string s to its int value.
*/
int intConvert(const string &s);

/*
* Type: floatConvert
* Usage: ans = floatConvert(s);
* -----------------------------
* This function is used to convert number string s to its float value.
*/
float floatConvert(const string &s);

/*
* Type: getLocalTime
* Usage: y = getLocalTime("year");
* --------------------------------
* This function is used to get local year time of the system.
*/
int getLocalTime(const string &s);

/*
 * This class is mainly built for RSA encryption calculation. Since unsigned int can only
 * support [0, 2^32) while unsigned long long can only support [0, 2^64) and reliable private
 * keys and public keys require integer around 2^1024. So this BigInterger is essential.
 */
class BigInteger {
public:
    /*
     * Constructor: BigIntegegr
     * Usage: BigInteger n = BigInteger();
     * -----------------------------------
     * Initializes a new BigInteger whose value is 0;
     */
    BigInteger(): is_negative(false) { data.push_back(0); }

    /*
     * Constructor: BigIntegegr
     * Usage: BigInteger n = BigInteger(val);
     * --------------------------------------
     * Initializes a new BigInteger according to the previous BigInteger variable val;
     */
    BigInteger(const BigInteger &val);

    /*
     * Constructor: BigIntegegr
     * Usage: BigInteger n = BigInteger(str);
     * --------------------------------------
     * Initializes a new BigInteger according to the previous HEXADECIMAL value string;
     */
    BigInteger(const string &str);

    /*
     * Constructor: BigIntegegr
     * Usage: BigInteger n = BigInteger(num);
     * --------------------------------------
     * Initializes a new BigInteger according to the previous long long variable num;
     */
    BigInteger(const long long &num);

    /*
     * Destructor: ~BigInteger();
     * --------------------------
     * Frees any heap storage associated with this BigInteger.
     */
    ~BigInteger() {}

    /*
     * Method: add
     * Usage: bign1 = bign1.add(bign2);
     * --------------------------------
     * Adds two large integers.
     */
    BigInteger add(const BigInteger &val);

    /*
     * Method: subtract
     * Usage: bign1 = bign1.subtract(bign2);
     * -------------------------------------
     * Subtracts two large integers.
     */
    BigInteger subtract(const BigInteger &val);

    /*
     * Method: multiply
     * Usage: bign1 = bign1.multiply(bign2);
     * -------------------------------------
     * Multiplies two large integers.
     */
    BigInteger multiply(const BigInteger &val) const;

    /*
     * Method: divied
     * Usage: bign1 = bign1.divide(bign2);
     * -----------------------------------
     * Divides two large integers and returns quotient.
     */
    BigInteger divide(const BigInteger &val);

    /*
     * Method: remainder
     * Usage: bign1 = bign1.remainder(bign2);
     * --------------------------------------
     * Divides two large integers and returns remainder.
     */
    BigInteger remainder(const BigInteger &val);

    /*
     * Method: mod
     * Usage: bign1 = bign1.divide(bign2);
     * -----------------------------------
     * Gets the remainder of bign1%bign2;
     */
    BigInteger mod(const BigInteger &m);

    /*
     * Method:
     * Usage: bign1 = bign1.divideAndRemainder(value, m);
     * --------------------------------------------------
     * Realizes division operation. Inputs divisor as "val".
     * Returns the quotient as the result of this function and
     * returns the remainder as the reference variable "m".
     */
    BigInteger divideAndRemainder(const BigInteger &val, BigInteger &m);

    /*
     * Method: pow
     * Usage: bign1 = bign1.pow(bign2);
     * --------------------------------
     * Calculates pow(bign1, bign2) by fast exponentiation method.
     */
    BigInteger pow(const BigInteger &exponent);

    /*
     * Method: modPow
     * Usage: bign1 = bign1.modPow(bign2, m);
     * --------------------------------------
     * Calculates pow(bign1, bign2) in %m condition by fast exponentiation method.
     */
    BigInteger modPow(const BigInteger &exponent, const BigInteger &m) const;

    /*
     * Method: modInverse
     * Usage: bign1 = bign1.modInverse(m);
     * -----------------------------------
     * Calculates the inverse value (%m) in the sense of number theory using the extension of Euclidean algorithm.
     */
    BigInteger modInverse(const BigInteger &m);

    /*
     * Method: shiftLeft
     * Usage: bign1 = bign1.shiftLeft(len);
     * ------------------------------------
     * Realizes logic left shift.
     */
    BigInteger shiftLeft(const unsigned len);

    /*
     * Method: shiftRight
     * Usage: bign1 = bign1.shiftRight(len);
     * -------------------------------------
     * Realizes logic right shift.
     */
    BigInteger shiftRight(const unsigned len);

    /*
     * Method: compareTo
     * Usage: bign1 = bign1.compareTo(bign2);
     * --------------------------------------
     * Compares values of bign1 and bign2.
     * Returns -1 <=> bign1<bign2.
     * Returns 0 <=> bign1==bign2.
     * Returns 1  <=> bign1>bign2
     */
    int compareTo(const BigInteger &val) const;

    /*
     * Method: equals
     * Usage: bign1 = bign1.equals(bign2);
     * -----------------------------------
     * Returns whether bign1 == bign2.
     */
    bool equals(const BigInteger &val) const;

    /*
     * Method: toString
     * Usage: bign1 = bign1.toString(num);
     * -----------------------------------
     * Converts BigInteger value into its HEXADECIMAL value string.
     */
    string toString() const;

    /*
     * Method: abs
     * Usage: bign1 = bign1.abs();
     * ---------------------------
     * Returns absolute value of bign1.
     */
    BigInteger abs() const;

    /*
     * Method: intTransfer
     * Usage: n = bign1.intTransfer();
     * -------------------------------
     * Returns the lowest int bit of BigInteger.
     */
    int intTransfer();

protected:
    /*
     * Operators: +, -, *, /, %
     * Usage: bign1 + bign2 ...
     * ------------------------
     * Relational operators between two BigInteger value.
     */
    friend BigInteger operator + (const BigInteger &a, const BigInteger &b);
    friend BigInteger operator - (const BigInteger &a, const BigInteger &b);
    friend BigInteger operator * (const BigInteger &a, const BigInteger &b);
    friend BigInteger operator / (const BigInteger &a, const BigInteger &b);
    friend BigInteger operator % (const BigInteger &a, const BigInteger &b);

    /*
     * Operators: <, <=, >, >=, ==, !=
     * Usage: if (bign1 < bign2) ...
     * -----------------------------
     * Relational operators to compare two BigInteger value.
     */
    friend bool operator < (const BigInteger &a, const BigInteger &b);
    friend bool operator > (const BigInteger &a, const BigInteger &b);
    friend bool operator == (const BigInteger &a, const BigInteger &b);
    friend bool operator <= (const BigInteger &a, const BigInteger &b);
    friend bool operator >= (const BigInteger &a, const BigInteger &b);
    friend bool operator != (const BigInteger &a, const BigInteger &b);

    /*
     * Operators: +, -, *, /, %
     * Usage: bign + longInt ...
     * -------------------------
     * Relational operators between a BigInteger value and a long long value.
     */
    friend BigInteger operator + (const BigInteger &a, const long long &b);
    friend BigInteger operator - (const BigInteger &a, const long long &b);
    friend BigInteger operator * (const BigInteger &a, const long long &b);
    friend BigInteger operator / (const BigInteger &a, const long long &b);
    friend BigInteger operator % (const BigInteger &a, const long long &b);

    /*
     * Operators: <, <=, >, >=, ==, !=
     * Usage: if (bign < longInt) ...
     * -------------------------------
     * Relational operators to compare a BigInteger value and a long long value.
     */
    friend bool operator < (const BigInteger &a, const long long &b);
    friend bool operator > (const BigInteger &a, const long long &b);
    friend bool operator == (const BigInteger &a, const long long &b);
    friend bool operator <= (const BigInteger &a, const long long &b);
    friend bool operator >= (const BigInteger &a, const long long &b);
    friend bool operator != (const BigInteger &a, const long long &b);

    /*
     * Operators: <<
     * Usage: cout << bign;
     * --------------------
     * Relational operators of stream output.
     */
    friend ostream & operator << (ostream &out, const BigInteger &val);

    /*
     * Operators: =
     * Usage: bign1 = str;
     * --------------------
     * Relational operators of assignment from a HEXADECIMAL string to BigInteger value.
     */
    BigInteger operator = (const string & str) {return (*this) = BigInteger(str);}

    /*
     * Operators: =
     * Usage: bign1 = num;
     * --------------------
     * Relational operators of assignment from long long variable to BigInteger value.
     */
    BigInteger operator = (const long long & num) {return (*this) = BigInteger(num);}

private:
    /*
     * Method: trim
     * Usage: bign1 = bign1.trim();
     * ----------------------------
     * Eliminates "0" before the BigInteger vector<unsigned> data.
     */
    BigInteger trim();

    /*
     * Method: hexToNum
     * Usage: n = hexToNum(c);
     * -----------------------
     * Returns corresponding value according to HEXADECIMAL char.
     */
    int hexToNum(char ch);

public:
    /*
     * Constant values.
     */
    static const int base_bit = 5;
    static const int base_char = 8;
    static const int base_int = 32;
    static const int base_num = 0xffffffff;
    static const int base_temp = 0x1f;
    static const BigInteger ZERO;
    static const BigInteger ONE;
    static const BigInteger TWO;
    static const BigInteger TEN;

private:
    bool is_negative;
    vector<unsigned> data;
    class bit {
    public:
        // Constructor
        bit(const BigInteger &);
        // Return binary value length of this BigInteger value.
        int size() { return length; }
        // judge whether the ith bit is 1.
        bool at(int id);
    private:
        vector<unsigned> bit_vector;
        int length;
    };
    // Declare RSA class as a friend class
    friend class RSA;
};

#endif // TOOLS_H
