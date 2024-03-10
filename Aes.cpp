// Copyright Eric Chauvin 2022 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "Aes.h"
#include "../CppBase/ByteHex.h"



void Aes::addRoundKey( Int32 round )
{
// Nb = Number of columns, 32 bit words, in
// the state.  It is always 4.

// "Each round key consists of Nb words from
// the key schedule."
// That is 4 words.
Int32 where = round * 4;
Uint32 keyWord0 = aesKeySched.getWord( where );
Uint32 keyWord1 = aesKeySched.getWord( where + 1 );
Uint32 keyWord2 = aesKeySched.getWord( where + 2 );
Uint32 keyWord3 = aesKeySched.getWord( where + 3 );

// Take each byte from this one keyword down
// one column.

Uint8 keyByte = (keyWord0 >> 24) & 0xFF;
//                           row, column
Uint8 newByte = aesState.getV( 0, 0 ) ^ keyByte;
aesState.setV( 0, 0, newByte );

keyByte = (keyWord0 >> 16) & 0xFF;
newByte = aesState.getV( 1, 0 ) ^ keyByte;
aesState.setV( 1, 0, newByte );

keyByte = (keyWord0 >> 8) & 0xFF;
newByte = aesState.getV( 2, 0 ) ^ keyByte;
aesState.setV( 2, 0, newByte );

keyByte = keyWord0 & 0xFF;
newByte = aesState.getV( 3, 0 ) ^ keyByte;
aesState.setV( 3, 0, newByte );


keyByte = (keyWord1 >> 24) & 0xFF;
newByte = aesState.getV( 0, 1 ) ^ keyByte;
aesState.setV( 0, 1, newByte );


keyByte = (keyWord1 >> 16) & 0xFF;
newByte = aesState.getV( 1, 1 ) ^ keyByte;
aesState.setV( 1, 1, newByte );

keyByte = (keyWord1 >> 8) & 0xFF;
newByte = aesState.getV( 2, 1 ) ^ keyByte;
aesState.setV( 2, 1, newByte );

keyByte = keyWord1 & 0xFF;
newByte = aesState.getV( 3, 1 ) ^ keyByte;
aesState.setV( 3, 1, newByte );


keyByte = (keyWord2 >> 24) & 0xFF;
newByte = aesState.getV( 0, 2 ) ^ keyByte;
aesState.setV( 0, 2, newByte );

keyByte = (keyWord2 >> 16) & 0xFF;
newByte = aesState.getV( 1, 2 ) ^ keyByte;
aesState.setV( 1, 2, newByte );

keyByte = (keyWord2 >> 8) & 0xFF;
newByte = aesState.getV( 2, 2 ) ^ keyByte;
aesState.setV( 2, 2, newByte );

keyByte = keyWord2 & 0xFF;
newByte = aesState.getV( 3, 2 ) ^ keyByte;
aesState.setV( 3, 2, newByte );


keyByte = (keyWord3 >> 24) & 0xFF;
newByte = aesState.getV( 0, 3 ) ^ keyByte;
aesState.setV( 0, 3, newByte );

keyByte = (keyWord3 >> 16) & 0xFF;
newByte = aesState.getV( 1, 3 ) ^ keyByte;
aesState.setV( 1, 3, newByte );

keyByte = (keyWord3 >> 8) & 0xFF;
newByte = aesState.getV( 2, 3 ) ^ keyByte;
aesState.setV( 2, 3, newByte );

keyByte = keyWord3 & 0xFF;
newByte = aesState.getV( 3, 3 ) ^ keyByte;
aesState.setV( 3, 3, newByte );
}



