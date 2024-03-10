/*
// Copyright Eric Chauvin, 2022 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#pragma once



#include "../CppBase/BasicTypes.h"
#include "../CppBase/RangeC.h"
#include "../CppBase/FileIO.h"

#include "../CppInt/Integer.h"
#include "../CppInt/IntegerMath.h"
#include "Mod.h"



class TonelliShanks
  {
  private:
  bool testForCopy = false;

  public:
  inline TonelliShanks( void )
    {
    }

  inline TonelliShanks( const TonelliShanks& in )
    {
    if( in.testForCopy )
      return;

    throw "Copy constructor for TonelliShanks.";
    }


  inline ~TonelliShanks( void )
    {
    }

  static bool modRoot( const Integer& A,
                       Integer& result,
                       const Integer& prime,
                       IntegerMath& intMath,
                       Mod& mod );

  static bool modRoot3( const Integer& A,
                        Integer& result,
                        const Integer& prime,
                        IntegerMath& intMath,
                        Mod& mod );

  };
*/

