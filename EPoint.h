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


// A point on an elliptic curve.


class EPoint
  {
  private:
  bool testForCopy = false;
  bool infin = false; // Infinite
  Integer X;
  Integer Y;


  public:
  inline EPoint( void )
    {
    // X and Y are set to zero by default.
    }


  inline EPoint( const EPoint& in )
    {
    if( in.testForCopy )
      return;

    throw "EPoint copy constructor called.";
    }


  inline EPoint( const Integer& setX,
                 const Integer& setY )
    {
    infin = false;
    X.copy( setX );
    Y.copy( setY );
    }


  inline ~EPoint( void )
    {
    }

  inline bool getInfin( void ) const
    {
    return infin;
    }

  inline void setInfin( bool setTo )
    {
    infin = setTo;
    }

  inline void getX( Integer& toGet ) const
    {
    toGet.copy( X );
    }

  inline void getY( Integer& toGet ) const
    {
    toGet.copy( Y );
    }

  inline void setX( const Integer& toSet )
    {
    X.copy( toSet );
    }

  inline void setY( const Integer& toSet )
    {
    Y.copy( toSet );
    }



  inline void setValues( const Integer& setX,
                         const Integer& setY )
    {
    infin = false;
    X.copy( setX );
    Y.copy( setY );
    }

  inline void copy( const EPoint& p )
    {
    // This might be copying itself, so
    // this.x = p.x, which is OK.

    infin = p.infin;
    X.copy( p.X );
    Y.copy( p.Y );
    }


  inline bool isEqual( const EPoint& in ) const
    {
    if( infin != in.infin )
      return false;

    if( !X.isEqual( in.X ))
      return false;

    if( !Y.isEqual( in.Y ))
      return false;

    return true;
    }

  bool isConjugate( const EPoint& p,
                    const Integer& modulus,
                    Mod& mod,
                    IntegerMath& intMath ) const;

  bool add( const EPoint& p,
            const EPoint& q,
            const Integer& coefA,
            const Integer& coefB,
            const Integer& modulus,
            Integer& gcd,
            Mod& mod,
            IntegerMath& intMath  );

  bool doubleP( const EPoint& p,
                const Integer& coefA,
                const Integer& modulus,
                Integer& gcd,
                Mod& mod,
                IntegerMath& intMath );

  };

*/

