/*

// Copyright Eric Chauvin 2022 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "FindFacQr.h"
#include "../CppBase/Casting.h"
#include "../CppBase/StIO.h"
#include "../Crt/Crt3.h"


FindFacQr::FindFacQr( void )
{
StIO::putS( "FindFacQr constructor." );
}



FindFacQr::FindFacQr( const FindFacQr& in )
{
if( in.testForCopy )
  return;

throw "FindFacQr copy constructor.";
}


FindFacQr::~FindFacQr( void )
{
}



void FindFacQr::init( IntegerMath& intMath,
                      const SPrimes& sPrimes )
{
StIO::putS( "FindFacQr.init()." );
goodX.init( sPrimes );
crtMath.init( intMath,
              sPrimes );
}



bool FindFacQr::findIt( const Integer& pubKey,
                        Integer& prime1,
                        Integer& prime2,
                        IntegerMath& intMath,
                        const SPrimes& sPrimes,
                        const MultInv& multInv,
                        const QuadRes& quadRes )
{
StIO::putS( "FindFacQr started findIt()." );

// This has to be set up before the arrays
// below.
setupGoodXQuadRes( pubKey, multInv, quadRes,
                   intMath, sPrimes );

// This would have already been checked by
// FindFacSm, so it doesn't have any
// small prime factors.

// The count varies depending on pubKey.

// howMany for 17 is:      9,216
// howMany for 23 is:    995,328
// howMany for 29 is: 14,929,920

// howMany for 17 is:     12,096
// howMany for 23 is:  1,197,504
// howMany for 29 is: 17,962,560

makeQrCounts( pubKey, intMath, sPrimes,
              multInv, quadRes );

makeSmallAr17();

makeBigAr23();

makeBigAr29();

if( !findQr31( pubKey, prime1, prime2,
               intMath ))
  {
  StIO::putS( "FindQr returned false." );
  return false;
  }

CharBuf showIt;
StIO::putS( "prime1: " );
intMath.toString10( prime1, showIt );
StIO::putCharBuf( showIt );
StIO::putLF();

StIO::putS( "prime2: " );
intMath.toString10( prime2, showIt );
StIO::putCharBuf( showIt );
StIO::putLF();

StIO::putS( "End of FindFacQr::findIt()." );
return true;
}




void FindFacQr::makeSmallAr17( void )
{
StIO::putS( "makeSmallAr17" );

const Int32 arSize =
               qrCounts.getVal( 0 ) *  // 2
               qrCounts.getVal( 1 ) *  // 3
               qrCounts.getVal( 2 ) *  // 5
               qrCounts.getVal( 3 ) *  // 7
               qrCounts.getVal( 4 ) *  // 11
               qrCounts.getVal( 5 ) *  // 13
               qrCounts.getVal( 6 );   // 17

StIO::putS( "arSize for 17 is: " );
StIO::printFD( arSize );
StIO::putLF();

smallAr17.setSize( arSize );

const Int32 max = 2 * 3 * 5 * 7 * 11 * 13 * 17;

Int32 howMany = 0;
for( Int32 count = 0; count < max; count++ )
  {
  Int32 modVal = count % 3;
  if( !goodX.getVal( 1, modVal )) // 3
    continue;

  modVal = count % 5;
  if( !goodX.getVal( 2, modVal )) // 5
    continue;

  modVal = count % 7;
  if( !goodX.getVal( 3, modVal )) // 7
    continue;

  modVal = count % 11;
  if( !goodX.getVal( 4, modVal )) // 11
    continue;

  modVal = count % 13;
  if( !goodX.getVal( 5, modVal )) // 13
    continue;

  modVal = count % 17;
  if( !goodX.getVal( 6, modVal )) // 17
    continue;

  smallAr17.setVal( howMany, count );
  howMany++;
  }


// This varies a lot with the pubKey.
// howMany for 17 is: 12096

smallAr17Last = howMany;

StIO::printF( "howMany for 17 is: " );
StIO::printFD( howMany );
StIO::putLF();
}



void FindFacQr::makeBigAr23( void )
{
StIO::putS( "makeBigAr23" );

const Int32 arSize =
              qrCounts.getVal( 0 ) *  // 2
              qrCounts.getVal( 1 ) *  // 3
              qrCounts.getVal( 2 ) *  // 5
              qrCounts.getVal( 3 ) *  // 7
              qrCounts.getVal( 4 ) *  // 11
              qrCounts.getVal( 5 ) *  // 13
              qrCounts.getVal( 6 ) *  // 17
              qrCounts.getVal( 7 ) *  // 19
              qrCounts.getVal( 8 );   // 23

StIO::printF( "arSize for 23 is: " );
StIO::printFD( arSize );
StIO::putLF();

bigAr23.setSize( arSize );

Int32 howMany = 0;

Int32 bigBase = 2 * 3 * 5 * 7 * 11 * 13 * 17;


const Int32 last17 = smallAr17Last;

for( Int32 countBase = 0;
             countBase < 19 * 23; countBase++ )
  {
  Int32 basePart = bigBase * countBase;

  for( Int32 count = 0; count < last17; count++ )
    {
    Int32 allParts = basePart +
                 smallAr17.getVal( count );

    Int32 modVal = allParts % 19;
    if( !goodX.getVal( 7, modVal )) // 19
      continue;

    modVal = allParts % 23;
    if( !goodX.getVal( 8, modVal )) // 23
      continue;

    if( allParts > 0x1FFFFFFF )
      throw "makeBigAr23 allParts > 0x1FFFFFFF";

    bigAr23.setVal( howMany, allParts );
    howMany++;
    }
  }

bigAr23Last = howMany;

StIO::putS( "howMany for 23 is: " );
StIO::printFD( howMany );
StIO::putLF();
}





//////////
2
3
5
7
11
13
17
19
23
29
31
37
41
43
47
53
59
61
67
71
73
79
83
89
97
101
103
107
109
113
127
131
137
139
149
151
157
163
167
173
179
181
191
193
197
199
211
223
227
229
233
239
241
251
257
263
269
271
277
281
283
293
307
311
313
317
331
337
347
349
353
359
367
373
379
383
389
397
401
409
419
421
431
433
439
443
449
457
461
463
467
479
487
491
499
503
509
521
523
541
//////////////





// What is the array size that is needed for
// this?

void FindFacQr::makeBigAr29( void )
{
StIO::putS( "Top of makeBigAr29" );

// This is Int64 size.
// const Int32 arSize =
//             qrCounts.getVal( 0 ) *  // 2
//             qrCounts.getVal( 1 ) *  // 3
//             qrCounts.getVal( 2 ) *  // 5
//             qrCounts.getVal( 3 ) *  // 7
//             qrCounts.getVal( 4 ) *  // 11
//             qrCounts.getVal( 5 ) *  // 13
//             qrCounts.getVal( 6 ) *  // 17
//             qrCounts.getVal( 7 ) *  // 19
//             qrCounts.getVal( 8 ) *  // 23
//             qrCounts.getVal( 9 );   // 29

// StIO::printF( "arSize for 29 is: " );
// StIO::printFD( arSize );
// StIO::putLF();

//             0xF FF FF FF
Int32 aBitIntAr = 0xFFFFFFF;
bigAr29.setSize( aBitIntAr );


// const Int64 max = 2L * 3L * 5L * 7L *
//                   11L * 13L *
//                   17L * 19L * 23L * 29L;

// Max 29 is: 6,469,693,230

// StIO::printF( "Max 29 is: " );
// StIO::printFD( max );
// StIO::putLF();

Int32 howMany = 0;

Int64 base = 2LL * 3LL * 5LL * 7LL * 11LL *
             13LL * 17LL * 19LL * 23LL;

const Int32 last23 = bigAr23Last;

for( Int32 countBase = 0; countBase < 29;
                                 countBase++ )
  {
  Int64 basePart = base * countBase;

  for( Int32 count = 0; count < last23; count++ )
    {
    Int64 allParts = basePart +
                    bigAr23.getVal( count );
    Int32 allParts29 = allParts % 29;

    Int32 modVal = allParts29 % 29;
    if( !goodX.getVal( 9, modVal )) // 29
      continue;

    // 47 bits.
    if( allParts > 0x7FFFFFFFFFFFLL )
      throw
      "makeBigAr29 allParts > 0x7FFFFFFFFFFFLL";

    howMany++;
    bigAr29.setVal( howMany, allParts );
    }
  }

bigAr29Last = howMany;

StIO::putS( "howMany for 29 is: " );
StIO::printFD( howMany );
StIO::putLF();
}






bool FindFacQr::findQr31(
                    const Integer& pubKey,
                    Integer& prime1,
                    Integer& prime2,
                    IntegerMath& intMath )
{
StIO::putS( "Top of findQr31" );

// const Int64 max = 2L * 3L * 5L * 7L *
//                   11L * 13L *
//                   17L * 19L * 23L * 29L;

// Max 29 is: 6,469,693,230

// StIO::printF( "Max 29 is: " );
// StIO::printFD( max );
// StIO::putLF();


Int64 howMany1 = 0;
Int64 howMany2 = 0;

Int32 prodByte = pubKey.getD( 0 ) & 0xFF;


// 6,469,693,230
const Int64 base = 2LL * 3LL * 5LL * 7LL *
       11LL * 13LL * 17LL * 19LL * 23LL * 29LL;

Integer bigBase;
bigBase.setFromLong48( base );

Integer basePart;
Integer bigParts;

const Int32 last29 = bigAr29Last;

for( Int32 countBase = 0;
           countBase < 10000; countBase++ )
  {
  basePart.copy( bigBase );
  basePart.multiply24( countBase );

  Int32 basePartByte = basePart.getD( 0 )
                                   & 0xFF;
  Int32 basePart31 = intMath.getMod24(
                              basePart, 31 );
  Int32 basePart37 = intMath.getMod24(
                              basePart, 37 );
  Int32 basePart41 = intMath.getMod24(
                              basePart, 41 );
  Int32 basePart43 = intMath.getMod24(
                              basePart, 43 );
  Int32 basePart47 = intMath.getMod24(
                              basePart, 47 );
  Int32 basePart53 = intMath.getMod24(
                              basePart, 53 );
  Int32 basePart59 = intMath.getMod24(
                              basePart, 59 );

  Int32 startAt = 0;
  if( countBase == 0 )
    startAt = 1; // Skip the number 1.

  for( Int32 count = startAt;
                   count < last29; count++ )
    {
    const Int64 val29 = bigAr29.getVal( count );

    Int32 allBytes = basePartByte +
                              (val29 & 0xFF);
    allBytes = allBytes * allBytes;
    allBytes += prodByte;
    allBytes = allBytes & 0xFF;
    if( !QuadRes::bytesQR( allBytes ))
      {
      howMany1++;
      continue;
      }

    Int64 allParts = basePart31 + val29;
    allParts = allParts % 31;
    Int32 small32 = Casting::i64ToI32(
                                  allParts );

    if( !goodX.getVal( 10, small32 )) // 31
      {
      // howMany++;
      continue;
      }

    allParts = basePart37 + val29;
    allParts = allParts % 37;
    small32 = Casting::i64ToI32( allParts );
    if( !goodX.getVal( 11, small32 )) // 37
      {
      // howMany++;
      continue;
      }

    allParts = basePart41 + val29;
    allParts = allParts % 41;
    small32 = Casting::i64ToI32( allParts );
    if( !goodX.getVal( 12, small32 )) // 41
      {
      // howMany++;
      continue;
      }

    allParts = basePart43 + val29;
    allParts = allParts % 43;
    small32 = Casting::i64ToI32( allParts );
    if( !goodX.getVal( 13, small32 )) // 43
      {
      // howMany++;
      continue;
      }

    allParts = basePart47 + val29;
    allParts = allParts % 47;
    small32 = Casting::i64ToI32( allParts );
    if( !goodX.getVal( 14, small32 )) // 47
      {
      // howMany++;
      continue;
      }

    allParts = basePart53 + val29;
    allParts = allParts % 53;
    small32 = Casting::i64ToI32( allParts );
    if( !goodX.getVal( 15, small32 )) // 53
      {
      // howMany++;
      continue;
      }

    allParts = basePart59 + val29;
    allParts = allParts % 59;
    small32 = Casting::i64ToI32( allParts );
    if( !goodX.getVal( 16, small32 )) // 59
      {
      // howMany++;
      continue;
      }


    howMany2++;
    if( howMany2 > 1000000000 )
      throw "howMany2 it too big.";
      // return false;

    // Do the big sqrRoot.
    bigParts.copy( basePart );
    bigParts.addLong48( bigAr29.getVal( count ));

    if( testSquare( pubKey,
                    bigParts,
                    prime1,
                    prime2,
                    intMath ))
      {
      // It does this more than 200 million
      // times for 48 bit prime size numbers.
      StIO::printF( "Howmany1: " );
      StIO::printFD( howMany1 );
      StIO::putLF();

      StIO::printF( "Howmany sqrRoot: " );
      StIO::printFD( howMany2 );
      StIO::putLF();
      return true;
      }

    }
  }

StIO::printF( "Howmany sqrRoot: " );
StIO::printFD( howMany2 );
StIO::putLF();

StIO::putS( "Went past loops in findQr." );

return false;
}





bool FindFacQr::testSquare(
                       const Integer& pubKey,
                       const Integer& x,
                       Integer& prime1,
                       Integer& prime2,
                       IntegerMath& intMath )
{
Integer test;
test.copy( x );
intMath.multiply( test, test );
test.add( pubKey );

Integer y;
if( !intMath.squareRoot( test, y ))
  return false;

prime1.copy( y );
intMath.subtract( prime1, x );

prime2.copy( y );
prime2.add( x );

return true;
}



void FindFacQr::makeQrCounts(
                       const Integer& pubKey,
                       IntegerMath& intMath,
                       const SPrimes& sPrimes,
                       const MultInv& multInv,
                       const QuadRes& quadRes )
{
StIO::putS( "makeQrCounts" );

qrCounts.setSize( CrtConst::DigitArraySize );

Crt3 prod;
prod.setFromInteger( pubKey,
                     intMath,
                     sPrimes,
                     multInv,
                     crtMath );

for( Int32 row = 0; row <
            CrtConst::DigitArraySize; row++ )
  {
  Int32 howMany = 0;
  Int32 prime = sPrimes.getPrimeAt( row );
  Int32 prodDigit = prod.crt.getD( row );
  for( Int32 col = 0; col < prime; col++ )
    {
    Int32 x = (col * col) + prodDigit;
    x = x % prime;
    // If the x that it makes is a quad res then
    // the original col value is a goodX.
    if( quadRes.getVal( row, x ))
      howMany++;

    }

  // if( row < 40 )
    // {
    // Str showP( prime );
    // mainIO.appendStr( showP );
    // mainIO.appendChars( ") howMany: " );
    // Str showTest( howMany );
    // mainIO.appendStr( showTest );
    // mainIO.appendChars( "\n" );
    // }

  qrCounts.setVal( row, howMany );
  }
}




void FindFacQr::setupGoodXQuadRes(
                       const Integer& pubKey,
                       const MultInv& multInv,
                       const QuadRes& quadRes,
                       IntegerMath& intMath,
                       const SPrimes& sPrimes )
{
goodX.setAllFalse();

Crt3 prod;
prod.setFromInteger( pubKey,
                     intMath,
                     sPrimes,
                     multInv,
                     crtMath );

for( Int32 row = 0; row <
            CrtConst::DigitArraySize; row++ )
  {
  Int32 prime = sPrimes.getPrimeAt( row );
  Int32 prodDigit = prod.crt.getD( row );
  for( Int32 col = 0; col < prime; col++ )
    {
    Int32 x = (col * col) + prodDigit;
    x = x % prime;
    // If the x that it makes is a quad res then
    // the original col value is a goodX.
    if( quadRes.getVal( row, x ))
      goodX.setVal( row, col, true );

    }
  }

}

*/

