# GLPK routine
# http://www.gnu.org/software/glpk/
AC_DEFUN([CHECK_PRECISION_INIT],[
  AC_SUBST(HAS_FLOAT128) 

  AC_MSG_NOTICE(Checking for float128)

  AC_LANG_PUSH([C++])

  AC_LINK_IFELSE(
    [
        AC_LANG_PROGRAM([[
#include <iostream>
#include <math.h>

using namespace std;

struct Coordinate
{
double x, y;
};

typedef __float128 Float128;
double MEAN_EARTH_RADIUS = 6371009;
typedef double Degrees;
typedef double Meters;
typedef double Radians;

Radians toRadians(Degrees d) { return d / 180.0 * M_PI; }

Degrees toDegrees(Radians r) { return r / M_PI * 180.0; }

double haversine(const Coordinate &c1, const Coordinate &c2)
{
  // use long double instead of double. With GCC this gives 128 bit double and can discern down
  // to about 2mm at the equator using haversine. Using a regular double only gives ~9cm.
  Float128 c1LatAsRadians = (Float128)c1.y / 180.0 * M_PI;
  Float128 c1LngAsRadians = (Float128)c1.x / 180.0 * M_PI;
  Float128 c2LatAsRadians = (Float128)c2.y / 180.0 * M_PI;
  Float128 c2LngAsRadians = (Float128)c2.x / 180.0 * M_PI;

  Float128 v = cosl(c1LatAsRadians) * cosl(c2LatAsRadians)
      * cosl(c1LngAsRadians - c2LngAsRadians) + sinl(c1LatAsRadians)
      * sinl(c2LatAsRadians);
  if (v > 1.0)
  {
    v = 1.0;
  }
  Float128 dsigma = acosl(v);
  return dsigma * MEAN_EARTH_RADIUS;
}
]],
        [[
        Coordinate c1, c2;
        c1.x = 0;
        c1.y = 0;
        c2.x = 1;
        c2.y = 0;
        cout << haversine(c1, c2) << endl;
]])
    ],
    [foundFloat128=yes; break;], [foundFloat128=no; break;])
    
    if test "$foundFloat128" == "yes"; then
      AC_DEFINE([HAVE_FLOAT128], [1], [Have working __float128])
      AC_MSG_NOTICE(Float128 was found.)
    else
      AC_DEFINE([HAVE_FLOAT128], [0], [Does not have working __float128])
      AC_MSG_NOTICE(Float128 was not found.)
    fi

    AC_LANG_POP
])

