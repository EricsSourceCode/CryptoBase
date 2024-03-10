// Copyright Eric Chauvin, 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


// There is a good tutorial by Martin Kleppmann
// called Implementing Curve25519/X25519: A
// Tutorial on Elliptic Curve Cryptography.
// https://martin.kleppmann.com/papers/
//                            curve25519.pdf


// curve25519 is named for the prime: 2^255 - 19.

// See makeThePrime().
// This makes the value 2^255:
// Int32 highBit = 0x8000;
// prime.setD( 10, highBit );
// And then subtracting 19:
// intMath.subLong48( prime, 19 );
// That makes the prime value so
// that the very top bit is zero.
// The top bit is then at 0x4000, not 0x8000.
// The prime is:
// "7FFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFF"
// "FFFFFFFFFFFFFFFFFFFFFFFFFFFFED";


#include "MCurve.h"
#include "TonelliShanks.h"
#include "Randomish.h"
#include "../CppInt/Mod.h"

#include "../CppBase/StIO.h"
#include "../CppBase/ByteHex.h"
#include "../CppBase/ByteSet64.h"
#include "../CppBase/TimeApi.h"



bool MCurve::isOnCurve( const MPoint& aPoint,
                        Mod& mod,
                        IntegerMath& intMath )
{
// y^2 = x^3 + Ax^2 + x

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

Integer left;
aPoint.getY( left );

mod.multiply( left, left, prime, intMath );

Integer X;
aPoint.getX( X );

Integer right;
aPoint.getX( right );
mod.multiply( right, right, prime, intMath );
mod.multiply( right, X, prime, intMath );

// Now right is x cubed.

Integer ax2;
ax2.copy( X );
mod.multiply( ax2, ax2, prime, intMath );
mod.multiplyL( ax2, MPoint::coefA, prime, intMath );

mod.add( right, ax2, prime, intMath );
mod.add( right, X, prime, intMath );

if( left.isEqual( right ))
  return true;

return false;
}




bool MCurve::scalarMult( MPoint& result,
                         const Integer& k,
                         Integer& gcd,
                         IntegerMath& intMath,
                         Mod& mod )
{
// If k = 23
// 23 = 16 + 7 = 0b10111.
// 23 = 2^4 + 2^2 + 2 + 1
// 23 times P = (2^4 + 2^2 + 2 + 1)P
// 23 times P = 2^4P + 2^2P + 2P + P

MPoint accumP;
accumP.setInfin( true ); // Additive Identiry.

MPoint tempD;
MPoint tempAdd;
MPoint toDouble;

toDouble.copy( result );

Integer kShift;
kShift.copy( k );

for( Int32 count = 0; count < 100000; count++ )
  {
  if( (kShift.getD( 0 ) & 1) != 0 )
    {
    if( !tempAdd.add( accumP, toDouble,
                 prime, gcd, mod, intMath ))
      return false;

    if( !isOnCurve( tempAdd, mod, intMath ))
      throw "tempAdd not on curve.";

    accumP.copy( tempAdd );
    }

  if( !tempD.doubleP( toDouble, prime, gcd,
                      mod, intMath ))
    return false;

  if( !isOnCurve( toDouble, mod, intMath ))
    throw "toDouble not on curve.";

  toDouble.copy( tempD );
  kShift.shiftRight( 1 );
  if( kShift.isZero())
    break;

  }

result.copy( accumP );

return true;
}



// The non-obfuscated version.
void MCurve::cSwap1( Int32 bit,
                     Integer& X2,
                     Integer& X3 )
{
// If bit is 1 then it is ~0 which is all ones.
// If bit is 0 then it is ~(-1) which is all zeros.
// If c is all zeros then xor with it does
// nothing.  It is not swapped.

// If bit is zero then it is not swapped.
// Int32 c = ~(bit - 1);

if( bit == 0 )
  return;

Integer temp;
temp.copy( X2 );
X2.copy( X3 );
X3.copy( temp );
}



