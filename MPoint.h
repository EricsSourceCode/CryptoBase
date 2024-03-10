// Copyright Eric Chauvin, 2023.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html


#pragma once


#include "../CppBase/BasicTypes.h"
// #include "../CppBase/FileIO.h"

#include "../CppInt/Integer.h"
#include "../CppInt/IntegerMath.h"
#include "../CppInt/Mod.h"


// A point on a Montgomery curve.


class MPoint
  {
  private:
  bool testForCopy = false;
  bool infin = false; // Infinite
  Integer X;
  Integer Y;


  public:
  static const Int32 coefA = 486662;
  // coefB; It is 1.

  inline MPoint( void )
    {
    // X and Y are set to zero by default.
    }


  inline MPoint( const MPoint& in )
    {
    if( in.testForCopy )
      return;

    throw "MPoint copy constructor called.";
    }


  inline MPoint( const Integer& setX,
                 const Integer& setY )
    {
    infin = false;
    X.copy( setX );
    Y.copy( setY );
    }


  inline ~MPoint( void )
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

  inline void copy( const MPoint& p )
    {
    // This might be copying itself, so
    // this.x = p.x, which is OK.

    infin = p.infin;
    X.copy( p.X );
    Y.copy( p.Y );
    }


  inline bool isEqual( const MPoint& in ) const
    {
    if( infin != in.infin )
      return false;

    if( !X.isEqual( in.X ))
      return false;

    if( !Y.isEqual( in.Y ))
      return false;

    return true;
    }

  bool isConjugate( const MPoint& p,
                    const Integer& prime,
                    Mod& mod,
                    IntegerMath& intMath ) const;

  bool add( const MPoint& p1,
            const MPoint& p2,
            const Integer& prime,
            Integer& gcd,
            Mod& mod,
            IntegerMath& intMath  );

  bool doubleP( const MPoint& p,
                const Integer& prime,
                Integer& gcd,
                Mod& mod,
                IntegerMath& intMath );

  };
