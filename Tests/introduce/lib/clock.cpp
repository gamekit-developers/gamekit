
#include "clock.h"
#include "sock_port.h"

#include <assert.h>
#include <math.h>

using namespace libintroduce;

namespace {
  class C_Clock : public I_Clock {
    public:
      C_Clock() {
        if( origin_ < 0 ) {
          myOrigin_ = 0;
          origin_ = realTime();
          assert( origin_ >= 0 );
        }
        myOrigin_ = origin_ - 100;
      }
      void dispose() { delete this; }
      double realTime();
      void adjust( double delta ) { 
        myOrigin_ -= delta;
      }
      static double origin_;
      double myOrigin_;
  };
  double C_Clock::origin_ = -100;
}

I_Clock * libintroduce::CreateClock()
{
  return static_cast< I_Clock * >( new C_Clock() );
}

double C_Clock::realTime()
{
  struct timeval tv;
  gettimeofday( &tv, 0 );
  double r = double( tv.tv_sec ) + 1e-6 * double( tv.tv_usec ) - myOrigin_;
  return r;
}

void Unittest_Clock()
{
  I_Clock * c1 = CreateClock();
  ::Sleep( 10 );
  I_Clock * c2 = CreateClock();
  double c1t, c2t;
  c1t = c1->realTime();
  c2t = c2->realTime();
  assert( fabs( c1t - c2t ) < 1e-5 );
  c1->adjust( 100 );
  c1t = c1->realTime();
  c2t = c2->realTime();
  assert( c1t > 99 + c2t );
  c1->adjust( -200 );
  c1t = c1->realTime();
  c2t = c2->realTime();
  assert( c1t < c2t-99 );
  c1->dispose();
  c2->dispose();
}
