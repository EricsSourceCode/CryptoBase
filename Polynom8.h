// Copyright Eric Chauvin, 2022.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#pragma once



#include "../CppBase/BasicTypes.h"
#include "../CppBase/RangeC.h"
#include "../CppBase/StIO.h"



// See Polynom128.h for notes about this.


class Polynom8
  {
  private:

  public:

  // This can be optimized a lot.

  static inline Uint8 multiply( Uint8 A, Uint8 B )
    {
    // Multiplication in the field GF2^8.
    // Multiplication is mod an irreducible
    // polynomial x^8 + x^4 + x^3 + x + 1.

    Uint8 product = 0;
    for( Int32 count = 0; count < 8; count++ )
      {
      if( (B & 1) != 0 )
        product ^= A; // Addition.

      // Get the carry bit before shifting it off.
      Uint8 carryBit = A & 0x80;
      A <<= 1; // Double it.
      if( carryBit != 0 )
        A ^= 0x1B; // x^8 + x^4 + x^3 + x + 1
        // This is subtraction by 0x1B.
        // Actually, by 0x11B, but that carry
        // bit goes off the 8 bit size.

      B >>= 1; // Get half of B.
      }

    return product;
    }

  };