void Aes::subBytes( void )
{
aesState.setV( 0, 0, aesSBox.subByte(
                     aesState.getV( 0, 0 )));
aesState.setV( 1, 0, aesSBox.subByte(
                     aesState.getV( 1, 0 )));
aesState.setV( 2, 0, aesSBox.subByte(
                     aesState.getV( 2, 0 )));
aesState.setV( 3, 0, aesSBox.subByte(
                     aesState.getV( 3, 0 )));

aesState.setV( 0, 1, aesSBox.subByte(
                     aesState.getV( 0, 1 )));
aesState.setV( 1, 1, aesSBox.subByte(
                     aesState.getV( 1, 1 )));
aesState.setV( 2, 1, aesSBox.subByte(
                     aesState.getV( 2, 1 )));
aesState.setV( 3, 1, aesSBox.subByte(
                     aesState.getV( 3, 1 )));

aesState.setV( 0, 2, aesSBox.subByte(
                     aesState.getV( 0, 2 )));
aesState.setV( 1, 2, aesSBox.subByte(
                     aesState.getV( 1, 2 )));
aesState.setV( 2, 2, aesSBox.subByte(
                     aesState.getV( 2, 2 )));
aesState.setV( 3, 2, aesSBox.subByte(
                     aesState.getV( 3, 2 )));

aesState.setV( 0, 3, aesSBox.subByte(
                     aesState.getV( 0, 3 )));
aesState.setV( 1, 3, aesSBox.subByte(
                     aesState.getV( 1, 3 )));
aesState.setV( 2, 3, aesSBox.subByte(
                     aesState.getV( 2, 3 )));
aesState.setV( 3, 3, aesSBox.subByte(
                     aesState.getV( 3, 3 )));
}




void Aes::shiftRows( void )
{
AesState tempState;

// The first row is unchanged.
tempState.setV( 0, 0, aesState.getV( 0, 0 ));
tempState.setV( 0, 1, aesState.getV( 0, 1 ));
tempState.setV( 0, 2, aesState.getV( 0, 2 ));
tempState.setV( 0, 3, aesState.getV( 0, 3 ) );

//                                 row, column
tempState.setV( 1, 0, aesState.getV( 1, 1 ));
tempState.setV( 1, 1, aesState.getV( 1, 2 ));
tempState.setV( 1, 2, aesState.getV( 1, 3 ));
tempState.setV( 1, 3, aesState.getV( 1, 0 ) );

tempState.setV( 2, 0, aesState.getV( 2, 2 ));
tempState.setV( 2, 1, aesState.getV( 2, 3 ));
tempState.setV( 2, 2, aesState.getV( 2, 0 ));
tempState.setV( 2, 3, aesState.getV( 2, 1 ));

tempState.setV( 3, 0, aesState.getV( 3, 3 ));
tempState.setV( 3, 1, aesState.getV( 3, 0 ));
tempState.setV( 3, 2, aesState.getV( 3, 1 ));
tempState.setV( 3, 3, aesState.getV( 3, 2 ));

aesState.copy( tempState );
}




