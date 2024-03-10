// Copyright Eric Chauvin 2022 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#include "FindFacSm.h"
#include "../CppBase/StIO.h"



Int64 FindFacSm::findIt( Integer& pubKey,
                    const Int64 maxBase,
                    const SPrimes& sPrimes,
                    IntegerMath& intMath )
{
Int32 smallP = intMath.isDivisibleBySmallPrime(
                             pubKey, sPrimes );
if( smallP != 0 )
  return smallP;

Int64 result =
      findSmall31( pubKey, maxBase, intMath );

return result;
}




void FindFacSm::makeSmallAr13( void )
{
// Euler's Phi function.
const Int32 phi = (3 - 1) * (5 - 1) * (7 - 1) *
                          (11 - 1) * (13 - 1);

// Phi 13 is: 5760

smallAr13.setSize( phi );

const Int32 max = 2 * 3 * 5 * 7 * 11 * 13;
Int32 howMany = 0;

for( Int32 count = 1; count < max; count += 2 )
  {
  if( (count % 3) == 0 )
    continue;

  if( (count % 5) == 0 )
    continue;

  if( (count % 7) == 0 )
    continue;

  if( (count % 11) == 0 )
    continue;

  if( (count % 13) == 0 )
    continue;

  smallAr13.setVal( howMany, count );
  howMany++;
  }

if( howMany != phi )
  throw "howMany != phi for 13";

}



void FindFacSm::makeBigAr19( void )
{
// Euler's Phi function.
const Int32 phi = (3 - 1) * (5 - 1) * (7 - 1) *
                          (11 - 1) * (13 - 1) *
                          (17 - 1) * (19 - 1);

// Phi 13 is: 5760
// Phi 19 is: 1658880
// Max 19 is: 9699690

bigAr19.setSize( phi );

// const Int32 max = 2 * 3 * 5 * 7 * 11 *
//     13 * 17 * 19;

Int32 howMany = 0;

Int32 bigBase = 2 * 3 * 5 * 7 * 11 * 13;

const Int32 last13 = smallAr13.getSize();

for( Int32 countBase = 0; countBase < 17 * 19;
                                       countBase++ )
  {
  Int32 basePart = bigBase * countBase;

  for( Int32 count = 0; count < last13; count++ )
    {
    Int32 allParts = basePart + smallAr13.
                                 getVal( count );


    if( (allParts % 17) == 0 )
      continue;

    if( (allParts % 19) == 0 )
      continue;

    bigAr19.setVal( howMany, allParts );
    howMany++;
    }
  }

if( howMany != phi )
  throw "howMany != phi for big 19";

}


Int64 FindFacSm::findSmall31(
                         Integer& pubKey,
                         const Int64 maxBase,
                         IntegerMath& intMath )
{
// const Int64 max = 2LL * 3LL * 5LL * 7LL *
//       11LL * 13LL * 17LL * 19LL * 23LL *
//       29LL * 31LL;

// Max 29 is:   6,469,693,230
// Max 31 is: 200,560,490,130

// Int64 howMany = 0;

Int64 bigBase = 2L * 3L * 5L * 7L * 11L *
                             13L * 17L * 19L;

const Int32 last19 = bigAr19.getSize();

for( Int32 countBase = 0;
                    countBase < 23 * 29 * 31;
                    countBase++ )
  {
  Int64 basePart = bigBase * countBase;

  // Limit the search to not go higher than this.
  if( basePart > maxBase )
    return 0;

  Int32 startAt = 0;
  if( countBase == 0 )
    startAt = 1; // Skip the number 1.

  for( Int32 count = startAt; count < last19;
                                        count++ )
    {
    Int64 allParts = basePart +
                          bigAr19.getVal( count );
    // if( allParts == 1 )
      // continue;

    if( (allParts % 23) == 0 )
      continue;

    if( (allParts % 29) == 0 )
      continue;

    if( (allParts % 31) == 0 )
      continue;

    if( (allParts >> 56 ) != 0 )
      return 0;

    Int64 modTest = intMath.getMod24(
                         pubKey, allParts );
    if( modTest == 0 )
      return allParts;

    }
  }

return 0;
}
