/*

// Copyright Eric Chauvin 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#include "Reduce112.h"
// #include "../CppInt/Division.h"


//////////
void NumbSys::setupBaseArray(
                        const Integer& setBase,
                        IntegerMath& intMath )
{
currentBase.copy( setBase );

Integer base2;   // 0x1 00 00 00
base2.setFromLong48( 0x1000000L );

Integer baseValue;
Integer quotient;
Integer remainder;

baseValue.setToOne();

for( Int32 count = 0; count < last; count++ )
  {
  Division::divide( baseValue, currentBase,
                    quotient, remainder,
                    intMath );

  // setOneBaseFromInt( count, remainder );
  remainder.copyToIntBuf( intBufAr[count] );

  // Done at the bottom for the next round of
  // the loop.
  baseValue.copy( remainder );
  intMath.multiply( baseValue, base2 );
  }
}



void NumbSys::reduce( Integer& result,
                      const Integer& toReduce,
                      const Integer& modulus,
                      IntegerMath& intMath )
{
// currentBase would start out being set to
// zero, so it has to be set the first time
// it gets called.
if( !modulus.isEqual( currentBase ))
  setupBaseArray( modulus, intMath );

if( toReduce.paramIsGreater( currentBase ))
  {
  result.copy( toReduce );
  return;
  }

numVal.copy( toReduce );
const Int32 ind = numVal.getIndex();

Integer accumRow;

result.setToZero();
for( Int32 row = 0; row <= ind; row++ )
  {
  Int64 val = numVal.getD( row );
  const Int32 lastBase = intBufAr[row].
                           getIndex();

  accumRow.setIndex( lastBase );
  for( Int32 column = 0; column <= lastBase;
                                       column++ )
    {
    Int64 baseVal = intBufAr[row].getD( column );
    Int64 calc = baseVal * val;
    accumRow.setD( column, calc );
    }

  accumRow.cleanUp();
  result.add( accumRow );
  }
}

*/
