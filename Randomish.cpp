// Copyright Eric Chauvin 2022 - 2024.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "Randomish.h"
#include "../CppBase/Casting.h"


#include <random>



// RFC 4086 Randomness Requirements for Security


void Randomish::makeRandomBytes(
                            CharBuf& cBuf,
                            const Int32 howMany )
{
cBuf.clear();

// This is pretty crude at this point.

// The hardware source of entropy.
std::random_device rd;

// rd.entropy()
// rd.min()
// rd.max()

// std::mersenne_twister_engine
// std::mt19937_64
// Mersenne twister engine
// Mersenne as in Mersenne prime.
// 19937 is the exponent for a Mersenne prime.


std::mt19937 gen( rd() );

Int32 where = 0;
for( Int32 count = 0; count < 100000; count++ )
  {
  // dist( gen())

  Uint32 rBytes = gen();

  cBuf.appendU32( rBytes );
  where += 4;
  if( where > howMany )
    return;

  }
}


Float32 Randomish::makeRandomFloat32( void )
{
// This is crude.

CharBuf cBuf;
Int32 bytes = 0;
makeRandomBytes( cBuf, 10 );
bytes = cBuf.getU8( 0 );
bytes <<= 8;
bytes |= cBuf.getU8( 1 );
bytes <<= 8;
bytes |= cBuf.getU8( 2 );

Float32 result = static_cast<Float32>( bytes );
return result;
}
