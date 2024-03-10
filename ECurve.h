/*

// Copyright Eric Chauvin, 2021 - 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#pragma once


#include "../CppBase/BasicTypes.h"
// #include "../CppBase/FileIO.h"

#include "../CppInt/Integer.h"
#include "../CppInt/IntegerMath.h"
#include "Mod.h"
#include "EPoint.h"



// A specific elliptic curve.
// y^2 = x^3 + a*x + b,
// This is the Weierstrass form.

class ECurve
  {
  private:
  bool testForCopy = false;
  Integer coefA;
  Integer coefB;

  // EPoint* pArray;
  // EPoint* baseArray;
  // static const Int32 arraySize =
  //                      ProjConst::ellipseLast;

  // Integer prime;
  // Integer generator;
  // Integer orderN; // The order of the
  //                   generator G.
  // Integer coFactor; // h is
  // pound symbol E( Fp ) is the number of
  // points on the curve.
  // The cofactor h is pound symbol E( Fp ) / n.

  // void setupBases( const EPoint p );

  public:
  // RFC 6090: Fundamental Elliptic Curve
  // Cryptography Algorithms
  // RFC 4492: Elliptic Curve Groups (ECDHE)


  // Named Curves:
  // sect163k1 (1),
  // sect163r1 (2),
  // sect163r2 (3),
  // sect193r1 (4),
  // sect193r2 (5),
  // sect233k1 (6),
  // sect233r1 (7),
  // sect239k1 (8),
  // sect283k1 (9),
  // sect283r1 (10),
  // sect409k1 (11),
  // sect409r1 (12),
  // sect571k1 (13),
  // sect571r1 (14),
  // secp160k1 (15),
  // secp160r1 (16),
  // secp160r2 (17),
  // secp192k1 (18),
  // secp192r1 (19),
  // secp224k1 (20),
  // secp224r1 (21),
  // secp256k1 (22),

  // For TLS 1.3 in RFC 8446 Section 4.2.7.
  // Elliptic Curve Groups (ECDHE)
  // Starting at decimal 23 = 0x17.
  static const Uint32 NamedCrvSecp256r1 = 23;
  static const Uint32 NamedCrvSecp384r1 = 24;
  static const Uint32 NamedCrvSecp521r1 = 25;
  // 26, 27, 28

  // See MCurve.cpp for these:
  // static const Uint32 NamedCrvX25519 = 29;
  // static const Uint32 NamedCrvX448 = 30;


  inline ECurve( void )
    {
    // pArray = new EPoint[arraySize];
    // baseArray = new EPoint[arraySize];
    }

  inline ECurve( const ECurve& in )
    {
    // pArray = new EPoint[arraySize];
    // baseArray = new EPoint[arraySize];

    if( in.testForCopy )
      return;

    throw "ECurve copy constructor called.";
    }

  inline ~ECurve( void )
    {
    // delete[] pArray;
    // delete[] baseArray;
    }


  bool isOnCurve( const EPoint& aPoint,
                  const Integer& prime,
                  Mod& mod,
                  IntegerMath& intMath );

  bool findFactors( const Integer& N,
                    Integer& gcd,
                    IntegerMath& intMath,
                    Mod& mod );


  bool scalarMult( EPoint& result,
                   const Integer& k,
                   const Integer& modulus,
                   Integer& gcd,
                   IntegerMath& intMath,
                   Mod& mod );

  void testLoopAdd( EPoint& result,
                    const Integer& k,
                    const Integer& modulus,
                    Integer& gcd,
                    IntegerMath& intMath,
                    Mod& mod );

  void testMult( const Integer& N,
                 Integer& gcd,
                 IntegerMath& intMath,
                 Mod& mod );

  };

*/
