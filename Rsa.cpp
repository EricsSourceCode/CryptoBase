// Copyright Eric Chauvin 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "Rsa.h"
#include "Euclid.h"
#include "../CppInt/Division.h"
#include "../CppBase/StIO.h"
#include "../Crt/Garner.h"
#include "../Crt/GarnerCrt.h"



bool Rsa::makeKeys( const Integer& prime1,
                    const Integer& prime2,
                    IntegerMath& intMath,
                    Mod& mod,
                    SPrimes& sPrimes,
                    FindFacSm& findFacSm,
                    // FindFacQr& findFacQr,
                    const MultInv& multInv,
                    // const QuadRes& quadRes,
                    const CrtMath& crtMath,
                    GarnerCrt& garnerCrt )
{
const Int32 PubKeyExponentL = 65537;

pubKeyExponent.setFromLong48( PubKeyExponentL );

primeP.copy( prime1 );
primeQ.copy( prime2 );

CharBuf showBuf;

StIO::putS( "primeP:" );
intMath.toString10( primeP, showBuf );
StIO::putCharBuf( showBuf );
StIO::putLF();

StIO::putS( "primeQ:" );
intMath.toString10( primeQ, showBuf );
StIO::putCharBuf( showBuf );
StIO::putLF();


if( !isGoodPair( intMath ))
  {
  StIO::putS( "isGoodPair false." );
  return false;
  }

primePMinus1.copy( primeP );

primePMinus1.decrement();
primeQMinus1.copy( primeQ );
primeQMinus1.decrement();

phiN.copy( primePMinus1 );
intMath.multiply( phiN, primeQMinus1 );

pubKeyN.copy( primeP );
intMath.multiply( pubKeyN, primeQ );

if( !setPrivateKeys( intMath ))
  throw "!setPrivateKeys().";
  // return false;

if( !testEncryption( mod, intMath,
                     sPrimes,
                     multInv,
                     crtMath ))
  throw "!testEncryption().\n";
  // return false;

// Does it have any small prime factors
// in it?
Int64 testFac = findFacSm.findIt( pubKeyN,
                                  10000,
                                  sPrimes,
                                  intMath );

if( testFac != 0 )
  {
  StIO::putS( "FindFacSm found testFac: " );
  StIO::printFD( testFac );
  StIO::putLF();
  }

// Integer prime1Result;
// Integer prime2Result;

// if( findFacQr.findIt( pubKeyN,
//                      prime1Result,
//                      prime2Result,
//                      intMath,
//                      sPrimes,
//                      multInv,
//                      quadRes ))
//  {
//  StIO::putS( "Found it. FindFacQr" );
//  return true;
//  }

Crt crt;
crt.setFromInteger( pubKeyN, intMath, sPrimes );
Integer garnerTest;
garnerCrt.toInteger( crt, garnerTest, intMath );
if( !garnerTest.isEqual( pubKeyN ))
  throw "GarnerCrt.toInteger() test was bad.";

Crt3 crt3;
crt3.setFromInteger( pubKeyN, intMath,
                     sPrimes, multInv,
                     crtMath );

Integer testCrt3;
crt3.toInteger( crtMath, testCrt3 );
if( !testCrt3.isEqual( pubKeyN ))
  throw "Crt3.toInteger() test was bad.";


StIO::putS( "Finished makeKeys." );


return true;
}



bool Rsa::isGoodPair( IntegerMath& intMath )
{
// This is extremely unlikely since they are
// supposed to be primes, and have passed some
// primality tests.
Integer gcd;
Euclid::greatestComDiv( primeP, primeQ,
                                 gcd, intMath );
if( !gcd.isOne())
  {
  // mainIO.appendChars( "Bad pair had a GCD.\n" );
  return false;
  }

Euclid::greatestComDiv( primeP, pubKeyExponent,
                              gcd, intMath );
if( !gcd.isOne())
  {
  // mainIO.appendChars(
     //  "primeP had a GCD with pubKeyExponent.\n" );
  return false;
  }

Euclid::greatestComDiv( primeQ, pubKeyExponent,
                                 gcd, intMath );
if( !gcd.isOne())
  {
  // mainIO.appendChars(
   // "primeQ had a GCD with pubKeyExponent.\n" );
  return false;
  }

// And other tests...

return true;
}




