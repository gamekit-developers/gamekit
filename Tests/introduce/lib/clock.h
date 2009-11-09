
#if !defined( clock_h )
#define clock_h

namespace libintroduce {

class I_Clock {
  public:
    virtual void dispose() = 0;
    virtual double realTime() = 0;
    virtual void adjust( double delta ) = 0;
};

I_Clock * CreateClock();

}

#endif  //  clock_h