/*
I'm making a new number system to do this.
void MCurve::cSwap2( Int32 bit,
                     Integer& X2,
                     Integer& X3 )
{
Integer mask;
if( bit == 0 )
  mask.setBitMask( 10, 0 );
else
  mask.setBitMask( 10, 0xFFFFFF );
////////

// X2 xor (X2 xor X3) = X3
// (X2 xor X2) xor X3 = X3

Integer X2xorX3;
X2xorX3.xorBits( X2, X3 );

// Integer dummy;
// dummy.andBits( X2xorX3, mask );

Integer X2Temp;
Integer X3Temp;

if( bit == 1 )
  {
  X2Temp.xorBits( X2, X2xorX3 );
  X3Temp.xorBits( X3, X2xorX3 );
  }

X2.copy( X2Temp );
X3.copy( X3Temp );
}
*/





// Montgomery Ladder.
// Equations are from RFC 7748.


bool MCurve::montLadder1( Integer& result,
                          const Integer& U,
                          const Integer& k,
                          IntegerMath& intMath,
                          Mod& mod )
{

try
{
if( U.isZero())
  throw "montLadder1 U.isZero";

if( k.isZero())
  throw "montLadder1 k.isZero";

Integer X1;
Integer X2;
Integer X3;
Integer Z2;
Integer Z3;
Integer Z3Right;
Integer bitTest;
Integer bitMask;
Integer A;
Integer AA;
Integer B;
Integer BB;
Integer E;
Integer C;
Integer D;
Integer DA;
Integer CB;

const Int32 a24 = (486662 - 2) / 4;
Int32 kt = 0;


// x_1 = u
// x_2 = 1
// z_2 = 0
// x_3 = u
// z_3 = 1
// swap = 0

Int32 swap = 0;

X1.copy( U );
X2.setToOne();
Z2.setToZero();
X3.copy( U );
Z3.setToOne();

// 2^254:
bitMask.setDigitAndClear( 10, 0x4000 );


// "the bits parameter should be set to
// 255 for X25519 and 448 for
// X448:
// For t = bits-1 down to 0:

Integer X2Test;
Integer X3Test;

for( Int32 t = 254; t >= 0; t-- )
  {
  // k_t = (k >> t) & 1

  bitTest.andBits( bitMask, k );
  bitMask.shiftRight( 1 );

  if( bitTest.isZero())
    kt = 0;
  else
    kt = 1;

  swap ^= kt;

  // X2Test.copy( X2 );
  // X3Test.copy( X3 );
  // cSwap2( swap, X2Test, X3Test );

  cSwap1( swap, X2, X3 );
  cSwap1( swap, Z2, Z3 );
  swap = kt;

  // if( !X2Test.isEqual( X2 ))
    // throw "Swap is not right.";

  // if( !X3Test.isEqual( X3 ))
    // throw "Swap is not right.";


  //  A = x_2 + z_2
  A.copy( X2 );
  mod.add( A, Z2, prime, intMath );

  // AA = A^2
  AA.copy( A );
  mod.multiply( AA, AA, prime, intMath );

  // B = x_2 - z_2
  B.copy( X2 );
  mod.subtract( B, Z2, prime, intMath );

  // BB = B^2
  BB.copy( B );
  mod.multiply( BB, BB, prime, intMath );

  // E = AA - BB
  E.copy( AA );
  mod.subtract( E, BB, prime, intMath );

  // C = x_3 + z_3
  C.copy( X3 );
  mod.add( C, Z3, prime, intMath );

  // D = x_3 - z_3
  D.copy( X3 );
  mod.subtract( D, Z3, prime, intMath );

  // DA = D * A
  DA.copy( D );
  mod.multiply( DA, A, prime, intMath );

  // CB = C * B
  CB.copy( C );
  mod.multiply( CB, B, prime, intMath );

  // x_3 = (DA + CB)^2
  X3.copy( DA );
  mod.add( X3, CB, prime, intMath );
  mod.multiply( X3, X3, prime, intMath );

  // z_3 = x_1 * (DA - CB)^2
  Z3Right.copy( DA );
  mod.subtract( Z3Right, CB, prime, intMath );
  mod.multiply( Z3Right, Z3Right, prime, intMath );
  Z3.copy( X1 );
  mod.multiply( Z3, Z3Right, prime, intMath );

  // x_2 = AA * BB
  X2.copy( AA );
  mod.multiply( X2, BB, prime, intMath );

  // z_2 = E * (AA + a24 * E)
  Z2.copy( E );
  mod.multiplyL( Z2, a24, prime, intMath );
  mod.add( Z2, AA, prime, intMath );
  mod.multiply( Z2, E, prime, intMath );
  }

// (x_2, x_3) = cswap(swap, x_2, x_3)
cSwap1( swap, X2, X3 );

// (z_2, z_3) = cswap(swap, z_2, z_3)
cSwap1( swap, Z2, Z3 );

Integer primeM2;
primeM2.copy( prime );

primeM2.decrement();
primeM2.decrement(); // Minus 2.
// intMath.subLong56( primeM2, 2 );
// mod.subtractL( primeM2, 2, prime, intMath );

Integer z2Inv;
z2Inv.copy( Z2 );
mod.toPower( z2Inv, primeM2, prime, intMath );

Integer result2;
result2.copy( X2 );
mod.multiply( result2, z2Inv, prime, intMath );

// Return x_2 * (z_2^(p - 2))
Integer gcd;
mod.divide( result, X2, Z2, prime, gcd, intMath );

if( !result2.isEqual( result ))
  throw "montLadder1 !result2.isEqual( result )";

return true;

}
catch( const char* in )
  {
  StIO::putS( "Exception in MCurve::montLadder1" );
  StIO::putS( in );
  return false;
  }

catch( ... )
  {
  StIO::putS( "Exception in MCurve::montLadder1" );

  const char* in = "An unknown exception"
                   " happened.";

  StIO::putS( in );
  return false;
  }
}



