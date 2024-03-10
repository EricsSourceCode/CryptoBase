// Copyright Eric Chauvin, 2022 - 2023.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once



#include "../CppBase/BasicTypes.h"
#include "../CppBase/RangeC.h"
#include "../CppBase/CharBuf.h"
// #include "../CppBase/StIO.h"
// #include "../CppBase/ByteHex.h"
#include "AesSBox.h"



// -Wno-unsafe-buffer-usage

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"


class AesKeySched
  {
  private:
  Uint8 key[32] = { 1, 2, 3 };
  bool keyIsSet = false;

  // The key schedule generates Nb(Nr + 1) words.
  // The maximum size is 4 * (14 + 1) = 60.
  Uint32 keyWords[60] = { 1, 2, 3 };

  // Nb = Number of columns, 32 bit words, in
  // the state.  It is always 4.
  // Nk = Number of 32 bit words in the key.
  // It is 4, 6 or 8.
  // Nr = Number of rounds.

  // 10 rounds for 128 bit key, 12 rounds for 192,
  // 14 rounds for 256 bit key.
  Int32 numberOfRounds = 14;
  Int32 keyLengthInBytes = 32; // 256 / 8
  Int32 keyWordsSize = 4 * (numberOfRounds + 1);

  void keyExpansion( const AesSBox& aesSBox );

  // This is called RotWord() in the FIPS
  // standard document.
  static inline Uint32 rotateWord(
                      const Uint32 inWord )
    {
    Uint32 outWord = (inWord << 8) |
                     (inWord >> 24); // 32 - 8
    return outWord;
    }

  inline Uint8 getKeyByte( const Int32 where ) const
    {
    RangeC::test2( where, 0,
         32 - 1,
         "AesKey.getKeyByte() range for where." );

    return key[where];
    }

  inline void setKeyByte( const Int32 where,
                          const Uint8 val )
    {
    RangeC::test2( where, 0,
         32 - 1,
         "AesKey.setKeyByte() range for where." );

    key[where] = val;
    }

  inline void setWord( const Int32 where,
                       const Uint32 val )
    {
    RangeC::test2( where, 0,
                  60 - 1,
        "AesKeySched.setWord() range for where." );

    keyWords[where] = val;
    }

  inline static Uint32 makeUintFromBytes(
                          const Uint8 byte0,
                          const Uint8 byte1,
                          const Uint8 byte2,
                          const Uint8 byte3 )
    {
    Uint32 result = byte0;
    result <<= 8;
    result |= byte1;
    result <<= 8;
    result |= byte2;
    result <<= 8;
    result |= byte3;

    return result;
    }



  public:
  void setKey( const CharBuf& theKey,
               const Int32 keySize,
               const AesSBox& aesSBox );

  inline Int32 getNumberOfRounds( void ) const
    {
    return numberOfRounds;
    }

  inline Uint32 getWord( const Int32 where ) const
    {
    // Comment these out after testing.
    RangeC::test2( where, 0,
                  60 - 1,
        "AesKeySched.getWord() range for where." );

    return keyWords[where];
    }

  inline bool getKeyIsSet( void ) const
    {
    return keyIsSet;
    }

  // void showAll( const Int32 howMany );

  };

#pragma clang diagnostic pop