bool Rsa::setPrivateKeys( IntegerMath& intMath )
{
// In RFC 8017 there are commonly used
// letters/symbols to represent
// the numbers used.  So the number e
// is the public exponent.
// The number e that is used here is
// called pPubKeyExponentUint = 65537.
// In the RFC the private key d is the
// multiplicative inverse of
// e mod PhiN.  Which is mod
// (P - 1)(Q - 1).  It's called
// privKInverseExponent here.

Integer gcd;
if( !Euclid::multInverse( pubKeyExponent,
                          phiN,
                          privKInverseExponent,
                          gcd,
                          intMath ))
  {
  StIO::putS( "Rsa multInverse was false. (1)" );
  return false;
  }

// In RFC 8017 it defines a number dP
// which is the multiplicative
// inverse, mod (P - 1) of e.  That dP
// is named PrivKInverseExponentDP here.

if( !Euclid::multInverse( pubKeyExponent,
                          primePMinus1,
                          privKInverseExponentDP,
                          gcd,
                          intMath ))
  {
  StIO::putS( "Rsa multInverse was false. (2)" );
  return false;
  }

Integer quotient;
Integer remainder;
Integer test1;

test1.copy( privKInverseExponent );
Division::divide( test1, primePMinus1,
                  quotient, remainder, intMath );
test1.copy( remainder );
if( !test1.isEqual( privKInverseExponentDP ))
  throw "Rsa Bad privKInverseExponentDP.";

// In RFC 8017 it defines a number dQ
// which is the multiplicative
//      // inverse, mod (Q - 1) of e.  That dQ
// is named privKInverseExponentDQ here.

if( !Euclid::multInverse( pubKeyExponent,
                          primeQMinus1,
                          privKInverseExponentDQ,
                          gcd,
                          intMath ))
  {
  StIO::putS( "Rsa multInverse was false. (3)" );
  }

test1.copy( privKInverseExponent );
Division::divide( test1, primeQMinus1,
                  quotient, remainder, intMath );
test1.copy( remainder );
if( !test1.isEqual( privKInverseExponentDQ ))
  throw "Bad privKInverseExponentDQ.\n";

return true;
}




bool Rsa::testEncryption( Mod& mod,
                     IntegerMath& intMath,
                     const SPrimes& sPrimes,
                     const MultInv& multInv,
                     const CrtMath& crtMath )
{
StIO::putS( "testEncryption top." );

Integer gcd;

// This does not include the standard padding
// and all that.

// Make a random number to test
// encryption/decryption.

Integer toEncrypt;

const Int32 testIndex = primeP.getIndex() - 2;
if( testIndex < 0 )
  {
  StIO::putS( "testIndex < 0." );
  return true;
  }

if( !toEncrypt.makeRandomOdd( testIndex ))
  throw "Error making toEncrypt random number.";

Integer plainTextNumber;
plainTextNumber.copy( toEncrypt );

mod.toPower( toEncrypt, pubKeyExponent,
                        pubKeyN, intMath );

Integer cipherTextNumber;
cipherTextNumber.copy( toEncrypt );

mod.toPower( toEncrypt,
                     privKInverseExponent,
                     pubKeyN, intMath );

if( !plainTextNumber.isEqual( toEncrypt ))
  throw "PlainTextNumber not equal.\n";

// Test the standard optimized way of
// decrypting:
if( !toEncrypt.makeRandomOdd( testIndex ))
  throw "Error making toEncrypt random number.";

plainTextNumber.copy( toEncrypt );
mod.toPower( toEncrypt, pubKeyExponent,
                      pubKeyN, intMath );

cipherTextNumber.copy( toEncrypt );

Integer plainResult;

Garner garner;
bool result = garner.decryptWithQInverse(
              primeP, primeQ,
              privKInverseExponentDP,
              privKInverseExponentDQ,
              cipherTextNumber,
              plainResult, mod, intMath );

if( !plainResult.isEqual( plainTextNumber ))
  throw "Garner test 1 was not right.";

Integer plainResultF;

result = garner.formula(
                primeP, primeQ,
                privKInverseExponentDP,
                privKInverseExponentDQ,
                cipherTextNumber,
                plainResultF, mod, intMath );

if( !plainResultF.isEqual( plainTextNumber ))
  throw "Garner test 2 was not right.";

garner.testCrt( pubKeyN,
                intMath,
                sPrimes,
                multInv,
                crtMath );

return result;
}


