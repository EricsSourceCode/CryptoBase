/*

// Copyright Eric Chauvin 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "EPoint.h"



bool EPoint::isConjugate( const EPoint& p,
                          const Integer& modulus,
                          Mod& mod,
                          IntegerMath& intMath )
                                        const
{
if( !X.isEqual( p.X ))
  return false;

Integer temp;
temp.copy( p.Y );
mod.negate( temp, modulus, intMath );
if( !temp.isEqual( Y ))
  throw "Can't happen. Point not on curve.";
//  return false;

return true;
}



// isConjugate() defines the meaning of
// the phrase
// Additive Identity:  P + -P = 0;
// Geometrically speaking, a curve like
// y^2 = x^3 + ax + b is
// symmetric around the X axis.
// Because -y^2 equals y^2.
// As the curve crosses the X axis it goes
// directly vertical, which would be an
// infinite slope, so to speak.
// The conjugate has the same x coordinate.
// But it has a negated y coordinate.
// So the point and its conjugate are on
// a vertical line.

// If y was 1 and base was 5.
// y = -1 + 5 = 4.
// P + -P = 0.
// 1 + 4 = 0 mod 5.
// y + negate( y ) = 0.
// Also, -4 + 5 is 1.
// So negating 4 makes it 1.


// Equations are from RFC 6090 section-3
// Elliptic Curve Groups.

bool EPoint::add( const EPoint& p,
                  const EPoint& q,
                  const Integer& coefA,
                  const Integer& coefB,
                  const Integer& modulus,
                  Integer& gcd,
                  Mod& mod,
                  IntegerMath& intMath  )
{
if( modulus.isEqual( coefB ))
  throw "EPoint modulus can't be coefB.";

// q + Infinity = q.
if( p.getInfin())
  {
  // If q was infinite too, then it's like
  // zero + zero = zero.
  copy( q );
  return true;
  }

if( q.getInfin())
  {
  copy( p );
  return true;
  }

if( p.isEqual( q ))
  {
  return doubleP( p, coefA, modulus, gcd,
                  mod, intMath );
  }

if( p.X.isEqual( q.X ))
  {
  // If it's the same x then the y values
  // can only be the negated values for
  // each other.

  // For testing only.
  if( !p.isConjugate( q, modulus, mod, intMath ))
    {
    throw "Can't happen. !p.isConjugate( q )";
    }

  // Then the two points are in a vertical
  // line, and the line points to something
  // At Infinity.
  setInfin( true );
  return true;
  }

// One of these points, either p or q, might
// be this same object.  Like P = P + Q.
Integer pX;
Integer pY;
Integer qX;
Integer qY;
pX.copy( p.X );
pY.copy( p.Y );
qX.copy( q.X );
qY.copy( q.Y );

// The X values are different here, but
// the numerator, the difference in y values,
// can equal 0.  So the slope can be zero.

// If p.x is different from q.x can p.y equal
// q.y?  Yes.

// p.y - q.y
Integer numerator;
numerator.copy( pY );
mod.subtract( numerator, qY, modulus, intMath );

// if( numerator.isZero())
  // throw "EPoint. Yes, the numerator can be zero.";

Integer denom;
denom.copy( pX );
mod.subtract( denom, qX, modulus, intMath );

// The x values are not equal, so denom can't
// be zero, but it can be 1.
Integer slope;
if( denom.isMoreThanInt24( 1 ))
  {
  // Avoid calling divide() if denominator is 1
  // because it would just return the
  // numerator if it was 1.

  // If this can't find an inverse it returns
  // false, and then it has found a factor in
  // the Lenstra factoring method.
  // This has to do the extended Euclidean
  // algorithm to get the multiplicative inverse.

  if( !mod.divide( slope, numerator, denom,
                   modulus, gcd, intMath ))
    return false;

  }
else
  {
  slope.copy( numerator );
  }

Integer slopeSqr;
slopeSqr.copy( slope );
mod.square( slopeSqr, modulus, intMath );

// x = slopeSqr - p.x - q.x;
Integer tempX;
tempX.copy( slopeSqr );
mod.subtract( tempX, pX, modulus, intMath );
mod.subtract( tempX, qX, modulus, intMath );
X.copy( tempX );

Integer xDelta;
xDelta.copy( pX );
mod.subtract( xDelta, X, modulus, intMath );

// if( xDelta == 0 )
//   Then that's OK.

// y = (slope * xDelta) - p.y;
Integer tempY;
tempY.copy( slope );
mod.multiply( tempY, xDelta, modulus, intMath );
mod.subtract( tempY, pY, modulus, intMath );
Y.copy( tempY );
return true;
}




bool EPoint::doubleP( const EPoint& p,
                      const Integer& coefA,
                      const Integer& modulus,
                      Integer& gcd,
                      Mod& mod,
                      IntegerMath& intMath )
{
// coefA and coefB are the coefficients in
// this equation:
// y^2 = a*(x^3) + b

// This might be doubling itself, so
// this.x would be the same as p.x.

Integer pX;
Integer pY;
pX.copy( p.X );
pY.copy( p.Y );

// Double a point:
// P + P = 2P
// The number 3 comes from the derivative,
// or the slope of the line, of x^3, so it's
// 3x^2.  coefB is a constant and so it's
// not in the derivative.

if( p.getInfin())
  {
  // Infinite + Infinite = Infinite
  copy( p );
  return true;
  }

if( pY.isZero() )
  {
  // Then the conjugate to Y would be
  // zero too.
  setInfin( true );
  return true;
  }

// The derivative of x^3 + ax is
//                  3x^2 + a

// Multiplying by 3 here means you don't
// want to use base == 3.
// "a prime order finite
// field with characteristic greater
// than three"


// numerator = (3 * (p.x * p.x)) + coefA;

Integer numerator;
if( pX.isZero())
  {
  numerator.copy( coefA );
  mod.makeExact( numerator, modulus, intMath );
  }
else
  {
  numerator.copy( pX );
  mod.square( numerator, modulus, intMath );
  mod.multiplyL( numerator, 3, modulus, intMath );

  mod.add( numerator, coefA, modulus, intMath );
  }

// p.y is not zero here.
// Multiplying by 2 here means you don't
// want to use a base == 2.
Integer denom;
denom.copy( pY );
mod.multiplyL( denom, 2, modulus, intMath );

Integer slope;
if( !mod.divide( slope, numerator, denom,
                 modulus, gcd, intMath ))
  return false;

Integer slopeSqr;
slopeSqr.copy( slope );
mod.square( slopeSqr, modulus, intMath );

Integer rightSide;
rightSide.copy( pX );
mod.multiplyL( rightSide, 2, modulus, intMath );

// x = slopeSqr - (2 * p.x);

Integer tempX;
tempX.copy( slopeSqr );
mod.subtract( tempX, rightSide, modulus, intMath );
X.copy( tempX );

// p.x minus the new x.
// y = slope * (p.x - x) - p.y;
Integer xPart;
xPart.copy( pX );
mod.subtract( xPart, X, modulus, intMath );

Integer tempY;
tempY.copy( slope );
mod.multiply( tempY, xPart, modulus, intMath );
Y.copy( tempY );
mod.subtract( Y, pY, modulus, intMath );
return true;
}

*/
