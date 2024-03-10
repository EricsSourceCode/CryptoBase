/*

// Copyright Eric Chauvin 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "ECurve.h"
#include "TonelliShanks.h"

#include "../CppBase/StIO.h"
#include "../CppBase/TimeApi.h"



bool ECurve::isOnCurve( const EPoint& aPoint,
                        const Integer& prime,
                        Mod& mod,
                        IntegerMath& intMath )
{
// Check if this gets the right modular
// square root for y.
// Integer rootTonelli;
// bool TonelliOK = TonelliShanks::modRoot( right,
//                        rootTonelli,
//                        modulus, intMath, mod );
//  if( !TonelliOK )
//    continue; // Square root doesn't exist.


if( aPoint.getInfin() )
  return true;

// The curve E: y^2 = x^3 + ax + b

// If the prime is 71 and x = 4 then that
// is 64 + 7 = 71
// so y is zero.

Integer left;
aPoint.getY( left );

mod.square( left, prime, intMath );

Integer X;
aPoint.getX( X );

Integer right;
aPoint.getX( right );
mod.square( right, prime, intMath );
mod.multiply( right, X, prime, intMath );

// Now right is x cubed.

Integer ax;
ax.copy( coefA );
mod.multiply( ax, X, prime, intMath );
mod.add( right, ax, prime, intMath );

mod.add( right, coefB, prime, intMath );

if( left.isEqual( right ))
  return true;

return false;
}




bool ECurve::findFactors( const Integer& N,
                          Integer& gcd,
                          IntegerMath& intMath,
                          Mod& mod )
{
// The Elliptic Curve Factoring Method ECM.
// This is like the Pollard factoring
// algorithm in Pollard.cpp.  There are better,
// more optimized versions of this algorithm.

StIO::putS( "Ellip Factoring start." );

gcd.setToOne();

Int32 setToIndex = N.getIndex();
if( setToIndex > 0 )
  setToIndex--;

// Make a random coefficient A for a random
// curve.
for( Int32 count = 0; count < 10000; count++ )
  {
  if( coefA.makeRandomOdd( setToIndex ))
    break;

  }

Integer xRand;
Integer yRand;

for( Int32 count = 0; count < 10000; count++ )
  {
  if( xRand.makeRandomOdd( setToIndex ))
    break;

  }

for( Int32 count = 0; count < 10000; count++ )
  {
  if( yRand.makeRandomOdd( setToIndex ))
    break;

  }

// Make sure they are mod N.  Less than N.
mod.makeExact( coefA, N, intMath );
mod.makeExact( xRand, N, intMath );
mod.makeExact( yRand, N, intMath );


// Make a coefficient B so that the random
// point is on the curve.

coefB.copy( yRand );
mod.square( coefB, N, intMath );

Integer xCubed;
xCubed.copy( xRand );
mod.square( xCubed, N, intMath );
mod.multiply( xCubed, xRand, N, intMath );

Integer aX;
aX.copy( xRand );
mod.multiply( aX, coefA, N, intMath );

mod.subtract( coefB, xCubed, N, intMath );
mod.subtract( coefB, aX, N, intMath );

// Test:
// 4a^3 + 27b^2 not zero.  Non-singular.

// Make the initial random point.
EPoint startPoint( xRand, yRand );

if( !isOnCurve( startPoint, N, mod, intMath ))
  throw "startPoint is not on the curve.";

Integer I;
CharBuf showBuf;

for( Int32 i = 2; i < 100000; i++ )
  {
  I.setFromLong48( i );

  // If it can't find the multiplicative inverse.
  if( !scalarMult( startPoint, I, N, gcd,
                   intMath, mod ))
    {
    StIO::putS( "One factor is: " );
    intMath.toString10( gcd, showBuf );
    StIO::putCharBuf( showBuf );
    StIO::putLF();
    return true;
    }

  // if( !isOnCurve( startPoint, N, mod, intMath ))
  }

StIO::putS( "Did not find factor." );
StIO::putS( "End of Ellip Factoring." );

return false;
}



bool ECurve::scalarMult( EPoint& result,
                         const Integer& k,
                         const Integer& modulus,
                         Integer& gcd,
                         IntegerMath& intMath,
                         Mod& mod )

{
// If k = 23
// 23 = 16 + 7 = 0b10111.
// 23 = 2^4 + 2^2 + 2 + 1
// 23 times P = (2^4 + 2^2 + 2 + 1)P
// 23 times P = 2^4P + 2^2P + 2P + P

EPoint accumP;
accumP.setInfin( true ); // Additive Identiry.

EPoint tempD;
EPoint tempAdd;
EPoint toDouble;

toDouble.copy( result );

Integer kShift;
kShift.copy( k );

for( Int32 count = 0; count < 100000; count++ )
  {
  if( (kShift.getD( 0 ) & 1) != 0 )
    {
    if( !tempAdd.add( accumP, toDouble, coefA, coefB,
                 modulus, gcd, mod, intMath ))
      return false;

    accumP.copy( tempAdd );
    }

  if( !tempD.doubleP( toDouble, coefA,
                      modulus, gcd, mod, intMath ))
    return false;

  toDouble.copy( tempD );
  kShift.shiftRight( 1 );
  if( kShift.isZero())
    break;

  }

result.copy( accumP );
return true;
}



void ECurve::testLoopAdd( EPoint& result,
                         const Integer& k,
                         const Integer& modulus,
                         Integer& gcd,
                         IntegerMath& intMath,
                         Mod& mod )

{
EPoint accumP;
accumP.setInfin( true ); // Additive Identiry.

EPoint tempAdd;
EPoint addConst;
addConst.copy( result );

const Int32 loops = k.getD( 0 ) & 0xFFFFFF;

for( Int32 count = 0; count < loops; count++ )
  {
  tempAdd.add( accumP, addConst, coefA, coefB,
               modulus, gcd, mod, intMath );
  accumP.copy( tempAdd );
  }

result.copy( accumP );
}



void ECurve::testMult( const Integer& N,
                       Integer& gcd,
                       IntegerMath& intMath,
                       Mod& mod )
{
StIO::putS( "Test multiply." );

Int32 setToIndex = N.getIndex();
if( setToIndex > 0 )
  setToIndex--;

// Make a random coefficient A for a random
// curve.
for( Int32 count = 0; count < 10000; count++ )
  {
  if( coefA.makeRandomOdd( setToIndex ))
    break;

  }

mod.makeExact( coefA, N, intMath );

Integer xRand;
Integer yRand;

for( Int32 count = 0; count < 10000; count++ )
  {
  if( xRand.makeRandomOdd( setToIndex ))
    break;

  }

for( Int32 count = 0; count < 10000; count++ )
  {
  if( yRand.makeRandomOdd( setToIndex ))
    break;

  }

// Make sure they are mod N.  Less than N.
mod.makeExact( xRand, N, intMath );
mod.makeExact( yRand, N, intMath );

// Make a coefficient B so that the random
// point is on the curve.

coefB.copy( yRand );
mod.square( coefB, N, intMath );

Integer xCubed;
xCubed.copy( xRand );
mod.square( xCubed, N, intMath );
mod.multiply( xCubed, xRand, N, intMath );

Integer aX;
aX.copy( xRand );
mod.multiply( aX, coefA, N, intMath );

mod.subtract( coefB, xCubed, N, intMath );
mod.subtract( coefB, aX, N, intMath );

// Test:
// 4a^3 + 27b^2 not zero.  Non-singular.


EPoint test1Point( xRand, yRand );
EPoint test2Point( xRand, yRand );

if( !isOnCurve( test1Point, N, mod, intMath ))
  throw "test1Point is not on the curve.";

Integer k;
k.setFromLong48( 157 );

scalarMult( test1Point, k, N, gcd, intMath, mod );

testLoopAdd( test2Point, k, N, gcd,
                         intMath, mod );

if( !test1Point.isEqual( test2Point ))
  {
  StIO::putS( "Test multiply not right." );
  return;
  }

StIO::putS( "Test multiply is OK." );
}



//////
void ECurve::setupBases( const EPoint p )
{
EPoint toDouble;
toDouble.copy( p );

// If point becomes infinite, or if y becomes
// zero, then it stays infinite.

for( Int32 count = 0; count < arraySize; count++ )
  {
  temp.doubleP( toDouble ...
  toDouble.copy( temp );
  toDouble.doubleP( toDouble, modulus, mod, intMath );
  baseArray[count].copy( toDouble );
  }
}
////////////

*/