void MCurve::makeRandExponentK( Integer& k,
                                CharBuf& keyShare )
{
ByteArray kBytes;
kBytes.setSize( 32 );
makeRandom32Bytes( kBytes );
clampK( kBytes );

keyShare.clear();
keyShare.appendCharArray( kBytes, 32 );

cArrayToInt( kBytes, k );
}




/*
void MCurve::test( Mod& mod,
                   IntegerMath& intMath )
{
StIO::putS( "Start of MCurve test." );

CharBuf showBuf;

CharArray cArray;
CharArray cArray2;
cArray.setSize( 32 );
cArray2.setSize( 32 );


makeRandom32Bytes( cArray );

Integer U;

cArrayToInt( cArray, U );

StIO::putS( "U as hex:" );
intMath.toString16( U, showBuf );
StIO::putCharBuf( showBuf );
StIO::putLF();

StIO::putS( "uCoordTo32Bytes:" );
uCoordTo32Bytes( U, cArray2, // mod,
                  intMath );

if( !cArray.isEqual( cArray2 ))
  throw "32 bytes are not equal.";

Integer uTest;
StIO::putS( "cArrayToInt:" );
cArrayToInt( cArray2, uTest );

if( !uTest.isEqual( U ))
  throw "uTest not equal.";

StIO::putS( "Utest as hex:" );
intMath.toString16( uTest, showBuf );
StIO::putCharBuf( showBuf );
StIO::putLF();


MPoint basePoint;
Integer X;
X.setFromLong48( 9 );
Integer Y;
const char* yStr = "147816194475895447910"
                   "205935684099868872646"
                   "061346164752889648818"
                   "37755586237401";

intMath.setFromStr( Y, yStr );

basePoint.setX( X );
basePoint.setY( Y );

if( !isOnCurve( basePoint, mod, intMath ))
  throw "Basepoint not on curve.";

CharArray kBytes;
kBytes.setSize( 32 );
makeRandom32Bytes( kBytes );
clampK( kBytes );

Integer k;
cArrayToInt( kBytes, k );

StIO::putS( "k as hex:" );
intMath.toString16( k, showBuf );
StIO::putCharBuf( showBuf );
StIO::putLF();

MPoint bpResult;
bpResult.copy( basePoint );

Integer gcd;
scalarMult( bpResult, k, gcd, intMath, mod );
Integer slowX;
bpResult.getX( slowX );

StIO::putS( "slowX:" );
intMath.toString16( slowX, showBuf );
StIO::putCharBuf( showBuf );
StIO::putLF();

Integer U;
basePoint.getX( U );
Integer fastX;
montLadder1( fastX, U, k, intMath, mod );

StIO::putS( "fastX:" );
intMath.toString16( fastX, showBuf );
StIO::putCharBuf( showBuf );
StIO::putLF();

if( !fastX.isEqual( slowX ))
  throw "fastX != slowX";



StIO::putS( "End of MCurve test." );
}
*/




