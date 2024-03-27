// Copyright Eric Chauvin 2022 - 2024.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#include "AesGalois.h"
#include "Polynom128.h"
#include "..\CppBase\Casting.h"



//////////////////////////////////////
// Symbols used in the NIST document
// NIST Special Publication 800-38D.

// K: The key.

// IV: Initialization Vector.
// Section 2.1 Inputs and Outputs.

// P: Plaintext.

// A: AAD Additional Authentication Data.

// C: Ciphertext

// T: Authentication Tag
// authTag length has to be 128, 120, 112, 104,
// or 96 bits.

// t: The length of the authTag.

// CIPHk: The AES cipher function for one block
// with a specific key.

// inc32: The increment function to generate
// the sequence of counter blocks.

// H: The hash sub key.

// ICB: Initial counter block.

// J0: Precounter block.

//////////////////////////////////




void AesGalois::encryptTest( void )
{
StIO::putS( "AesGalois encrypt test start." );

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

setKey( key, 16 );

CharBuf cipherBuf;
CharBuf IV( "This is the IV." );
CharBuf aaData( "This is the additional data." );

               //  12345678901234567890123
CharBuf plainBuf( "This is the plain text." );
CharBuf plainBuf2;

StIO::putS( "plainBuf:" );
StIO::putCharBuf( plainBuf );
StIO::putS( " " );

StIO::putS( "Before encrypt." );

encryptCharBuf( plainBuf,
                   IV,
                   aaData,
                   cipherBuf );

// StIO::putS( "cipherBuf:" );
// StIO::putCharBuf( cipherBuf );
// StIO::putS( " " );

StIO::putS( "Before decrypt." );

decryptCharBuf( cipherBuf,
                IV,
                aaData,
                plainBuf2 );

StIO::putS( "plainBuf2:" );
StIO::putCharBuf( plainBuf2 );
StIO::putS( " " );


StIO::putS( "AesGalois test finished." );

// aes.encryptTest();
}



void AesGalois::setKey( const CharBuf& theKey,
                        const Int32 keySize )
{
aes.setKey( theKey, keySize );

makeHashSubKey();
}




void AesGalois::makeHashSubKey( void )
{
// StIO::putS( "Making hash sub key." );

if( !aes.getKeyIsSet())
  throw "Set key before calling makeHashSubKey.";

H.setToZero();

// The H value has to be kept secret.
aes.encryptPolynom128( H );
}




void AesGalois::setCharBufToBlock16( CharBuf& cBuf )
{
Int32 byteCount = cBuf.getLast();

Int32 bytesMod16 = byteCount % 16;
if( bytesMod16 != 0 )
  {
  Int32 toAdd = 16 - bytesMod16;
  for( Int32 padCount = 0; padCount < toAdd;
                                padCount++ )
    {
    // Pad it with zeros.
    cBuf.appendU8( 0 );
    }
  }

byteCount = cBuf.getLast();
if( (byteCount % 16 ) != 0 )
  throw "byteCount has to be zero mod 16.";

}



void AesGalois::setJ0fromIV( const CharBuf& ivBuf,
                             Polynom128& J0 )
{
// See section 7.1 of the NIST document.

// J0 is the precounter block.
J0.setToZero();

// In this function the IV length is limited to
// a 31 bit integer.  Which is plenty big
// enough for an IV.
// But check the specs for the max allowed.

Int32 ivByteCount = ivBuf.getLast();

// The length of the IV in bits.
Uint64 ivLength = 8 * Casting::i32ToU64(
                                 ivByteCount );


// If it's a 96 bit IV.
if( ivByteCount == 12 )
  {
  J0.setFromCharBuf( ivBuf, 0 );
  J0.setByte( 15, 1 );
  return; // With J0 set.
  }

CharBuf ivBuf2;
ivBuf2.copy( ivBuf );

// Make ivByteCount so it's a multiple of 128 bits.
setCharBufToBlock16( ivBuf2 );

ivByteCount = ivBuf2.getLast();

ivBuf2.appendU64( 0 );
ivBuf2.appendU64( ivLength );

gHash( ivBuf2, J0 );
}



void AesGalois::gHash( const CharBuf& bufToHash,
                       Polynom128& hashVal )
{
hashVal.setToZero();

Int32 byteCount = bufToHash.getLast();
if( byteCount < 16 )
  throw "gHash() byteCount < 16";

if( (byteCount % 16 ) != 0 )
  throw "gHash() byteCount has to be zero mod 16.";

Polynom128 x;
Polynom128 z;

// It is already divisible by 16.
Int32 m = byteCount / 16;

for( Int32 count = 0; count < m; count++ )
  {
  // Get the series of X blocks.
  x.setFromCharBuf( bufToHash, count * 16 );

  hashVal.add( x );
  z.multiply( hashVal, H );
  hashVal.copy( z );
  }

// hashVal is the return value.
}




// The GCTR function is defined in section 6.5
// of the NIST document.

