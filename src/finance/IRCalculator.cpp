/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <maxmm/finance/IRCalculator.h>

#include <cmath>

namespace maxmm
{
namespace finance
{

IRCalculator::IRCalculator(InterestRate const& ir)
:   _ir(ir)
{

}


double
IRCalculator::interest_value(
    double fraction_of_year,
    double initial_amount) const
{
    double increase_factor 
        = exp(_ir.rate_continuous_compounding() * fraction_of_year);

    return initial_amount * increase_factor; 
}

double
IRCalculator::discounted_value(
    double fraction_of_year,
    double final_amount) const
{
    double increase_factor 
        = exp( -1 *  _ir.rate_continuous_compounding() * fraction_of_year);

    return final_amount * increase_factor; 
}


} // namespace finance
} // namespace maxmm

