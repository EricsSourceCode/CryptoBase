// Copyright Eric Chauvin 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "RsaTest.h"
#include "MakePrime.h"
#include "../CppBase/StIO.h"
#include "../CppBase/TimeApi.h"
#include "../CppInt/NumbSysFull.h"



void RsaTest::test( Rsa& rsa,
                    Mod& mod,
                    SPrimes& sPrimes,
                    IntegerMath& intMath,
                    FindFacSm& findFacSm,
                    // FindFacQr& findFacQr,
                    const MultInv& multInv,
                    // const QuadRes& quadRes,
                    const CrtMath& crtMath,
                    GarnerCrt& garnerCrt )
{
StIO::putS( "Top of RSA test." );

TimeApi t1;

Integer prime1;
Integer prime2;

MakePrime makePrime;

for( Int32 count = 0; count < 2; count++ )
  {
  makePrime.makeIt( prime1,
                    testIndex,
                    sPrimes,
                    mod,
                    intMath );

  makePrime.makeIt( prime2,
                    testIndex,
                    sPrimes,
                    mod,
                    intMath );

  StIO::putS( "Before RSA makeKeys." );

  if( rsa.makeKeys( prime1,
                    prime2,
                    intMath,
                    mod,
                    sPrimes,
                    findFacSm,
                    // findFacQr,
                    multInv,
                    // quadRes,
                    crtMath,
                    garnerCrt ))
    break;

  }

///////////
// For testing.
// NumbSysFull numbSysFull;
// numbSysFull.setupBaseArray( prime1, intMath );


TimeApi t2;

Int64 diff = t2.diffSec( t1 );

CharBuf showTime( diff );
StIO::printF( "Seconds: " );
StIO::putCharBuf( showTime );
StIO::putLF();

StIO::putS( "Finished test." );
}
