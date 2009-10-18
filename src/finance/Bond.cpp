/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <maxmm/finance/Bond.h>

namespace maxmm
{
namespace finance
{

Bond::Bond(
    InterestRate const& ir,
    unsigned int frequency,
    double fraction_of_year_duration,
    double principal)
:   _ir(ir),
    _frequency(frequency),
    _fraction_of_year_duration(fraction_of_year_duration),
    _principal(principal)
{

}

InterestRate const& 
Bond::ir(void) const
{
    return _ir;
}


unsigned int 
Bond::frequency(void) const
{
    return _frequency;
}

double 
Bond::fraction_of_year_duration(void) const
{
    return _fraction_of_year_duration;
}

double
Bond::principal(void) const
{
    return _principal;
}

} // namespace finance
} // namespace maxmm
