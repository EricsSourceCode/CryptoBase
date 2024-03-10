// Copyright Eric Chauvin 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#include "Fermat.h"
#include "../CppBase/StIO.h"


void Fermat::makeAPrime( Integer& result,
                     const Int32 setToIndex,
                     const SPrimes& sPrimes,
                     Mod& mod,
                     IntegerMath& intMath )
{
// StIO::putS( "Fermat makeAPrime()." );

// while( true )
for( Int32 count = 0; count < 10000; count++ )
  {
  if( !result.makeRandomOdd( setToIndex ))
    {
    // StIO::putS(
     // "Fermat: Error making random number." );
    continue;
    }

  // StIO::putS( "Before isDivisible." );

  Int32 testPrime = intMath.
                     isDivisibleBySmallPrime(
                     result, sPrimes );
  if( 0 != testPrime)
    {
    // StIO::printF( "isDivisible(): " );
    // StIO::printFD( testPrime );
    // StIO::putLF();

    continue; // It's divisible by testPrime.
    }

  // CharBuf showBuf;
  // StIO::putS( "After small primes:" );
  // intMath.toString10( result, showBuf );
  // StIO::putCharBuf( showBuf );
  // StIO::putLF();

  if( !isPrime( result, sPrimes, mod, intMath ))
    {
    StIO::printFD( count );
    StIO::printF( ") Did not pass Fermat test." );
    StIO::putLF();
    continue;
    }

  StIO::putS( "Got a probable prime." );
  return;
  }

// It should not have to try this many.
// This should not happen.
throw "Fermat.makeAPrime End of loop.";
}




bool Fermat::isPrime( Integer& toTest,
                      const SPrimes& sPrimes,
                      Mod& mod,
                      IntegerMath& intMath )
{
// StIO::putS( "Fermat isPrime()." );

// Use bigger primes for Fermat test because the
// modulus can't be too small.  And also it's
// more likely to be congruent to 1 with a very
// small modulus.  In other words it's a lot more
// likely to appear to be a prime when it isn't.
// This Fermat primality test is usually
// described as using random primes to test with,
// and you could do it that way too.
// An adversary will know which primes you use
// for testing this unless they are random.
// A common way of doing this is to use a
// multiple of several primes as the base, like
// 2 * 3 * 5 * 7 = 210.


// Halfway down the array.
Int32 startAt = SPrimes::primesArraySize >> 1;

for( Int32 count = startAt; count <
                   (100 + startAt); count++ )
  {
  if( !isPrimeForOneValue( toTest,
               sPrimes.getPrimeAt( count ),
               mod, intMath ))
    {
    // It is definitely not a prime.
    return false;
    }
  }

// It _might_ be a prime if it passed this test.
// Increasing how many loops it does, how many
// tests it does, increases the probability
// that it's a prime.

return true;
}



bool Fermat::isPrimeForOneValue(
                         Integer& toTest,
                         const Int64 base,
                         Mod& mod,
                         IntegerMath& intMath )
{
// StIO::putS( "Fermat isPrimeForOneValue()." );

// CharBuf showBuf;
// StIO::putS( "toTest:" );
// intMath.toString10( toTest, showBuf );
// StIO::putCharBuf( showBuf );
// StIO::putLF();

// Assume toTest is not a small number.  (Not
// the size of a small prime.)
// Normally it would be something like a 1024
// bit number or bigger, but I assume it's at
// least bigger than a 32 bit number.
// Assume this has already been checked to
// see if it's divisible by a small prime.
// A has to be coprime to P and it is here
// because toTest is not divisible by a small
// prime.

// Fermat's little theorem:
// If p is a prime,
// a^p - a is a multiple of p.

// a^p is congruent to a mod p.

// A ^ (P - 1) is congruent to 1 mod P if P
// is a prime.
// Or: A^P - A is congruent to A mod P.
// If you multiply A by itself P times then
// divide it by P, the remainder is A.
//  (A^P / P)
// 5^3 = 125.  125 - 5 = 120.  A multiple of 3.
// 2^7 = 128.  128 - 2 = 7 * 18 (a multiple of 7.)

Integer pMinus1;
Integer A;

pMinus1.copy( toTest );

pMinus1.decrement();  // Decrement by 1.

// StIO::putS( "After decrement:" );
// intMath.toString10( pMinus1, showBuf );
// StIO::putCharBuf( showBuf );
// StIO::putLF();

A.setFromLong48( base );

mod.toPower( A, pMinus1, toTest, intMath );

if( A.isOne())
  return true; // It _might_ be a prime.
else
  return false; // It is definitely a
                // composite number.

}
