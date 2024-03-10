// Copyright Eric Chauvin 2022 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html




#include "MakePrime.h"
#include "Fermat.h"
#include "../CppBase/StIO.h"
#include "../CppInt/Division.h"



void MakePrime::makeIt( Integer& result,
                     const Int32 setToIndex,
                     const SPrimes& sPrimes,
                     Mod& mod,
                     IntegerMath& intMath )
{
// StIO::putS( "Making a prime top." );

Integer primeMinus1;

for( Int32 count = 0; count < 10000; count++ )
  {
  Fermat::makeAPrime( result,
                      setToIndex,
                      sPrimes,
                      mod,
                      intMath );

  // Make sure that the prime minus 1 doesn't
  // have all small factors.

  // StIO::putS( "Before primeMinus1." );

  primeMinus1.copy( result );
  primeMinus1.decrement();

  // StIO::putS( "Checking for small primes." );

  // Check for small factors up to 
  // how many?
  removeSmallP( primeMinus1, 10000,
                sPrimes, intMath );

  // StIO::putS( "Done checking small primes." );

  // What is left of primeMinus1?

  // CharBuf showBuf;
  // StIO::putS( "P - 1 after divides:" );
  // intMath.toString10( primeMinus1, showBuf );
  // StIO::putCharBuf( showBuf );
  // StIO::putLF();

  if( !primeMinus1.isOne() )
    {
    // StIO::putS( "It has something left." );
    return;
    }

  // if( some other test )

  }
}



void MakePrime::removeSmallP( Integer& result,
                        const Int64 maxBase,
                        const SPrimes& sPrimes,
                        IntegerMath& intMath )
{
// StIO::putS( "RemoveSmallP top." );

for( Int32 count = 0; count < 10000; count++ )
  {
  if( (result.getD( 0 ) & 1) == 1)
    break;

  result.shiftRight( 1 ); // divide by 2.
  }

Integer smallFac;
Integer quotient;
Integer remainder;

for( Int32 count = 0; count < 10000; count++ )
  {
  Int32 testPrime =
       intMath.isDivisibleBySmallPrime(
                             result, sPrimes );
  if( 0 == testPrime)
    {
    // StIO::putS( "No more small factors." );
    break;
    }

  // StIO::putS( "Removing small factor:" );
  // StIO::printFD( testPrime );
  // StIO::putLF();

  smallFac.setFromLong48( testPrime );

  Division::divide( result,
                    smallFac,
                    quotient,
                    remainder,
                    intMath );

  // StIO::putS( "After divide." );

  result.copy( quotient );
  if( result.isOne() )
    {
    // StIO::putS( "P - 1 had all small factors." );
    return;
    }
  }

// StIO::putS( "Before findFacSmall." );

for( Int32 count = 0; count < 10000; count++ )
  {
  Int64 testFac = findFacSm.findIt( result,
                                    maxBase,
                                    sPrimes,
                                    intMath );

  if( 0 == testFac )
    {
    StIO::putS( "No more small-64 factors." );
    break;
    }

  StIO::putS( "Removing small-64 factor:" );
  StIO::printFD( testFac );
  StIO::putLF();

  smallFac.setFromLong48( testFac );

  Division::divide( result,
                    smallFac,
                    quotient,
                    remainder,
                    intMath );

  result.copy( quotient );
  if( result.isOne() )
    {
    StIO::putS( "P - 1 had all small factors." );
    return;
    }
  }

// StIO::putS( "End of removeSmallP." );

}
