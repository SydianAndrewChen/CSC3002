/*
 * File: MyServer.cpp
 * ------------------
 * This file implements the RSAEncipherment.h interface.
 */

#include <ostream>
#include <cstring>
#include <sstream>
#include <cassert>
#include <string>

#include "Tools.h"
#include "RSAEncipherment.h"

void RSA::init(const unsigned len) {
    srand((unsigned)time(NULL));
    p = createPrime(len, 15);
    q = createPrime(len, 15);
    n = p*q;
    eul = (p-1)*(q-1);
    createExponent(eul);
}

BigInteger RSA::encryptByPublic(const BigInteger & m) {
    return m.modPow(e, n);
}

BigInteger RSA::decryptByPrivate(const BigInteger & c) {
    return c.modPow(d, n);
}

BigInteger RSA::encryptByPrivate(const BigInteger & m) {
    return decryptByPrivate(m);
}

BigInteger RSA::decryptByPublic(const BigInteger & c) {
    return encryptByPublic(c);
}

ostream & operator << (ostream & out, const RSA & rsa) {
    out << "n: " << rsa.n << "\n";
    out << "p: " << rsa.p << "\n";
    out << "q: " << rsa.q << "\n";
    out << "e: " << rsa.e << "\n";
    out << "d: " << rsa.d;
    return out;
}

BigInteger RSA::createOddNum(unsigned len) {
    static const char hex_table[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                                    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    len >>= 2;
    if (len) {
        ostringstream oss;
        for (int i=0; i<(int)len-1; ++i)
            oss << hex_table[rand()%16];
        oss << hex_table[1];
        return BigInteger(oss.str());
    }
    return BigInteger("F");
}

bool RSA::isPrime(const BigInteger & num, const unsigned k) {
    assert(num != BigInteger::ZERO);
    if (num == BigInteger::ONE)
        return false;
    if (num == BigInteger::TWO)
        return true;

    BigInteger t = num-1;
    BigInteger::bit b(t);
    if (b.at(0) == 1)
        return false;
    int s = 0;
    BigInteger d(t);
    for (int i=0; i<(int)b.size(); ++i) {
        if (!b.at(i)) {
            ++s;
            d = d.shiftRight(1);
        }
        else
            break;
    }

    for (int i=0; i<(int)k; ++i) {
        BigInteger a = createRandomSmaller(num);
        BigInteger x = a.modPow(d, num);
        if (x == BigInteger::ONE)
            continue;
        bool ok = true;
        for (int j=0; j<s && ok; ++j) {
            if (x == t)
                ok = false;
            x = x.multiply(x).mod(num);
        }
        if (ok)
            return false;
    }
    return true;
}


BigInteger RSA::createRandomSmaller(const BigInteger & val) {
    unsigned t = 0;
    do {
        t = rand();
    } while (t == 0);

    BigInteger mod(t);
    BigInteger ans = mod%val;
    if (ans == BigInteger::ZERO)
        ans = val-BigInteger::ONE;
    return ans;
}

BigInteger RSA::createPrime(unsigned len, const unsigned k) {
    assert(k > 0);
    BigInteger ans = createOddNum(len);
    while (!isPrime(ans, k)) {
        ans = ans.add(BigInteger::TWO);
    }
    return ans;
}

void RSA::createExponent(const BigInteger & eul) {
    e = 65537;
    d = e.modInverse(eul);
}

/*
void EncryptDecrypt::menu() {
    cout << "*******Welcome to use RSA encoder********" << endl;
    cout << "               e: encrypt                " << endl;
    cout << "               d: decrypt                " << endl;
    cout << "               p: print                  " << endl;
    cout << "               r: reset                  " << endl;
    cout << "               q: quit                   " << endl;
    cout << "input your choice:" << :endl;
}
*/

BigInteger EncryptDecrypt::encrypt(char ch) {
    BigInteger m((int)ch);
    BigInteger c = rsa.encryptByPublic(m);
    return c;
}

char EncryptDecrypt::decrypt(BigInteger bign) {

    BigInteger m = rsa.decryptByPrivate(bign);
    return (char)m.intTransfer();
}

/*
void EncryptDecrypt::print() {
    cout << rsa << endl;
}
*/

/*
void EncryptDecrypt::reset() {
    cout << "Input cipher length: ";
    int len;
    cin >> len;
    load(len>>1);
}
*/

/*
void EncryptDecrypt::load(int len) {
    cout << "Initializing..." << endl;
    clock_t start = clock();
    rsa.init(len);
    clock_t finish = clock();
    cout << "Initialization Done." << endl;
    cout << fixed;
    cout.precision(3);
    cout << "Time: " << (double)(finish-start)/CLOCKS_PER_SEC << "s." << endl;
}
*/

/*
bool EncryptDecrypt::islegal(const string & str) {
    for(string::const_iterator it=str.begin(); it!=str.end(); ++it) {
        if (!isalnum(*it))
            return false;
        if (isalpha(*it)) {
            char ch = tolower(*it);
            if (ch > 'f')
               return false;
        }
    }
    return true;
}
*/
