// Copyright Eric Chauvin, 2022 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once


// Find small factors.


#include "../CppBase/BasicTypes.h"
#include "../CppMem/Int32Array.h"
#include "../CppInt/Integer.h"
#include "../CppInt/IntegerMath.h"
#include "SPrimes.h"




class FindFacSm
  {
  private:
  bool testForCopy = false;
  Int32Array smallAr13;
  Int32Array bigAr19;

  public:
  FindFacSm( void )
    {
    makeSmallAr13();
    makeBigAr19();
    }


  FindFacSm( const FindFacSm& in )
    {
    if( in.testForCopy )
      return;

    throw "FindFacSm copy constructor.";
    }

  ~FindFacSm( void )
    {
    }

  void makeSmallAr13( void );
  void makeBigAr19( void );
  Int64 findIt( Integer& pubKey,
                const Int64 maxBase,
                const SPrimes& sPrimes,
                IntegerMath& intMath );

  Int64 findSmall31( Integer& pubKey,
                     const Int64 maxBase,
                     IntegerMath& intMath );

  };
