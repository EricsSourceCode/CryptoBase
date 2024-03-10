// Copyright Eric Chauvin, 2022 - 2024.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


// See https://ericinarizona.github.io/
// For guides and information.


// See RFC 5116.
// See NIST Special Publication 800-38D.



#pragma once



#include "../CppBase/BasicTypes.h"
#include "../CppBase/RangeC.h"
#include "../CppBase/StIO.h"
#include "Aes.h"
#include "Polynom128.h"



// The cipher suite used in the TLS 1.3
// handshake protocol.
// TLS_AES_128_GCM_SHA256    {0x13,0x01}



class AesGalois
  {
  private:
  bool testForCopy = false;
  Aes aes;
  Polynom128 H; // Hash sub key.

  void makeHashSubKey( void );
  void setJ0fromIV( const CharBuf& ivBuf,
                    Polynom128& J0 );

  void gHash( const CharBuf& bufToHash,
              Polynom128& hashVal );

  void gctr( const CharBuf& inBuf,
             const Polynom128& ICB,
             CharBuf& outBuf );

  void setCharBufToBlock16( CharBuf& cBuf );

  void makeAuthTag( const CharBuf& aaData,
                    const CharBuf& cipherBuf,
                    const Polynom128& J0,
                    CharBuf& authTag );


  public:
  AesGalois( void )
    {
    }

  AesGalois( const AesGalois& in )
    {
    if( in.testForCopy )
      return;

    throw "Copy constructor for AesGalois.";
    }

  ~AesGalois( void )
    {
    // Erase what was there.

    // Integer.erase()
    // H.setToZero() This doesn't erase it.

    // And what else?
    // Erase the AES key schedule things.
    }



  void encryptTest( void );

  void encryptCharBuf( const CharBuf& plainBuf,
                       const CharBuf& IV,
                       const CharBuf& aaData,
                       CharBuf& cipherBuf );

  void decryptCharBuf( const CharBuf& cipherBuf,
                       const CharBuf& IV,
                       const CharBuf& aaData,
                       CharBuf& plainBuf );

  void setKey( const CharBuf& theKey,
               const Int32 keySize );


  };
