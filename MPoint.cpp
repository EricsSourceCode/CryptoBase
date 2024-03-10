// Copyright Eric Chauvin 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "MPoint.h"



bool MPoint::isConjugate( const MPoint& p,
                          const Integer& prime,
                          Mod& mod,
                          IntegerMath& intMath )
                                        const
{
if( !X.isEqual( p.X ))
  return false;

if( Y.isEqual( p.Y ))
  throw "Not conjugate. It is equal.";

Integer temp;
temp.copy( p.Y );
mod.negate( temp, prime, intMath );
if( !temp.isEqual( Y ))
  throw "Can't happen. Point not on curve.";
//  return false;

return true;
}



// These equations come from the tutorial
// by Martin Kleppmann called Implementing
// Curve25519/X25519: A Tutorial on Elliptic
// Curve Cryptography.
// https://martin.kleppmann.com/papers/
//                            curve25519.pdf



bool MPoint::add( const MPoint& p1,
                  const MPoint& p2,
                  const Integer& prime,
                  Integer& gcd,
                  Mod& mod,
                  IntegerMath& intMath  )
{
if( p1.getInfin())
  {
  copy( p2 );
  return true;
  }

if( p2.getInfin())
  {
  copy( p1 );
  return true;
  }

if( p1.isEqual( p2 ))
  {
  return doubleP( p1, prime, gcd,
                  mod, intMath );
  }

if( p1.X.isEqual( p2.X ))
  {
  // From the above check, the points are not
  // equal. If it's the same x then the y values
  // can only be the negated values for
  // each other.

  // For testing.
  if( !p1.isConjugate( p2, prime, mod, intMath ))
    {
    throw "Can't happen. !p.isConjugate( q )";
    }

  setInfin( true );
  return true;
  }

Integer p1X;
Integer p1Y;
Integer p2X;
Integer p2Y;
p1X.copy( p1.X );
p1Y.copy( p1.Y );
p2X.copy( p2.X );
p2Y.copy( p2.Y );


// y^2 = x^3 + Ax^2 + x

// p2.Y - p1.Y
Integer numerator;
numerator.copy( p2Y );
mod.subtract( numerator, p1Y, prime, intMath );

// p2.X - p1.X
Integer denom;
denom.copy( p2X );
mod.subtract( denom, p1X, prime, intMath );

// The x values are not equal, so denom can't
// be zero, but it can be 1.
Integer slope;
if( denom.isMoreThanInt24( 1 ))
  {
  if( !mod.divide( slope, numerator, denom,
                   prime, gcd, intMath ))
    return false;

  }
else
  {
  slope.copy( numerator );
  }

Integer slopeSqr;
Integer slopeCubed;
slopeSqr.copy( slope );
mod.multiply( slopeSqr, slopeSqr,
              prime, intMath );

slopeCubed.copy( slopeSqr );
mod.multiply( slopeCubed, slope, prime, intMath );


// x = slopeSqr - a - p1.x - p2.x;
Integer tempX;
tempX.copy( slopeSqr );

Integer coefAInt;
coefAInt.setFromLong48( coefA );
mod.subtract( tempX, coefAInt, prime, intMath );
mod.subtract( tempX, p1X, prime, intMath );
mod.subtract( tempX, p2X, prime, intMath );
X.copy( tempX );


// Calc Y:
Integer twoP1X;
twoP1X.copy( p1X );

// This is not just a shift left.  It's mod.
Integer two;
two.setFromLong48( 2 );
mod.multiply( twoP1X, two, prime, intMath );

Integer numerLeft;
numerLeft.copy( twoP1X );
mod.add( numerLeft, p2X, prime, intMath );
mod.addL( numerLeft, coefA, prime, intMath );

Integer numerAll;
numerAll.copy( numerLeft );
mod.multiply( numerAll, numerator, prime, intMath );

Integer term1;
if( !mod.divide( term1, numerAll, denom,
                        prime, gcd, intMath ))
  {
  return false;
  }

Integer tempY;
tempY.copy( term1 );
mod.subtract( tempY, slopeCubed, prime, intMath );
mod.subtract( tempY, p1Y, prime, intMath );
Y.copy( tempY );

return true;
}




bool MPoint::doubleP( const MPoint& p,
                      const Integer& prime,
                      Integer& gcd,
                      Mod& mod,
                      IntegerMath& intMath )
{
Integer pX;
Integer pY;
pX.copy( p.X );
pY.copy( p.Y );

// Double a point:
// P + P = 2P

if( p.getInfin())
  {
  // Infinite + Infinite = Infinite
  copy( p );
  return true;
  }

if( pY.isZero() )
  {
  // For this Montgomery curve this could only
  // happen if x is zero.
  // Then the conjugate to Y would be
  // zero too.
  setInfin( true );
  return true;
  }

// Calc X:
Integer threeXSqr;
threeXSqr.copy( pX );
mod.multiply( threeXSqr, threeXSqr,
                         prime, intMath );
mod.multiplyL( threeXSqr, 3, prime, intMath );

Integer twoAx;
twoAx.copy( pX );
mod.multiplyL( twoAx, coefA * 2, prime, intMath );

Integer threeXTerm;
threeXTerm.copy( threeXSqr );
mod.add( threeXTerm, twoAx, prime, intMath );
mod.addL( threeXTerm, 1, prime, intMath );

Integer twoY;
twoY.copy( pY );
mod.multiplyL( twoY, 2, prime, intMath );

Integer term1;
if( !mod.divide( term1, threeXTerm, twoY,
                 prime, gcd, intMath ))
  return false;

mod.multiply( term1, term1, prime, intMath );

Integer twoX;
twoX.copy( pX );
mod.multiplyL( twoX, 2, prime, intMath );

Integer tempX;
tempX.copy( term1 );
Integer coefAInt;
coefAInt.setFromLong48( coefA );
mod.subtract( tempX, coefAInt, prime, intMath );
mod.subtract( tempX, twoX, prime, intMath );

X.copy( tempX );



// Calc Y:
// y^2 = x^3 + Ax^2 + x

// 2*px + px + a

Integer numerLeft;
numerLeft.copy( twoX );
mod.add( numerLeft, pX, prime, intMath );
mod.addL( numerLeft, coefA, prime, intMath );



Integer numerAll;
numerAll.copy( threeXTerm );
mod.multiply( numerAll, numerLeft, prime, intMath );

Integer termLeft;
if( !mod.divide( termLeft, numerAll, twoY,
                 prime, gcd, intMath ))
  return false;

Integer termMiddle;
if( !mod.divide( termMiddle, threeXTerm, twoY,
                 prime, gcd, intMath ))
  return false;


Integer middleCubed;
middleCubed.copy( termMiddle );
mod.multiply( middleCubed, termMiddle, prime,
                                       intMath );
mod.multiply( middleCubed, termMiddle, prime,
                                       intMath );

Integer tempY;
tempY.copy( termLeft );
mod.subtract( tempY, middleCubed, prime, intMath );
mod.subtract( tempY, p.Y, prime, intMath );

Y.copy( tempY );

return true;
}
