/*

// Copyright Eric Chauvin 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once


#include "../CppBase/BasicTypes.h"
#include "../CppInt/Int112.h"



class Reduce112
  {
  private:
  bool testForCopy = false;
  // static const Int32 last =
  //                 Integer::digitArraySize;

  Int112 currentBase;
  Int112 base0; // For N0
  Int112 base1; // For N1

  // Integer numVal;
  // IntBuf* intBufAr;

  // void setupBaseArray( const Integer& setBase,
  //                     IntegerMath& intMath );

  public:
  Reduce112( void )
    {
    }

  Reduce112( const Reduce112& in )
    {
    if( in.testForCopy )
      return;

    throw "Copy constructor for Reduce112.";
    }

  ~Reduce112( void )
    {
    }

  //   void reduce( Integer& result,
  //             const Integer& toReduce,
  //             const Integer& modulus,
  //             IntegerMath& intMath );

  };
*/
