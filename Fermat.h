// Copyright Eric Chauvin 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once



#include "../CppBase/BasicTypes.h"
#include "../CppInt/Integer.h"
#include "../CppInt/IntegerMath.h"
#include "SPrimes.h"
#include "../CppInt/Mod.h"



class Fermat
  {
  private:
  // bool testForCopy = false;

  public:

  static void makeAPrime( Integer& result,
                   const Int32 setToIndex,
                   const SPrimes& sPrimes,
                   Mod& mod,
                   IntegerMath& intMath );


  static bool isPrime( Integer& toTest,
                const SPrimes& sPrimes,
                Mod& mod,
                IntegerMath& intMath );

  static bool isPrimeForOneValue(
                      Integer& toTest,
                      const Int64 base,
                      Mod& mod,
                      IntegerMath& intMath );

  };
