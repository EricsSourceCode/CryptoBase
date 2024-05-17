// Copyright Eric Chauvin 2022 - 2024.



// This is licensed under the GNU General
// Public License (GPL).  It is the
// same license that Linux has.
// https://www.gnu.org/licenses/gpl-3.0.html



#pragma once



// This is supposed to be cryptographically
// random.  It is random enough for now
// to test an algorithm.

// But it needs to be done better.



#include "../CppBase/BasicTypes.h"
#include "../CppBase/CharBuf.h"


class Randomish
  {
  private:

  public:
  static void makeRandomBytes( CharBuf& cBuf,
                             const Int32 howMany );

  static Float32 makeRandomFloat32( void );

  };
