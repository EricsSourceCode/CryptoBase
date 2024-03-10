// Copyright Eric Chauvin 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once



#include "../CppBase/BasicTypes.h"
#include "../CppMem/Int32Array.h"



class SPrimes
  {
  public:
  static const Int32 primesArraySize = 1024 * 32;

  private:
  bool testForCopy = false;

  Int32Array pArray;
  void makeArray( void );

  static const Int32 last = primesArraySize;

  public:
  SPrimes( void )
    {
    makeArray();
    }


  SPrimes( const SPrimes& in )
    {
    if( in.testForCopy )
      return;

    throw "Copy constructor for SPrimes.";
    }


  ~SPrimes( void )
    {
    }


  Int32 getFirstFactor(
                  const Int64 toTest ) const;

  inline Int32 getPrimeAt(
                   const Int32 where ) const
    {
    return pArray.getVal( where );
    }

  inline Int32 getBiggestPrime( void ) const
    {
    return pArray.getVal( last - 1 );
    }

  };
