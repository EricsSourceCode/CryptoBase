/*

// Copyright Eric Chauvin 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "Pollard.h"
#include "Euclid.h"
#include "../CppBase/StIO.h"



bool Pollard::findFactors( Integer& N,
                        // const SPrimes& sPrimes,
                        IntegerMath& intMath,
                        Mod& mod )
{
// This is just the most basic type of Pollard
// factoring algorithm.  There are better,
// more optimized versions of it.


StIO::putS( "Pollard start." );

Integer A;
Integer aMinus1;
Integer I;
Integer gcd;
CharBuf showBuf;


A.setFromLong48( 2 );
for( Int32 i = 2; i < 10000; i++ )
  {
  I.setFromLong48( i );
  mod.toPower( A, I, N, intMath );
  // StIO::putS( "A is: " );
  // intMath.toString10( A, showBuf );
  // StIO::putCharBuf( showBuf );
  // StIO::putLF();

  aMinus1.copy( A );
  intMath.subLong48( aMinus1, 1 );

  Euclid::greatestComDiv( aMinus1, N, gcd,
                          intMath );

  if( !gcd.isOne())
    {
    StIO::putS( "One factor is: " );
    intMath.toString10( gcd, showBuf );
    StIO::putCharBuf( showBuf );
    StIO::putLF();
    return true;
    }
  }

StIO::putS( "Did not find factor." );
StIO::putS( "End of Pollard test." );
return false;
}

*/
