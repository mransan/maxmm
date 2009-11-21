/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <maxmm/finance/BondCouponCalculator.h>

#include <maxmm/finance/Bond.h>
#include <maxmm/finance/IRCalculator.h>

#include <cmath>

namespace maxmm
{
namespace finance
{

/** static **/
std::vector<std::pair<IRCoupon, double> >
BondCouponCalculator::calculate(
    double fraction_of_year_elapsed,
    Bond const& bond)
{
    double remaining_duration 
        = bond.length() - fraction_of_year_elapsed;
    
    // return no coupons.
    if(remaining_duration < 0)
    {
        std::vector<std::pair<IRCoupon, double> > coupons;
        return coupons;
    }

    double full_coupon_duration_foy 
        = 1.0 / bond.coupon_frequency();

    double number_of_coupon_foy
        = remaining_duration / full_coupon_duration_foy;
    
    unsigned int number_of_coupon 
        = ceil(number_of_coupon_foy);
    
    double first_coupon_duration_foy = 0.0;
    if(number_of_coupon != 0)
    {
        first_coupon_duration_foy
            = remaining_duration - 
              ((number_of_coupon -1) * full_coupon_duration_foy);
    } 
    
    IRCalculator calc(bond.ir());
    double coupon_value 
        = calc.interest_value(
            (1.0/bond.coupon_frequency()), 
            bond.principal()) -
          bond.principal(); 

    std::vector<std::pair<IRCoupon, double> > coupons;
    if(number_of_coupon == 1 || number_of_coupon == 0)
    {
        coupons.push_back(std::make_pair(
            IRCoupon(1, true, coupon_value + bond.principal()),
            first_coupon_duration_foy));
        return coupons;
    }
    
    // insert the first coupon
    coupons.push_back(std::make_pair(
            IRCoupon(1, false, coupon_value),
            first_coupon_duration_foy));
    
    double coupon_total_duration_foy 
        = first_coupon_duration_foy;
    for(unsigned int i_coupon = 2; i_coupon < number_of_coupon ; ++i_coupon)
    {
        coupon_total_duration_foy += full_coupon_duration_foy;

        coupons.push_back(std::make_pair(
            IRCoupon(i_coupon, false, coupon_value),
            coupon_total_duration_foy));
    }
    coupon_total_duration_foy += full_coupon_duration_foy;
    
    // insert the last coupon.
    coupons.push_back(std::make_pair(
        IRCoupon(number_of_coupon, true, bond.principal() + coupon_value),
        coupon_total_duration_foy));

    return coupons;
}

} // namespace finance
} // namespace maxmm