void Aes::mixColumns( void )
{
AesState tempState;

Uint8 val =
  Polynom8::multiply( 0x02, aesState.getV( 0, 0 )) ^
  Polynom8::multiply( 0x03, aesState.getV( 1, 0 )) ^
  aesState.getV( 2, 0 ) ^
  aesState.getV( 3, 0 );

tempState.setV( 0, 0, val );


val = aesState.getV( 0, 0 ) ^
  Polynom8::multiply( 0x02, aesState.getV( 1, 0 )) ^
  Polynom8::multiply( 0x03, aesState.getV( 2, 0 )) ^
  aesState.getV( 3, 0 );

tempState.setV( 1, 0, val );


val = aesState.getV( 0, 0 ) ^
      aesState.getV( 1, 0 ) ^
 Polynom8::multiply( 0x02, aesState.getV( 2, 0 )) ^
 Polynom8::multiply( 0x03, aesState.getV( 3, 0 ));

tempState.setV( 2, 0, val );


val =
  Polynom8::multiply( 0x03, aesState.getV( 0, 0 )) ^
  aesState.getV( 1, 0 ) ^
  aesState.getV( 2, 0 ) ^
  Polynom8::multiply( 0x02, aesState.getV( 3, 0 ));

tempState.setV( 3, 0, val );



val =
  Polynom8::multiply( 0x02, aesState.getV( 0, 1 )) ^
  Polynom8::multiply( 0x03, aesState.getV( 1, 1 )) ^
  aesState.getV( 2, 1 ) ^
  aesState.getV( 3, 1 );

tempState.setV( 0, 1, val );


val =
  aesState.getV( 0, 1 ) ^
  Polynom8::multiply( 0x02, aesState.getV( 1, 1 )) ^
  Polynom8::multiply( 0x03, aesState.getV( 2, 1 )) ^
  aesState.getV( 3, 1 );

tempState.setV( 1, 1, val );


val = aesState.getV( 0, 1 ) ^
      aesState.getV( 1, 1 ) ^
  Polynom8::multiply( 0x02, aesState.getV( 2, 1 )) ^
  Polynom8::multiply( 0x03, aesState.getV( 3, 1 ));

tempState.setV( 2, 1, val );


val =
  Polynom8::multiply( 0x03, aesState.getV( 0, 1 )) ^
  aesState.getV( 1, 1 ) ^
  aesState.getV( 2, 1 ) ^
  Polynom8::multiply( 0x02, aesState.getV( 3, 1 ));

tempState.setV( 3, 1, val );




val =
  Polynom8::multiply( 0x02, aesState.getV( 0, 2 )) ^
  Polynom8::multiply( 0x03, aesState.getV( 1, 2 )) ^
  aesState.getV( 2, 2 ) ^
  aesState.getV( 3, 2 );

tempState.setV( 0, 2, val );


val = aesState.getV( 0, 2 ) ^
  Polynom8::multiply( 0x02, aesState.getV( 1, 2 )) ^
  Polynom8::multiply( 0x03, aesState.getV( 2, 2 )) ^
  aesState.getV( 3, 2 );

tempState.setV( 1, 2, val );


val = aesState.getV( 0, 2 ) ^
      aesState.getV( 1, 2 ) ^
  Polynom8::multiply( 0x02, aesState.getV( 2, 2 )) ^
  Polynom8::multiply( 0x03, aesState.getV( 3, 2 ));

tempState.setV( 2, 2, val );


val =
  Polynom8::multiply( 0x03, aesState.getV( 0, 2 )) ^
  aesState.getV( 1, 2 ) ^
  aesState.getV( 2, 2 ) ^
  Polynom8::multiply( 0x02, aesState.getV( 3, 2 ));

tempState.setV( 3, 2, val );



val =
  Polynom8::multiply( 0x02, aesState.getV( 0, 3 )) ^
  Polynom8::multiply( 0x03, aesState.getV( 1, 3 )) ^
  aesState.getV( 2, 3 ) ^
  aesState.getV( 3, 3 );

tempState.setV( 0, 3, val );


val = aesState.getV( 0, 3 ) ^
  Polynom8::multiply( 0x02, aesState.getV( 1, 3 )) ^
  Polynom8::multiply( 0x03, aesState.getV( 2, 3 )) ^
  aesState.getV( 3, 3 );

tempState.setV( 1, 3, val );


val = aesState.getV( 0, 3 ) ^
      aesState.getV( 1, 3 ) ^
  Polynom8::multiply( 0x02, aesState.getV( 2, 3 )) ^
  Polynom8::multiply( 0x03, aesState.getV( 3, 3 ));

tempState.setV( 2, 3, val );


val =
  Polynom8::multiply( 0x03, aesState.getV( 0, 3 )) ^
  aesState.getV( 1, 3 ) ^
  aesState.getV( 2, 3 ) ^
  Polynom8::multiply( 0x02, aesState.getV( 3, 3 ));

tempState.setV( 3, 3, val );

aesState.copy( tempState );
}



void Aes::encryptLoop( void )
{
addRoundKey( 0 );

const Int32 numberOfRounds = aesKeySched.
                         getNumberOfRounds();

// This loop is what needs to be optimized.
for( Int32 round = 1; round < numberOfRounds;
                                      round++ )
  {
  subBytes();
  shiftRows();
  mixColumns();
  addRoundKey( round );
  }

// Do the last part without the mixColumns():

subBytes();
shiftRows();
addRoundKey( numberOfRounds );
}




