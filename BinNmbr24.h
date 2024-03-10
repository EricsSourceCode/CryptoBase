// Copyright Eric Chauvin 2023


// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#pragma once


#include "../CppBase/BasicTypes.h"




class BinNmbr24
  {
  private:
  bool testForCopy = false;
  Int32 index = 0; // Index in to base values.
                   // It can be different bases.
  Int64 digit = 0;

  public:
  inline BinNmbr24( void )
    {
    setZero();
    }

  inline BinNmbr24( const BinNmbr24& in )
    {
    if( in.testForCopy )
      return;

    throw "BinNmbr24 copy constructor.";
    }

  inline ~BinNmbr24( void )
    {
    }


  inline Int32 getIndex( void ) const
    {
    return index;
    }

  inline void setIndex( const Int32 setTo )
    {
    index = setTo;
    }


  inline void setZero( void )
    {
    digit = 0;
    }

  inline void setOne( void )
    {
    digit = 1;
    }


//  inline Uint64 getMod64( Uint64 x )


  inline void add( const BinNmbr24& toAdd )
    {
    if( toAdd.digit < 0 )
      throw "BinNmbr24 add with negative.";

    digit += toAdd.digit;
    }

  inline bool isZero( void ) const
    {
    if( digit == 0 )
      return true;

    return false;
    }


  inline bool isOne( void ) const
    {
    if( digit == 1 )
      return true;

    return false;
    }

  inline void copy( BinNmbr24& from )
    {
    digit = from.digit;
    index = from.index;
    }


  inline void multiply( BinNmbr24& x,
                        BinNmbr24& y )
    {
    if( x.digit < 0 )
      throw "BinNmbr24 Multiply with negative.";

    if( y.digit < 0 )
      throw "BinNmbr24 Multiply with negative.";

    digit = x.digit * y.digit;
    index = x.index + y.index;
    }


  inline void getLowPart( BinNmbr24& lPart )
    {
    lPart.digit = digit & 0xFFFFFF;
    }

  inline void getHighPart( BinNmbr24& hPart )
    {
    // If you right shift a negative number the
    // result is implementation dependant.

    if( digit < 0 )
      throw "Shift right on negative digit.";

    hPart.digit = digit >> 24;
    }

  inline void clearHighPart( void )
    {
    digit = digit & 0xFFFFFF;
    }


  };
