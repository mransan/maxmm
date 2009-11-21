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
    unsigned int coupon_frequency,
    double length,
    double principal)
:   _ir(ir),
    _coupon_frequency(coupon_frequency),
    _length(length),
    _principal(principal)
{

}

InterestRate const& 
Bond::ir(void) const
{
    return _ir;
}


unsigned int 
Bond::coupon_frequency(void) const
{
    return _coupon_frequency;
}

double 
Bond::length(void) const
{
    return _length;
}

double
Bond::principal(void) const
{
    return _principal;
}

} // namespace finance
} // namespace maxmm
