// Copyright Eric Chauvin, 2022 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#pragma once



#include "../CppBase/BasicTypes.h"
#include "../CppInt/Integer.h"
#include "../CppInt/IntegerMath.h"
#include "../CppInt/Mod.h"
#include "FindFacSm.h"



class MakePrime
  {
  private:
  bool testForCopy = false;
  FindFacSm findFacSm;

  public:
  MakePrime( void )
    {
    }

  MakePrime( const MakePrime& in )
    {
    if( in.testForCopy )
      return;

    throw "Copy constructor: MakePrime.";
    }


  ~MakePrime( void )
    {
    }

  void makeIt( Integer& result,
               const Int32 setToIndex,
               const SPrimes& sPrimes,
               Mod& mod,
               IntegerMath& intMath );


  void removeSmallP( Integer& result,
                     const Int64 maxBase,
                     const SPrimes& sPrimes,
                     IntegerMath& intMath );


  };
