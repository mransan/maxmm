/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#include <maxmm/finance/IRCouponCalculator.h>

namespace maxmm
{
namespace finance
{

IRCouponCalculator::IRCouponCalculator(InterestRate const& ir)
:   _ir(ir)
{

}


std::vector<IRCoupon> 
IRCouponCalculator::calculate(
    unsigned int frequency,
    unsigned int number_of_year,
    double initial_amount) const
{
    double final_amount = initial_amount;
    double rate = 
        _ir.rate_frequency_compounding(frequency) / (double)(frequency);
    
    std::vector<IRCoupon> coupons;
    unsigned int total_number_of_coupons = number_of_year * frequency;

    for(unsigned int i_coupon = 0; 
        i_coupon<total_number_of_coupons - 1; 
        ++i_coupon)
    {
        double coupon_value = final_amount * rate;
        final_amount += coupon_value;
        coupons.push_back(
            maxmm::finance::IRCoupon(
                i_coupon + 1,
                false,
                coupon_value));
    }
    
    final_amount += final_amount * rate;
    coupons.push_back(
        maxmm::finance::IRCoupon(
            total_number_of_coupons,
            true,
            final_amount));
    return coupons;
}

} // namespace finance
} // namespace maxmm
