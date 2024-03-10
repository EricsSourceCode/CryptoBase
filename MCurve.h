// Copyright Eric Chauvin, 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#pragma once



// RFC 7748


#include "../CppBase/BasicTypes.h"
#include "../CppInt/Integer.h"
#include "../CppInt/IntegerMath.h"
#include "../CppInt/Mod.h"
#include "MPoint.h"




class MCurve
  {
  private:
  bool testForCopy = false;
  Integer prime;

  public:
  // Named Curves:
  // For TLS 1.3 in RFC 8446 Section 4.2.7.
  static const Uint32 NamedCrvX25519 = 29;
  static const Uint32 NamedCrvX448 = 30;


  inline MCurve( void )
    {
    }

  inline MCurve( const MCurve& in )
    {
    if( in.testForCopy )
      return;

    throw "MCurve copy constructor called.";
    }

  inline ~MCurve( void )
    {
    }


  bool isOnCurve( const MPoint& aPoint,
                  Mod& mod,
                  IntegerMath& intMath );

  bool scalarMult( MPoint& result,
                   const Integer& k,
                   Integer& gcd,
                   IntegerMath& intMath,
                   Mod& mod );

  void cSwap1( Int32 bit,
               Integer& X2,
               Integer& X3 );

  // void cSwap2( Int32 bit,
  //              Integer& X2,
  //              Integer& X3 );

  bool montLadder1( Integer& result,
                    const Integer& U,
                    const Integer& k,
                    IntegerMath& intMath,
                    Mod& mod );

  // void clampKInt( Integer& toSet );
  void clampK( ByteArray& cArray );
  void clampU( ByteArray& cArray );

  void cArrayToInt( const ByteArray& cAr,
                          Integer& X );

  void uCoordTo32Bytes( const Integer& U,
                        ByteArray& uBytes,
                        Mod& mod,
                        IntegerMath& intMath )
                                        const;

  void makeRandom32Bytes( ByteArray& cArray );
  void makeThePrime( IntegerMath& intMath );

  // void test( Mod& mod,
  //           IntegerMath& intMath );

  // void testVectors( Mod& mod,
  //                  IntegerMath& intMath );

  void makeRandExponentK( Integer& k,
                          CharBuf& keyShare );

  };