/*
Old version.
bool Rsa::decryptWithQInverse(
             const Integer& encryptedNumber,
             Integer& decryptedNumber,
             Mod& mod,
             IntegerMath& intMath )
{

// See RFC 8017.

// ========

// Garner's Formula.
//      2.2 Let m_1 = c^dP mod p.
//      2.3 Let m_2 = c^dQ mod q.
//      2.4 Let h = qInv ( m_1 - m_2 ) mod p.
//      2.5 Let m = m_2 + hq.

Integer testForDecrypt;
testForDecrypt.copy( encryptedNumber );
mod.toPower( testForDecrypt,
                    privKInverseExponentDP,
                    primeP, intMath );

Integer m1ForInverse;
Integer m2ForInverse;

m1ForInverse.copy( testForDecrypt );
//      2.3 Let m_2 = c^dQ mod q.
testForDecrypt.copy( encryptedNumber );

mod.toPower( testForDecrypt,
                    privKInverseExponentDQ,
                    primeQ, intMath );

m2ForInverse.copy( testForDecrypt );
//      2.4 Let h = qInv ( m_1 - m_2 ) mod p.

// =======
// Fix up this stuff.
// ======

// Don't do this obviously.
// How many is optimal to avoid the division?
const Int32 howManyIsOptimal =
                          primeP.getIndex() * 3;
for( Int32 count = 0; count < howManyIsOptimal;
                                      count++ )
  {
  if( m1ForInverse.paramIsGreater(
                             m2ForInverse ))
    m1ForInverse.add( primeP );
  else
    break;
  }

Integer m1M2SizeDiff;
Integer quotient;
Integer remainder;

if( m1ForInverse.paramIsGreater( m2ForInverse ))
  {
  m1M2SizeDiff.copy( m2ForInverse );
  intMath.subtract( m1M2SizeDiff, m1ForInverse );
  // Unfortunately this long Divide()
  // has to be done.
  Division::divide( m1M2SizeDiff, primeP,
                 quotient, remainder, intMath );

  quotient.increment();
  intMath.multiply( quotient, primeP );
  m1ForInverse.add( quotient );
  }

Integer m1MinusM2;

m1MinusM2.copy( m1ForInverse );
intMath.subtract( m1MinusM2, m2ForInverse );
if( m1MinusM2.getNegative() )
  throw "m1MinusM2 is negative.";

if( qInv.getNegative() )
  throw "qInv is negative.";

Integer hForQInv;
hForQInv.copy( m1MinusM2 );
intMath.multiply( hForQInv, qInv );
if( hForQInv.getNegative() )
  throw "hForQInv is negative.";

if( primeP.paramIsGreater( hForQInv ))
  {
  Division::divide( hForQInv, primeP,
                 quotient, remainder, intMath );

  hForQInv.copy( remainder );
  }

//      2.5 Let m = m_2 + hq.
decryptedNumber.copy( hForQInv );
intMath.multiply( decryptedNumber, primeQ );
decryptedNumber.add( m2ForInverse );

if( !testForDecrypt.isEqual(
                           decryptedNumber ))
  throw "!testDecrypt.IsEqual()";

return true;
}
*/