void Aes::encryptCharBuf( const CharBuf& inBuf,
                          CharBuf& outBuf,
                          const Int32 where )
{
aesState.moveCharBufToState( inBuf, where );
// aesState.showState();

encryptLoop();

aesState.moveStateToCharBuf( outBuf );
}



void Aes::encryptPolynom128( Polynom128& poly )
{
aesState.movePolynom128ToState( poly );
// aesState.showState();

encryptLoop();

aesState.moveStateToPolynom128( poly );
}



void Aes::encryptTest( void )
{
// Appendix C.1.
// PLAINTEXT: 00112233445566778899aabbccddeeff

CharBuf plainText;
plainText.appendU8( 0x00 );
plainText.appendU8( 0x11 );
plainText.appendU8( 0x22 );
plainText.appendU8( 0x33 );
plainText.appendU8( 0x44 );
plainText.appendU8( 0x55 );
plainText.appendU8( 0x66 );
plainText.appendU8( 0x77 );
plainText.appendU8( 0x88 );
plainText.appendU8( 0x99 );
plainText.appendU8( 0xaa );
plainText.appendU8( 0xbb );
plainText.appendU8( 0xcc );
plainText.appendU8( 0xdd );
plainText.appendU8( 0xee );
plainText.appendU8( 0xff );

StIO::putS( "PlainText set." );

CharBuf key;

key.appendU8( 0x00 );
key.appendU8( 0x01 );
key.appendU8( 0x02 );
key.appendU8( 0x03 );
key.appendU8( 0x04 );
key.appendU8( 0x05 );
key.appendU8( 0x06 );
key.appendU8( 0x07 );
key.appendU8( 0x08 );
key.appendU8( 0x09 );
key.appendU8( 0x0A );
key.appendU8( 0x0B );
key.appendU8( 0x0C );
key.appendU8( 0x0D );
key.appendU8( 0x0E );
key.appendU8( 0x0F );

// For a 256 bit key.
// 000102030405060708090a0b0c0d0e0f
//       101112131415161718191a1b1c1d1e1f
key.appendU8( 0x10 );
key.appendU8( 0x11 );
key.appendU8( 0x12 );
key.appendU8( 0x13 );
key.appendU8( 0x14 );
key.appendU8( 0x15 );
key.appendU8( 0x16 );
key.appendU8( 0x17 );
key.appendU8( 0x18 );
key.appendU8( 0x19 );
key.appendU8( 0x1a );
key.appendU8( 0x1b );
key.appendU8( 0x1c );
key.appendU8( 0x1d );
key.appendU8( 0x1e );
key.appendU8( 0x1f );


/*
// Appendix A.1 Key Expansion.
key.appendU8( 0x2b, 1024 );
key.appendU8( 0x7e, 1024 );
key.appendU8( 0x15, 1024 );
key.appendU8( 0x16, 1024 );
key.appendU8( 0x28, 1024 );
key.appendU8( 0xae, 1024 );
key.appendU8( 0xd2, 1024 );
key.appendU8( 0xa6, 1024 );
key.appendU8( 0xab, 1024 );
key.appendU8( 0xf7, 1024 );
key.appendU8( 0x15, 1024 );
key.appendU8( 0x88, 1024 );
key.appendU8( 0x09, 1024 );
key.appendU8( 0xcf, 1024 );
key.appendU8( 0x4f, 1024 );
key.appendU8( 0x3c, 1024 );
*/

aesKeySched.setKey( key, 32, aesSBox );

StIO::putS( "Key set." );

CharBuf outBuf;
encryptCharBuf( plainText, outBuf, 0 );

for( Int32 count = 0; count < 16; count++ )
  {
  Uint8 theByte = outBuf.getU8( count );
  char c = ByteHex::getLeftChar( theByte );
  StIO::putChar( c );
  c = ByteHex::getRightChar( theByte );
  StIO::putChar( c );

  // Uint8 ByteHex::charToU8( const char c )

  }

StIO::putLF();
}
