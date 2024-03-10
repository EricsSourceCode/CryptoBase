/*

// Copyright Eric Chauvin 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once



#include "../CppBase/BasicTypes.h"
#include "../CppBase/CharBuf.h"

#include "../CppInt/Integer.h"
#include "../CppInt/IntegerMath.h"
#include "Mod.h"



// Pollard's P - 1 algorithm.  From 1974.
// If P - 1 has small factors, then this finds
// the factors of P.



class Pollard
  {
  private:
  bool testForCopy = false;

  public:
  inline Pollard( void )
    {
    }

  inline Pollard( const Pollard& in )
    {
    if( in.testForCopy )
      return;

    throw "Pollard copy constructor called.";
    }

  inline ~Pollard( void )
    {
    }

  bool findFactors( Integer& N,
                    // const SPrimes& sPrimes,
                    IntegerMath& intMath,
                    Mod& mod );



  };

*/