/*
void MCurve::testVectors( Mod& mod,
                          IntegerMath& intMath )
{
StIO::putS( "Testing with test vectors." );

Integer k;
Integer U;

const char* kStr = "310298424921150409048955"
                   "60451863089656"
                   "4727726046782602655312210"
                   "36453811406496";

intMath.setFromStr( k, kStr );

const char* uStr = "34426434033919594451155"
                   "107781188821651"
                   "316167215306631574996226"
                   "621102155684838";

intMath.setFromStr( U, uStr );

Integer fastX;
montLadder1( fastX, U, k, intMath, mod );

CharBuf showBuf;
StIO::putS( "fastX is:" );
// intMath.toString10( fastX, showBuf );
intMath.toString16( fastX, showBuf );
StIO::putCharBuf( showBuf );
StIO::putLF();

// Output u-coordinate:
// c3da55379de9c6908e94ea4df28d084f3
// 2eccf03491c71f754b4075577a28552

}
*/




void MCurve::makeThePrime( IntegerMath& intMath )
{
// 24 bits per digit
// 256 bits is 32 bytes times 8.
// 3 times 11 is 33 bytes.
prime.setIndex( 10 ); // Eleven 24 bit digits.
for( Int32 count = 0; count <= 10; count++ )
  prime.setD( count, 0 );

// At the top of 32 bytes.
Int32 highBit = 0x8000;
prime.setD( 10, highBit ); // 2^255

Integer nineteen;
nineteen.setFromLong48( 19 );
intMath.subtract( prime, nineteen );

CharBuf showBuf;
StIO::putS( "curve25519 prime is:" );
intMath.toString10( prime, showBuf );
StIO::putCharBuf( showBuf );
StIO::putLF();

// StIO::putS( "As hex:" );
// intMath.toString16( prime, showBuf );
// StIO::putCharBuf( showBuf );
// StIO::putLF();

const char* primeStr = "578960446186580977117"
                       "854925043439539266349"
                       "923328202820197287920"
                       "03956564819949";

CharBuf primeBuf( primeStr );
Integer primeTest;
intMath.setFromStr( primeTest, primeBuf );

if( !prime.isEqual( primeTest ))
  throw "Prime is not right.";


/*

const char* primeStrHex = "7FFFFFFFFFFFFFFFF"
                          "FFFFFFFFFFFFFFFFF"
                          "FFFFFFFFFFFFFFFFF"
                          "FFFFFFFFFFFED";

CharBuf primeBufHex( primeStrHex );
Integer primeTestHex;
intMath.setFromHexStr( primeTestHex, primeBufHex );

if( !prime.isEqual( primeTestHex ))
  StIO::putS( "Prime Hex is not right." );
*/


}



void MCurve::makeRandom32Bytes(
                            ByteArray& cArray )
{
CharBuf cBuf;
Randomish::makeRandomBytes( cBuf, 3 * 12 );

Uint8 aByte = 0;
for( Int32 count = 0; count < 32; count++ )
  {
  aByte = cBuf.getU8( count );
  cArray.setU8( count, aByte );
  }
}



void MCurve::clampK( ByteArray& cArray )
{
// This is only for the scalar K.

// Clear the bottom 3 bits.
Uint8 aByte = cArray.getU8( 0 );
aByte = aByte & 0xF8;
cArray.setU8( 0, aByte );

aByte = cArray.getU8( 31 );
aByte = aByte & 0x7F;
aByte = aByte | 0x40;  // Second highest bit: 2^254
cArray.setU8( 31, aByte );
}



