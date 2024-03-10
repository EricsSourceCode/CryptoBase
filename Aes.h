// Copyright Eric Chauvin 2022.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



// Make it work first, then make it work fast.




#pragma once



// See AesGalois.h for the mode of operation.

// There are Intel and AMD AES-NI instruction
// set extensions.  Those would be _much_ faster
// than this.

// And see the CLMUL instruction set for
// things like Galois Counter Mode.


// See: FIPS publication 197.


#include "../CppBase/BasicTypes.h"
#include "../CppBase/Casting.h"
#include "../CppBase/CharBuf.h"
// #include "../CppBase/StIO.h"

#include "AesState.h"
#include "AesSBox.h"
#include "AesKeySched.h"
#include "Polynom8.h"
#include "Polynom128.h"



class Aes
  {
  private:
  bool testForCopy = false;
  AesState aesState;
  AesSBox aesSBox;
  AesKeySched aesKeySched;

  // AES has a fixed block size of 128 bits.
  // Nb = Number of columns, 32 bit words, in
  // the state.  It is always 4.
  // Nk = Number of 32 bit words in the key.
  // It is 4, 6 or 8.
  // Nr = Number of rounds.

  void addRoundKey( Int32 round );
  void subBytes( void );
  void shiftRows( void );
  void mixColumns( void );


  public:
  inline Aes( void )
    {
    }

  inline Aes( const Aes& in )
    {
    if( in.testForCopy )
      return;

    throw "Copy constructor for Aes.";
    }

  inline ~Aes( void )
    {

    }

  void encryptLoop( void );

  void encryptPolynom128( Polynom128& poly );

  void encryptCharBuf( const CharBuf& inBuf,
                       CharBuf& outBuf,
                       const Int32 where );

  void encryptTest( void );

  inline void setKey( const CharBuf& theKey,
                      const Int32 keySize )
    {
    aesKeySched.setKey( theKey, keySize, aesSBox );
    }

  inline bool getKeyIsSet( void ) const
    {
    return aesKeySched.getKeyIsSet();
    }

  };
