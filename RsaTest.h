// Copyright Eric Chauvin, 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once



#include "../CppBase/BasicTypes.h"
#include "../CppInt/Integer.h"
#include "../CppInt/IntegerMath.h"
#include "../CppInt/Mod.h"
#include "SPrimes.h"
#include "Rsa.h"


class RsaTest
  {
  private:
  bool testForCopy = false;

  // The size of the primes.
  // 24 times 10 is 240 bits.

  const Int32 testIndex = 10;

  // 3 to 4 seconds.
  // const Int32 testIndex = 20;

  // 5 to 6 seconds.
  // const Int32 testIndex = 30;

  // 9 to 13 seconds.
  // const Int32 testIndex = 35;

  // Index of 43 is about a 1024 bit prime.
  // 11 to 16 seconds.
  // const Int32 testIndex = 40;

  // 14 to 15 seconds
  // const Int32 testIndex = 45;

  // 24 to 26 seconds.
  // const Int32 testIndex = 55;

  // 35 to 39
  // const Int32 testIndex = 60;

  // 41 to 45
  // const Int32 testIndex = 70;

  // 83 to 85
  // const Int32 testIndex = 80;

  // 124 to 206 seconds
  // const Int32 testIndex = 90;

  // Around 2,800 bits per prime.
  // 170 to 195 seconds.
  // const Int32 testIndex = 100;


  public:
  RsaTest( void )
    {
    }


  RsaTest( const RsaTest& in )
    {
    if( in.testForCopy )
      return;

    throw "Copy constructor for RsaTest.";
    }

  ~RsaTest( void )
    {
    }

  void test( Rsa& rsa, Mod& mod,
             SPrimes& sPrimes,
             IntegerMath& intMath,
             FindFacSm& findFacSm,
             // FindFacQr& findFacQr,
             const MultInv& multInv,
             // const QuadRes& quadRes,
             const CrtMath& crtMath,
             GarnerCrt& garnerCrt );

  };
