/*

// Copyright Eric Chauvin, 2022 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#pragma once


// An optimized version of Fermat's method
// of factoring.



#include "../CppBase/BasicTypes.h"
#include "../CppBase/StIO.h"
#include "../CppMem/Int32Array.h"
#include "../CppMem/Int64Array.h"
#include "../CppInt/Integer.h"
#include "../CppInt/IntegerMath.h"
#include "SPrimes.h"
#include "QuadRes.h"
#include "../Crt/MultInv.h"
#include "../Crt/GoodX.h"
#include "../Crt/CrtMath.h"



class FindFacQr
  {
  private:
  bool testForCopy = false;
  Int32 smallAr17Last = 0;
  Int32 bigAr23Last = 0;
  Int32 bigAr29Last = 0;

  Int32Array smallAr17;
  Int32Array bigAr23;
  Int64Array bigAr29;
  Int32Array qrCounts;

  GoodX goodX;
  // CrtMath crtMath; In MainApp?

  public:
  FindFacQr( void );
  FindFacQr( const FindFacQr& in );
  ~FindFacQr( void );

  void init( IntegerMath& intMath,
             const SPrimes& sPrimes );

  inline bool testTopByte(
                   const Int32 basePartByte,
                   const Int64 val23,
                   const Int32 prodByte )
    {
    Int32 allBytes = basePartByte +
                               (val23 & 0xFF);
    allBytes = allBytes * allBytes;
    allBytes += prodByte;
    // allBytes = allBytes & 0xFF;
    return QuadRes::bytesQR( allBytes );
    }

  void setupGoodXQuadRes(
                    const Integer& pubKey,
                    const MultInv& multInv,
                    const QuadRes& quadRes,
                    IntegerMath& intMath,
                    const SPrimes& sPrimes );




/////////////
  inline bool isGoodX( const Int64 x,
                       const Int32 pKeyMod,
                       const Int32 prime,
                       const Int32 primeAt )
    {
    Int64 test = x % prime;
    test = test * test;
    test += pKeyMod;
    test = test % prime;

    // return quadRes.getVal( primeAt, Casting::i64ToI32( test ));
    //                                   0x 0FFF FFFF
    return quadRes.getVal( primeAt, test & 0x0FFFFFFF );
    }
/////////////




  void makeSmallAr17( void );
  void makeBigAr23( void );
  void makeBigAr29( void );

  bool findIt( const Integer& pubKey,
               Integer& prime1,
               Integer& prime2,
               IntegerMath& intMath,
               const SPrimes& sPrimes,
               const MultInv& multInv,
               const QuadRes& quadRes );

  bool findQr31( const Integer& pubKey,
                 Integer& prime1,
                 Integer& prime2,
                 IntegerMath& intMath );

  bool testSquare( const Integer& pubKey,
                   const Integer& x,
                   Integer& prime1,
                   Integer& prime2,
                   IntegerMath& intMath );

  void makeQrCounts( const Integer& pubKey,
                     IntegerMath& intMath,
                     const SPrimes& sPrimes,
                     const MultInv& multInv,
                     const QuadRes& quadRes );

  };

*/

