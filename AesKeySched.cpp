// Copyright Eric Chauvin 2022.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "AesKeySched.h"
#include "AesRCon.h"
// #include "../CppBase/ByteHex.h"



void AesKeySched::keyExpansion(
                          const AesSBox& aesSBox )
{
// This is the only place these round constants
// are used.
AesRCon aesRCon;

// Makes a total of Nb(Nr + 1) words.

// Using these variable names to match up with
// the specs in the FIPS document.

// Nb = Number of columns, 32 bit words, in
// the state.  It is always 4.
// const Int32 Nb = 4;

// Nk = Number of 32 bit words in the key.
// It is 4, 6 or 8.
const Int32 Nk = keyLengthInBytes / 4;

// Nr = Number of rounds.
// const Int32 Nr = numberOfRounds;

for( Int32 count = 0; count < Nk; count++ )
  {
  // The four bytes: 0x60, 0x3d, 0xeb, 0x10,
  // become the uint: w0 = 603deb10
  Uint32 theWord = makeUintFromBytes(
                 getKeyByte( 4 * count ),
                 getKeyByte( (4 * count) + 1 ),
                 getKeyByte( (4 * count) + 2 ),
                 getKeyByte( (4 * count) + 3 ));

  setWord( count, theWord );
  }

// max = 4 * (numberOfRounds + 1)
const Int32 max = keyWordsSize;
for( Int32 count = Nk; count < max; count++ )
  {
  Uint32 temp = getWord( count - 1 );

  if( (count % Nk) == 0) // mod 4, 6 or 8.
    {
    Uint32 rConVal = aesRCon.getV( count / Nk );
    temp = rotateWord( temp );
    temp = aesSBox.subWord( temp );

    // The little end is on the left, so the
    // left shift brings the small RCon number
    // to the small side of the word.

    temp = temp ^ (rConVal << 24);
    }


  // The above if-statement can't be true if
  // this is true.  It is an else-if.
  // if Nk is > 6 then this is a 256 bit key.
  if( (Nk > 6) && ((count % Nk) == 4) )
    {
    temp = aesSBox.subWord( temp );
    }

  Uint32 prevWord = getWord( count - Nk );
  Uint32 toSet = prevWord ^ temp;
  setWord( count, toSet );
  }

// showAll( max );
}



void AesKeySched::setKey( const CharBuf& theKey,
                          const Int32 keySize,
                          const AesSBox& aesSBox )
{
// 10 rounds for 128 bit key,
// 12 rounds for 192,
// 14 rounds for 256 bit key.

// Not using 192 bit key size.
if( !((keySize == 16) || (keySize == 32)) )
  throw "Aes key size has to be 16 or 32.";

if( keySize == 16 )
  {
  keyLengthInBytes = 16; // 128 / 8
  numberOfRounds = 10;
  }

if( keySize == 32 )
  {
  keyLengthInBytes = 32; // 256 / 8
  numberOfRounds = 14;
  }

keyWordsSize = 4 * (numberOfRounds + 1);

// This is redundant, but it verifies it.
if( keySize != theKey.getLast())
  throw "Aes keySize doesn't match.";

for( Int32 count = 0; count < keySize; count++ )
  setKeyByte( count, theKey.getU8( count ));

keyExpansion( aesSBox );

keyIsSet = true;
}


/*
void AesKeySched::showAll( const Int32 howMany )
{
for( Int32 count = 0; count < howMany; count++ )
  {
  StIO::printFD( count );
  StIO::printF( ") " );
  ByteHex::showUint32( getWord( count ));
  StIO::putLF();
  }
}
*/
