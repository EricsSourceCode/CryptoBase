// Copyright Eric Chauvin, 2022 - 2023.


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html




#pragma once



#include "../CppBase/BasicTypes.h"
#include "../CppBase/RangeC.h"
// #include "../CppBase/StIO.h"
#include "../CppBase/CharBuf.h"
// #include "../CppBase/ByteHex.h"
#include "Polynom128.h"


// This corresponds to a 128 bit block.
// 16 times 8 bits.
// AES has a fixed block size of 128 bits.
// State as in "state machine".


// -Wno-unsafe-buffer-usage

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunsafe-buffer-usage"


class AesState
  {
  private:
  bool testForCopy = false;
  Uint8 aR[16] = { 1, 2, 3 };


  public:
  inline AesState( void )
    {

    }

  inline AesState( const AesState& in )
    {
    if( in.testForCopy )
      return;

    throw "Copy constructor for AesState.";
    }

  inline ~AesState( void )
    {

    }

  inline void copy( AesState& in )
    {
    for( Int32 count = 0; count < 16; count++ )
      aR[count] = in.aR[count];

    }


  inline Uint8 getV( const Int32 row,
                     const Int32 column ) const
    {
    // This is column Major order because
    // consecutive elements in a column are
    // next to each other.
    const Int32 where = (row * 4) + column;

    // This range checking can be commented out.
    RangeC::test2( where, 0, 15,
       "AesState.getV() range for where." );

    return aR[where];
    }

  inline void setV( const Int32 row,
                    const Int32 column,
                    const Uint8 val )
    {
    const Int32 where = (row * 4) + column;

    RangeC::test2( where, 0, 15,
       "AesState.setV() range for where." );

    aR[where] = val;
    }


  // The state is 4 words in columns.
/*
  inline void showState( void )
    {
    StIO::printF( "State: " );

    // Each column is one word.
    for( Int32 column = 0; column < 4; column++ )
      {
      // Down the row for each column.
      for( Int32 row = 0; row < 4; row++ )
        {
        Uint8 val = getV( row, column );
        char c = ByteHex::getLeftChar( val );
        StIO::putChar( c );
        c = ByteHex::getRightChar( val );
        StIO::putChar( c );
        }

      // Separate each word.
      StIO::putChar( ' ' );
      }

    StIO::putLF();
    }
*/


  // Is this needed at all?
  inline void moveCharBufToState(
                           const CharBuf& buf,
                           const Int32 where )
    {
    const Int32 last = buf.getLast();
    if( (where + 15) >= last )
      throw "AesState moveCharBufToState range.";

    // The word at column zero.
    setV( 0, 0, buf.getU8( where ));
    setV( 1, 0, buf.getU8( where + 1 ));
    setV( 2, 0, buf.getU8( where + 2 ));
    setV( 3, 0, buf.getU8( where + 3 ));

    // Word at column 1.
    setV( 0, 1, buf.getU8( where + 4 ));
    setV( 1, 1, buf.getU8( where + 5 ));
    setV( 2, 1, buf.getU8( where + 6 ));
    setV( 3, 1, buf.getU8( where + 7 ));

    setV( 0, 2, buf.getU8( where + 8 ));
    setV( 1, 2, buf.getU8( where + 9 ));
    setV( 2, 2, buf.getU8( where + 10 ));
    setV( 3, 2, buf.getU8( where + 11 ));

    setV( 0, 3, buf.getU8( where + 12 ));
    setV( 1, 3, buf.getU8( where + 13 ));
    setV( 2, 3, buf.getU8( where + 14 ));
    setV( 3, 3, buf.getU8( where + 15 ));
    }


  inline void moveStateToCharBuf( CharBuf& buf )
    {
    buf.appendU8( getV( 0, 0 ));
    buf.appendU8( getV( 1, 0 ));
    buf.appendU8( getV( 2, 0 ));
    buf.appendU8( getV( 3, 0 ));

    buf.appendU8( getV( 0, 1 ));
    buf.appendU8( getV( 1, 1 ));
    buf.appendU8( getV( 2, 1 ));
    buf.appendU8( getV( 3, 1 ));

    buf.appendU8( getV( 0, 2 ));
    buf.appendU8( getV( 1, 2 ));
    buf.appendU8( getV( 2, 2 ));
    buf.appendU8( getV( 3, 2 ));

    buf.appendU8( getV( 0, 3 ));
    buf.appendU8( getV( 1, 3 ));
    buf.appendU8( getV( 2, 3 ));
    buf.appendU8( getV( 3, 3 ));
    }



  inline void movePolynom128ToState(
                          const Polynom128& poly )
    {
    // The word at column zero.
    setV( 0, 0, poly.getByte( 0 ));
    setV( 1, 0, poly.getByte( 1 ));
    setV( 2, 0, poly.getByte( 2 ));
    setV( 3, 0, poly.getByte( 3 ));

    // Word at column 1.
    setV( 0, 1, poly.getByte( 4 ));
    setV( 1, 1, poly.getByte( 5 ));
    setV( 2, 1, poly.getByte( 6 ));
    setV( 3, 1, poly.getByte( 7 ));

    setV( 0, 2, poly.getByte( 8 ));
    setV( 1, 2, poly.getByte( 9 ));
    setV( 2, 2, poly.getByte( 10 ));
    setV( 3, 2, poly.getByte( 11 ));

    setV( 0, 3, poly.getByte( 12 ));
    setV( 1, 3, poly.getByte( 13 ));
    setV( 2, 3, poly.getByte( 14 ));
    setV( 3, 3, poly.getByte( 15 ));
    }


  inline void moveStateToPolynom128(
                               Polynom128& poly )
    {
    poly.setByte( 0, getV( 0, 0 ));
    poly.setByte( 1, getV( 1, 0 ));
    poly.setByte( 2, getV( 2, 0 ));
    poly.setByte( 3, getV( 3, 0 ));

    poly.setByte( 4, getV( 0, 1 ));
    poly.setByte( 5, getV( 1, 1 ));
    poly.setByte( 6, getV( 2, 1 ));
    poly.setByte( 7, getV( 3, 1 ));

    poly.setByte( 8, getV( 0, 2 ));
    poly.setByte( 9, getV( 1, 2 ));
    poly.setByte( 10, getV( 2, 2 ));
    poly.setByte( 11, getV( 3, 2 ));

    poly.setByte( 12, getV( 0, 3 ));
    poly.setByte( 13, getV( 1, 3 ));
    poly.setByte( 14, getV( 2, 3 ));
    poly.setByte( 15, getV( 3, 3 ));
    }


  };

#pragma clang diagnostic pop
