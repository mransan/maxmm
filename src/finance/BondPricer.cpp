/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <maxmm/finance/BondPricer.h>

#include <maxmm/finance/Bond.h>
#include <maxmm/finance/BondCouponCalculator.h>
#include <maxmm/finance/IRCalculator.h>

namespace maxmm
{
namespace finance
{

BondPricer::BondPricer(
    boost::function1<double, double> zero_rate_accessor)
:   _zero_rate_accessor(zero_rate_accessor)
{

}


double
BondPricer::price(Bond const& bond) const
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
        double duration_foy = coupon_itr->second;
        double continous_ir = _zero_rate_accessor(duration_foy);

        InterestRate ir(continous_ir);
        IRCalculator calculator(ir);

        price += calculator.discounted_value(
            duration_foy, 
            coupon_itr->first.amount());
    }
    
    return price;

}

} // namespace finance
} // namespace maxmm