void MCurve::clampU( ByteArray& cArray )
{
// This is only for the U Coordinate.

// RFC 7748:
// When receiving such an array,
// implementations of X25519
// (but not X448) MUST mask the most
// significant bit in the final byte.
// This is done to preserve compatibility
// with point formats that
// reserve the sign bit for use in other
// protocols..."
// "Implementations MUST accept non-canonical
// values and process them as
// if they had been reduced modulo the
// field prime.  The non-canonical
// values are 2^255 - 19 through 2^255 - 1 for

Uint8 aByte = cArray.getU8( 31 );
// Clear what would be a sign bit that is
// ignored.
aByte = aByte & 0x7F;
cArray.setU8( 31, aByte );
}



/*
void MCurve::clampKInt( Integer& toSet )
{
// This is for the scalar K.
What about this?
if( prime.paramIsGreater( tempU ))
  {
  CharBuf showBuf;
  StIO::putS( "tempU is:" );
  intMath.toString16( tempU, showBuf );
  throw "tempU is bigger than prime.";
  // mod.makeExact( tempU, prime, intMath );
  }

// Clear the bottom 3 bits.
Int64 D0 = toSet.getD( 0 );
D0 = D0 & 0xFFFFF8;
toSet.setD( 0, D0 );

Int64 D10 = toSet.getD( 10 );
// clear the top bit.
D10 = D10 & 0x7FFF;
// Set second highest bit: 2^254
D10 = D10 | 0x4000;
toSet.setD( 10, D10 );
}
*/





void MCurve::uCoordTo32Bytes( const Integer& U,
                          ByteArray& uBytes,
                          Mod& mod,
                          IntegerMath& intMath )
                                        const
{
Integer tempU;
tempU.copy( U );

// StIO::putS( "Utest as hex:" );

if( prime.isEqual( tempU ))
  throw "make32Bytes prime is equal.";

if( prime.paramIsGreater( tempU ))
  {
  StIO::putS( "tempU is > prime:" );
  mod.makeExact( tempU, prime, intMath );
  }

uBytes.setSize( 32 );

const Int32 max = tempU.getIndex();
if( max > 10 )
  throw "This can't happen with max too long.";

Uint8 aByte = 0;
Int32 where = 0;

// Put the bytes in little-endian order.
for( Int32 count = 0; count <= max; count++ )
  {
  Int64 digit =  tempU.getD( count );

  aByte = ByteSet64::getByte( 0, digit );
  uBytes.setU8( where, aByte );
  where++;

  // ByteHex::showByte( aByte );

  aByte = ByteSet64::getByte( 1, digit );
  uBytes.setU8( where, aByte );
  where++;

  // ByteHex::showByte( aByte );

  if( where >= 32 )
    break;

  aByte = ByteSet64::getByte( 2, digit );
  uBytes.setU8( where, aByte );
  where++;

  // ByteHex::showByte( aByte );
  }

for( Int32 count = 0; count < 32; count++ )
  {
  if( where >= 32 )
    break;

  uBytes.setU8( where, 0 );
  where++;
  }

// StIO::putLF();
}




void MCurve::cArrayToInt( const ByteArray& cAr,
                          Integer& X )
{
// This can be either a K scalar or a U
// coordinate.  Clamping is not done here.

X.setIndex( 10 );

Int32 where = 0;
for( Int32 count = 0; count <= 10; count++ )
  {
  Int64 digit = 0;

  Uint8 aByte = cAr.getU8( where );
  digit = ByteSet64::setByte( 0, digit, aByte );
  where++;

  aByte = cAr.getU8( where );
  digit = ByteSet64::setByte( 1, digit, aByte );
  where++;

  if( where >= 32 )
    {
    X.setD( count, digit );
    break;
    }

  aByte = cAr.getU8( where );
  digit = ByteSet64::setByte( 2, digit, aByte );
  where++;

  X.setD( count, digit );
  }
}
