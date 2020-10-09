/*
 * File: RSAEncipherment.h
 * -----------------------
 * This interface realizes Encryption.
 */

#ifndef RSAENCIPHERMENT_H
#define RSAENCIPHERMENT_H

#include <ostream>
#include <cstring>
#include <sstream>
#include <cassert>
#include <string>
#include "Tools.h"

using namespace std;

class RSA {
public:
    /*
     * Constructor: RSA
     * Usage: RSA n = RSA([len]);
     * --------------------------
     * Initializes a new RSA class (according to length).
     */
    RSA() {}
    RSA(const unsigned len) { init(len); }

    /*
     * Destructor: ~RSA();
     * -------------------
     * Frees any heap storage associated with this RSA.
     */
    ~RSA() {}

    /*
     * Method: init
     * Usage: init(len);
     * -----------------
     * Initialize this RSA class with exact length.
     */
    void init(const unsigned len);

    /*
     * Method: encryptByPublic
     * Usage: n = encryptByPublic(m);
     * ------------------------------
     * Encrypts informaiton by public keys.
     */
    BigInteger encryptByPublic(const BigInteger &m);

    /*
     * Method: decryptByPrivate
     * Usage: n = decryptByPrivate(c);
     * -------------------------------
     * Decrypts informaiton by private keys.
     */
    BigInteger decryptByPrivate(const BigInteger &c);

    /*
     * Method: encryptByPublic
     * Usage: n = encryptByPublic(m);
     * ------------------------------
     * Encrypts informaiton by private keys.
     */
    BigInteger encryptByPrivate(const BigInteger &m);

    /*
     * Method: decryptByPrivate
     * Usage: n = decryptByPrivate(c);
     * -------------------------------
     * Decrypts informaiton by public keys.
     */
    BigInteger decryptByPublic(const BigInteger &c);

protected:
    /*
     * Operators: <<
     * Usage: cout << bign;
     * --------------------
     * Relational operators of stream output.
     */
    friend ostream & operator << (ostream &out, const RSA &rsa);

private:
    /*
     * Method: createOddNum
     * Usage: n = createOddNum(len);
     * -----------------------------
     * Create a big odd integer number whose length is len.
     */
    BigInteger createOddNum(unsigned len);

    /*
     * Method: isPrime
     * Usage: if(isPrime(n, k))
     * ------------------------
     * Judge whether a big Integer is prime or not.
     */
    bool isPrime(const BigInteger &num, const unsigned k);

    /*
     * Method: createRandomSmaller
     * Usage: n = createRandomSmaller(val);
     * ------------------------------------
     * Create a smaller BigInteger randomly.
     */
    BigInteger createRandomSmaller(const BigInteger &val);

    /*
     * Method: createPrime
     * Usage: n = createPrime(n, k);
     * -----------------------------
     * Create a BigInteger prime number.
     */
    BigInteger createPrime(unsigned num, const unsigned k);

    /*
     * Method: createExponent
     * Usage: createExponent(euler);
     * -----------------------------
     * Create public and private keys according to euler function.
     */
    void createExponent(const BigInteger &euler);
public:
    // preprocessed public key
    BigInteger n = BigInteger("8D6F49AB877B7CD0A960AD3BDEF2A95D6587E0F9AC72F2B231AEF8B559FD7C31B26189D78D949566A2A0595ABF2C6802E0166876C09428FFB56C3FDF87ED3DF033C36A6F045D6A23BEFE2FBAAE549CFA5C02DA883EA2B44E1F0A4BAA7D25ADA4FB45391C2E5C89852491E0E843CBA89A9938F2556F46FC0F69847BE80509AB3"),
               e = BigInteger("10001");
private:
    // preprocessed private key
    BigInteger d = BigInteger("DAFC54B4A13132306210AE28E52A13D1E8425AB439AE40D866D4E380E7B4F3767F1B46B1D92515ADB45F51FA8FC5D1C5A1F1100DCE5F989B53EC2779929C622075EEF7109646A0BA34B38265443136E46F6A7A3B500DFCBDCF9DAB9B99D151C4A60F4780DCF4EF36330CDDEE22E485C9BEBCEECB15DF3301D6C853D7126FB1");
    // big prime integer p&q
    BigInteger p = BigInteger("1FF3DCEBE368EDFA0A69C92DD6BD74CADC8D24BEE33734F81A6C73BCAF02B29148233933BE3C72B10CCF1D4095C10B66EC6D82D0D9EF0FBD6BFA3F210082F76F"),
               q = BigInteger("46D281690B8460BEC3C07B33EC45479D24028F1A513F61F6BA430FD7BC0B1E92E6794A423DA3B148E930933646EE89E4148F4B7B2877D74CB92091EA23460EFD");
    // erler(n)
    BigInteger eul;
};

class EncryptDecrypt {
public:
    /*
     * Constructor: EncryptDecrypt
     * ---------------------------
     * Initializes a new EncryptDecrypt.
     */
    EncryptDecrypt() {}

    /*
     * Destructor: ~EncryptDecrypt();
     * ------------------------------
     * Frees any heap storage associated with this EncryptDecrypt.
     */
    ~EncryptDecrypt() {}

    /*
     * Method: menu
     * Usage: menu();
     * --------------
     * Show instruction list informaiton.
     */
    //void menu();

    /*
     * Method: encrypt
     * Usage: n = encrypt(ch);
     * -----------------------
     * Encrypt char value to BigInteger encryption information.
     */
    BigInteger encrypt(char ch);

    /*
     * Method: decrypt
     * Usage: ch = decrypt(n);
     * -----------------------
     * Decrypt BigInteger encryption information to char value.
     */
    char decrypt(BigInteger bign);

    /*
     * Method: print
     * Usage: print();
     * ---------------
     * Print out public and private keys information.
     */
    //void print();

    /*
     * Method: reset
     * Usage: reset();
     * ---------------
     * Reset public and private keys information.
     */
    //void reset();

protected:

    /*
     * Method: load
     * Usage: load(n);
     * ---------------
     * Load RSA class according to exact length.
     */
    void load(int len);

    /*
     * Method: islegal
     * Usage: if(islegal(str))
     * -----------------------
     * Check whether a string made of HEXADECIMAL string.
     */
    bool islegal(const string &str);

private:
    RSA rsa;
};

#endif // RSAENCIPHERMENT_H
