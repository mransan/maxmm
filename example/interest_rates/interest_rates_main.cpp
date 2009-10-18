/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <iostream>
#include <maxmm/finance/InterestRate.h>
#include <maxmm/finance/IRCouponCalculator.h>
#include <maxmm/finance/IRCalculator.h>
#include <maxmm/finance/Bond.h>
#include <maxmm/finance/BondCouponCalculator.h>

int main(void)
{
    std::cout << "/** Interest Rates **/" 
              << std::endl;
    

    {
        // semi annual compounding with 10% interest.
        // example 4.1 Hull Book
        std::cout << "IR 10% semi annual."
                  << std::endl;
        maxmm::finance::InterestRate ir(2, 0.10);
    
        std::cout << "continous: "
                  << ir.rate_continuous_compounding()
                  << std::endl;
        std::cout << "semi annual: "
                  << ir.rate_frequency_compounding(2)
                  << std::endl;
        std::cout << "quaterly: "
                  << ir.rate_frequency_compounding(4)
                  << std::endl;
    }
    {
        // semi annual compounding with 10% interest.
        // example 4.2 Hull Book
        std::cout << "IR 0.08 continuous."
                  << std::endl;
        
        maxmm::finance::InterestRate ir(0.08);
    
        std::cout << "continous: "
                  << ir.rate_continuous_compounding()
                  << std::endl;
        std::cout << "semi annual: "
                  << ir.rate_frequency_compounding(2)
                  << std::endl;
        std::cout << "quaterly: "
                  << ir.rate_frequency_compounding(4)
                  << std::endl;
    }


    {
        std::cout << "IR 10% 1 year."
                  << std::endl;
        maxmm::finance::InterestRate ir(1, 0.1);

        maxmm::finance::IRCouponCalculator calc(ir);

        {
            std::cout << "Semi Annual Coupons:"
                      << std::endl;
            std::vector<maxmm::finance::IRCoupon> coupons = calc.calculate(2, 1, 100);

            for(std::vector<maxmm::finance::IRCoupon>::const_iterator 
                itr_coupon  = coupons.begin();
                itr_coupon != coupons.end();
                ++itr_coupon)
            {
                std::cout << "Coupon number: "
                          << itr_coupon->number()
                          << ", is_final: "
                          << itr_coupon->is_final()
                          << ", amount: "
                          << itr_coupon->amount()
                          << "."
                          << std::endl;
            }
        }
        {
            std::cout << "Quaterly Coupons: "
                      << std::endl;
            std::vector<maxmm::finance::IRCoupon> coupons = calc.calculate(4, 1, 100);

            for(std::vector<maxmm::finance::IRCoupon>::const_iterator 
                itr_coupon  = coupons.begin();
                itr_coupon != coupons.end();
                ++itr_coupon)
            {
                std::cout << "Coupon number: "
                          << itr_coupon->number()
                          << ", is_final: "
                          << itr_coupon->is_final()
                          << ", amount: "
                          << itr_coupon->amount()
                          << "."
                          << std::endl;
            }

        }
    }
    {
        std::cout << "IR 0.025 4month."
                  << std::endl;
        maxmm::finance::InterestRate ir(4, 0.025);

        maxmm::finance::IRCouponCalculator calc(ir);
        
        std::cout << "Quaterly Coupons: "
                  << std::endl;
        std::vector<maxmm::finance::IRCoupon> coupons = calc.calculate(4, 1, 100);

        for(std::vector<maxmm::finance::IRCoupon>::const_iterator 
            itr_coupon  = coupons.begin();
            itr_coupon != coupons.end();
            ++itr_coupon)
        {
            std::cout << "Coupon number: "
                      << itr_coupon->number()
                      << ", is_final: "
                      << itr_coupon->is_final()
                      << ", amount: "
                      << itr_coupon->amount()
                      << "."
                      << std::endl;
        }
    }
    
    {
        std::cout << "IR 5% continuous."
                  << std::endl;
        
        maxmm::finance::InterestRate ir(0.05);
     
        maxmm::finance::IRCalculator calc(ir);

        std::cout << "5 years amount with 100 initial amount:"
                  << calc.interestValue(5, 100)
                  << std::endl;
        std::cout << "Initial amount required so that after 5 years "
                  << "the final amount is 100: "
                  << calc.discountedValue(5, 100)
                  << std::endl;
    }
    {
        std::cout << "Bond 100 principal 6% semi annual."
                  << std::endl;
    
        maxmm::finance::InterestRate ir(2, 0.06);
        maxmm::finance::Bond bond(ir, 2, 2.0, 100);
        
        typedef std::vector<std::pair<
            maxmm::finance::IRCoupon, 
            double> >                           Coupons;
        typedef Coupons::const_iterator         CouponsCItr;

        std::vector<std::pair<maxmm::finance::IRCoupon, double> > coupons
            = maxmm::finance::BondCouponCalculator::calculate(1.9, bond);
        
        for(CouponsCItr itr_coupon = coupons.begin();
            itr_coupon != coupons.end();
            ++itr_coupon)
        {
            std::cout << "Coupon number: "
                      << itr_coupon->first.number()
                      << ", is_final: "
                      << itr_coupon->first.is_final()
                      << ", amount: "
                      << itr_coupon->first.amount()
                      << ", fraction of year: "
                      << itr_coupon->second
                      << "."
                      << std::endl;
        }
    }
    return 0;
}
