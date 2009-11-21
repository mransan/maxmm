/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <maxmm/finance/BondPricer.h>

#include <maxmm/finance/Bond.h>
#include <maxmm/finance/BondCouponCalculator.h>
#include <maxmm/finance/IRCalculator.h>

#include <cmath>

namespace maxmm
{
namespace finance
{

/** static **/
double
BondPricer::price(
    boost::function1<double, double> zero_rate_accessor,
    Bond const& bond) 
{

    typedef std::vector<std::pair<IRCoupon, double> > Coupons;
    typedef Coupons::const_iterator                   CouponsCItr;
    
    std::vector<std::pair<IRCoupon, double> > coupons
        = BondCouponCalculator::calculate(0.0, bond);

    double price = 0.0;
    for(CouponsCItr coupon_itr = coupons.begin();
        coupon_itr != coupons.end();
        ++coupon_itr)
    {
        double duration = coupon_itr->second;
        double continous_ir = zero_rate_accessor(duration);

        InterestRate ir(continous_ir);
        IRCalculator calculator(ir);

        price += calculator.discounted_value(
            duration, 
            coupon_itr->first.amount());
    }
    
    return price;

}

/** static **/
double 
BondPricer::bond_yield(
    Bond const& bond,
    double price) 
{
    typedef std::vector<std::pair<IRCoupon, double> > Coupons;
    typedef Coupons::const_iterator                   CouponsCItr;
    
    std::vector<std::pair<IRCoupon, double> > coupons
        = BondCouponCalculator::calculate(0.0, bond);

    double y           = 0.05;
    double price_ratio = 1;
    
    while(price_ratio > 0.01)
    {
        double f_y       = -price;
        double f_prime_y = 0.0; 
    
        InterestRate ir(y);
        IRCalculator calculator(ir);
        
        for(CouponsCItr coupon_itr = coupons.begin();
            coupon_itr != coupons.end();
            ++coupon_itr)
        {
            double duration = coupon_itr->second;
            f_y += calculator.discounted_value(
                duration,
                coupon_itr->first.amount());
            
            f_prime_y += - duration * calculator.discounted_value(
                duration,
                coupon_itr->first.amount());

        }

        price_ratio = f_y/price;
        y = y - (f_y/f_prime_y);
    }
    
    return y;
}

} // namespace finance
} // namespace maxmm
