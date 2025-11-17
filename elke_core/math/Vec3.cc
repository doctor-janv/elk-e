#include "Vec3.h"

namespace elke::math
{

/**Default constructor. Initialized as all zeros.*/
Vec3::Vec3()
{
  x=0.0; y=0.0; z=0.0;
}

/**Constructor where single element is initialized \f$ x[z]=a \f$.*/
Vec3::Vec3(double a)
{
  x=a; y=0.0; z=0.0;
}

/**Constructor where \f$ x=a\f$ and \f$ y=b \f$. */
Vec3::Vec3(double a, double b)
{
  x=a; y=b; z=0.0;
}

/**Constructor where \f$ \vec{x}=[a,b,c] \f$.*/
Vec3::Vec3(double a, double b, double c)
{
  x=a; y=b; z=c;
}

}// namespace elk::math