void AesGalois::gctr( const CharBuf& inBuf,
                      const Polynom128& ICB,
                      CharBuf& outBuf )
{
outBuf.clear();

Int32 inLen = inBuf.getLast();
if( inLen == 0 )
  return; // With outBuf langth zero.

// Allocate memory for it.
outBuf.setSize( inLen + 1024 );

Int32 N = inLen / 16; // 16 bytes for 128 bits.

if( (inLen % 16) != 0 )
  N++; // Get the partial last block.

Polynom128 CB;
CB.copy( ICB );

Polynom128 x;
Polynom128 encCount;

for( Int32 where = 0; where < N; where++ )
  {
  x.setFromCharBuf( inBuf, where * 16 );

  encCount.copy( CB );
  aes.encryptPolynom128( encCount );
  x.add( encCount ); // xor with the counter block.
  x.appendToCharBuf( outBuf );
  CB.incr32();
  }

// In the NIST document it says the full block
// encCount gets encrypted.
// Then it does xor with the most significant
// bits with that.
// So those extra bits, the least significant
// bits, just get truncated off here.

outBuf.truncateLast( inLen );
}



void AesGalois::makeAuthTag(
                         const CharBuf& aaData,
                         const CharBuf& cipherBuf,
                         const Polynom128& J0,
                         CharBuf& authTag )
{
Uint64 aadLength = 8 * Casting::i32ToU64(
                              aaData.getLast());

Uint64 cipherLength = 8 * Casting::i32ToU64(
                             cipherBuf.getLast());

CharBuf authBuf;
authBuf.appendCharBuf( aaData );

// Make it an even 128 bits.
setCharBufToBlock16( authBuf );

authBuf.appendCharBuf( cipherBuf );

// Make it an even 128 bits.
setCharBufToBlock16( authBuf );

authBuf.appendU64( aadLength );
authBuf.appendU64( cipherLength );

Polynom128 S;

gHash( authBuf, S );

CharBuf sBuf;
S.appendToCharBuf( sBuf );

authTag.clear();
gctr( sBuf, J0, authTag );

// Since this is coming from a Polynom128
// it is 16 bytes.

// if( authTag.getLast() != 16 )
  // throw "makeAuthTag didn't make 16 bytes.";

// Pass a length parameter and then...
// The authTag would get truncated to a shorter
// length if you want a shorter tag.
}



// The GCM-AE function is defined in section 7.1
// of the NIST document.

// GCM-AE:
void AesGalois::encryptCharBuf(
                          const CharBuf& plainBuf,
                          const CharBuf& IV,
                          const CharBuf& aaData,
                          CharBuf& cipherBuf )
{
// The value of H was set when the key was set.

Polynom128 J0;

// Initial Counter Block.
Polynom128 ICB;

setJ0fromIV( IV, J0 );
ICB.copy( J0 );
ICB.incr32();

gctr( plainBuf, ICB, cipherBuf );

CharBuf authTag;
makeAuthTag( aaData, cipherBuf, J0, authTag );

// StIO::putS( "Inside Encrypt authTag:" );
// StIO::putCharBuf( authTag );
// StIO::putS( " " );

// RFC 5116: "using AES-128
// An authentication tag with a length
// of 16 octets (128 bits) is used."
// "The AEAD_AES_128_GCM ciphertext is
// formed by appending the authentication tag
// provided as an output to the GCM
// encryption operation to the ciphertext
// that is output by that operation.
// Test cases are provided in the appendix
// of [GCM]."
// "An AEAD_AES_128_GCM ciphertext is
// exactly 16 octets longer than its
// corresponding plaintext."

cipherBuf.appendCharBuf( authTag );
}




// The GCM-AD function is defined in section 7.2
// of the NIST document.

// GCM-AD:
void AesGalois::decryptCharBuf(
                    const CharBuf& cipherBuf,
                    const CharBuf& IV,
                    const CharBuf& aaData,
                    CharBuf& plainBuf )
{
Polynom128 J0;
Polynom128 ICB;

setJ0fromIV( IV, J0 );
ICB.copy( J0 );
ICB.incr32();

CharBuf authTag;
Int32 cipherLast = cipherBuf.getLast();
Int32 authStart = cipherLast - 16;
for( Int32 count = authStart;
                  count < cipherLast; count++ )
  {
  Uint8 aByte = cipherBuf.getU8( count );
  authTag.appendU8( aByte );
  }

CharBuf cipherBufShort;
cipherBufShort.copy( cipherBuf );
cipherBufShort.truncateLast( cipherLast - 16 );

gctr( cipherBufShort, ICB, plainBuf );

CharBuf authTagTest;
makeAuthTag( aaData, cipherBufShort, J0,
                           authTagTest );

if( !authTag.isEqual( authTagTest ))
  throw "Auth tags are not equal.";

// StIO::putS(
//      "Auth tags are equal in decrypt." );
}
