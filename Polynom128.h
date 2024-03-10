// Copyright Eric Chauvin, 2022 - 2023.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


// Compare how the multiply() function works in
// Polynom8.h and Polynom128.h.  In Polynom8.h the
// constant 0x1B shows how it is backwards from the
// way the constant 0xE1 is represented in
// Polynom128.h.  The least significant bits are
// on the right side in Polynom8.h, but on the
// left side in Polynom128.h.  Also, look at
// what gets shifted right or shifted left.
// They are both doing pretty much the same thing
// but it's just done left to right or the
// opposite way.



#pragma once



#include "../CppBase/BasicTypes.h"
#include "../CppBase/RangeC.h"
#include "../CppBase/StIO.h"
#include "../CppBase/CharBuf.h"




// -Wno-unsafe-buffer-usage

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"


class Polynom128
  {
  private:
  bool testForCopy = false;
  Uint8 aR[16] = { 0,1,2 };

  public:
  inline Polynom128( void )
    {
    setToZero();
    }

  inline Polynom128( const Polynom128& in )
    {
    if( in.testForCopy )
      return;

    throw "Copy constructor for Polynom128.";
    }

  inline ~Polynom128( void )
    {

    }


  inline void setToZero( void )
    {
    for( Int32 count = 0; count < 16; count++ )
      aR[count] = 0;

    }

  inline void copy( const Polynom128& in )
    {
    for( Int32 count = 0; count < 16; count++ )
      aR[count] = in.aR[count];

    }


  inline bool isEqual( const Polynom128& in )
    {
    for( Int32 count = 0; count < 16; count++ )
      {
      if( aR[count] != in.aR[count] )
        return false;

      }

    return true;
    }


  inline Uint8 getByte( const Int32 where ) const
    {
    // This range checking can be commented out.
    RangeC::test2( where, 0, 15,
       "Polynom128.getByte() range for where." );

    return aR[where];
    }

  inline void setByte( const Int32 where,
                       const Uint8 val )
    {
    RangeC::test2( where, 0, 15,
       "Polynom128.setByte() range for where." );

    aR[where] = val;
    }


  // Add is the same thing as subtract since it
  // is XOR.  They are the inverse of each other.

  inline void add( const Polynom128& toAdd )
    {
    for( Int32 count = 0; count < 16; count++ )
      aR[count] = aR[count] xor toAdd.aR[count];

    }


  inline void incr32( void )
    {
    // Increment the least significant four
    // bytes, which are on the right side.

    Uint32 counter = aR[12];
    counter <<= 8;
    counter |= aR[13];
    counter <<= 8;
    counter |= aR[14];
    counter <<= 8;
    counter |= aR[15];

    counter++; // Mod 2^32

    aR[15] = counter & 0xFF;
    counter >>= 8;
    aR[14] = counter & 0xFF;
    counter >>= 8;
    aR[13] = counter & 0xFF;
    counter >>= 8;
    aR[12] = counter & 0xFF;
    }



  inline void shiftRight1( void )
    {
    Uint64 right = aR[8];
    right <<= 8;
    right |= aR[9];
    right <<= 8;
    right |= aR[10];
    right <<= 8;
    right |= aR[11];
    right <<= 8;
    right |= aR[12];
    right <<= 8;
    right |= aR[13];
    right <<= 8;
    right |= aR[14];
    right <<= 8;
    right |= aR[15];

    Uint64 left = aR[0];
    left <<= 8;
    left |= aR[1];
    left <<= 8;
    left |= aR[2];
    left <<= 8;
    left |= aR[3];
    left <<= 8;
    left |= aR[4];
    left <<= 8;
    left |= aR[5];
    left <<= 8;
    left |= aR[6];
    left <<= 8;
    left |= aR[7];

    // Shift them to the right.
    right >>= 1;
    if( (left & 1) != 0 )
      right |= 0x8000000000000000ULL;

    left >>= 1;

    // Put it all back.
    aR[7] = left & 0xFF;
    left >>= 8;
    aR[6] = left & 0xFF;
    left >>= 8;
    aR[5] = left & 0xFF;
    left >>= 8;
    aR[4] = left & 0xFF;
    left >>= 8;
    aR[3] = left & 0xFF;
    left >>= 8;
    aR[2] = left & 0xFF;
    left >>= 8;
    aR[1] = left & 0xFF;
    left >>= 8;
    aR[0] = left & 0xFF;

    aR[15] = right & 0xFF;
    right >>= 8;
    aR[14] = right & 0xFF;
    right >>= 8;
    aR[13] = right & 0xFF;
    right >>= 8;
    aR[12] = right & 0xFF;
    right >>= 8;
    aR[11] = right & 0xFF;
    right >>= 8;
    aR[10] = right & 0xFF;
    right >>= 8;
    aR[9] = right & 0xFF;
    right >>= 8;
    aR[8] = right & 0xFF;
    }


  // By this definition, bit zero is on
  // the right side of the byte because it is
  // a normal integer.
  inline Uint8 getOneBit( const Int32 where )
    {
    // Get a 1 bit at location (where).
    switch( where )
      {
      case 0: return 0x01;
      case 1: return 0x02;
      case 2: return 0x04;
      case 3: return 0x08;
      case 4: return 0x10;
      case 5: return 0x20;
      case 6: return 0x40;
      case 7: return 0x80;

      default: return 0;

      }
    }



// These operations are commutative, associative,
// and distributive.


  inline void multiply( const Polynom128& x,
                        const Polynom128& y )
    {
    // The result z is this object.
    setToZero();

    Polynom128 R;
    // The constructor sets R to zero.
    // Notice that this sets the least significant
    // bits on the left side.
    // R is the polynomial 1 + a + a^2 + a^7 +
    // a^128.  Represented as
    // 0xE1 but missing the bit at a^128.
    R.setByte( 0, 0xE1 );

    Polynom128 v;
    v.copy( x );

    // For this polynomial bit zero is on the
    // left side.

    // The document says for i = 0 to 127
    // as the bits zero to 127.

    // For each byte.
    for( Int32 byteCount = 0; byteCount < 16;
                                   byteCount++ )
      {
      Uint8 yByte = y.getByte( byteCount );

      // For each bit in each byte.
      // From left to right.
      for( Int32 bitCount = 7; bitCount >= 0;
                 bitCount-- )
        {
        // If that particular bit is a 1.
        if( (yByte &
             getOneBit( bitCount)) != 0 )
          {
          add( v );  // z = z xor v.
          }

        // If V127 is zero.
        // If the right most bit is zero.
        if( (v.getByte( 15 ) & 0x01) == 0 )
          {
          v.shiftRight1();
          }
        else
          {
          // (v >> 1) xor R
          v.shiftRight1();

          // For R, the non zero bits are on
          // the far left.  The byte 0xE1 is
          // at index zero on the left.
          v.add( R );
          }
        }
      }

    // Z is this object.
    }



  inline void setFromCharBuf( const CharBuf& cBuf,
                              const Int32 where )
    {
    const Int32 last = cBuf.getLast();

    for( Int32 count = 0; count < 16; count++ )
      {
      Int32 index = count + where;
      if( index >= last )
        aR[count] = 0; // Pad it with zeros.
      else
        aR[count] = cBuf.getU8( index );

      }
    }


  inline void appendToCharBuf( CharBuf& cBuf )
    {
    for( Int32 count = 0; count < 16; count++ )
      {
      cBuf.appendU8( aR[count] );
      }
    }


  };

#pragma clang diagnostic pop
