/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <maxmm/InterestRate.h>

#include <cmath>

namespace maxmm
{

InterestRate::InterestRate(double rate)
:   _rate(rate)
{


}

InterestRate::InterestRate(unsigned int frequency, double rate)
:   _rate(-1.0)
{
    _rate = frequency * log(1 + rate / (double)(frequency));
}


double
InterestRate::rate_continuous_compounding(void) const
{
    return _rate;
}

double
InterestRate::rate_frequency_compounding(unsigned int frequency) const
{
    return frequency * (exp(_rate/(double)(frequency)) - 1);
}

} // namespace maxmm
