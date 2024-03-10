// Copyright Eric Chauvin 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



// #include "../CppBase/Casting.h"

#include "SPrimes.h"
#include "../CppInt/IntegerMath.h"
#include "../CppBase/StIO.h"



Int32 SPrimes::getFirstFactor(
                       const Int64 toTest ) const
{
if( toTest < 2 )
  return 0;

if( toTest == 2 )
  return 2;

if( toTest == 3 )
  return 3;

const Int32 max = Casting::i64ToI32(
         IntegerMath::find64SqrRoot( toTest ));

RangeC::test2( max, 1, Integer::Int24BitMask,
        "SPrimes.getFirstFactor Max range." );

for( Int32 count = 0; count < last; count++ )
  {
  Int32 testN = pArray.getVal( count );

  if( testN > max )
    return 0;

  if( (toTest % testN) == 0 )
    return testN;

  }

return 0;
}




void SPrimes::makeArray( void )
{
StIO::putS( "SPrimes.makeArray()." );

pArray.setSize( last );

pArray.setVal( 0, 2 );
pArray.setVal( 1, 3 );
pArray.setVal( 2, 5 );
pArray.setVal( 3, 7 );
pArray.setVal( 4, 11 );
pArray.setVal( 5, 13 );
pArray.setVal( 6, 17 );
pArray.setVal( 7, 19 );
pArray.setVal( 8, 23 );

Int32 index = 9;
for( Int32 testN = 29; ; testN += 2 )
  {
  if( (testN % 3) == 0 )
    continue;

  // If it has no prime factors then add it.
  if( 0 == getFirstFactor( testN ))
    {
    // StIO::printFD( testN );
    // StIO::putLF();

    pArray.setVal( index, testN );
    index++;
    if( index >= last )
      return;

    }
  }
}
