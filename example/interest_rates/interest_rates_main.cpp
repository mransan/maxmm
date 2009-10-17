/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <iostream>
#include <maxmm/InterestRate.h>
#include <maxmm/IRCouponCalculator.h>
#include <maxmm/IRCalculator.h>

int main(void)
{
    std::cout << "/** Interest Rates **/" 
              << std::endl;
    

    {
        // semi annual compounding with 10% interest.
        // example 4.1 Hull Book
        std::cout << "IR 10% semi annual."
                  << std::endl;
        maxmm::InterestRate ir(2, 0.10);
    
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
        
        maxmm::InterestRate ir(0.08);
    
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
        maxmm::InterestRate ir(1, 0.1);

        maxmm::IRCouponCalculator calc(ir);

        {
            std::cout << "Semi Annual Coupons:"
                      << std::endl;
            std::vector<maxmm::IRCoupon> coupons = calc.calculate(2, 1, 100);

            for(std::vector<maxmm::IRCoupon>::const_iterator 
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
            std::vector<maxmm::IRCoupon> coupons = calc.calculate(4, 1, 100);

            for(std::vector<maxmm::IRCoupon>::const_iterator 
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
        maxmm::InterestRate ir(4, 0.025);

        maxmm::IRCouponCalculator calc(ir);
        
        std::cout << "Quaterly Coupons: "
                  << std::endl;
        std::vector<maxmm::IRCoupon> coupons = calc.calculate(4, 1, 100);

        for(std::vector<maxmm::IRCoupon>::const_iterator 
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
        
        maxmm::InterestRate ir(0.05);
     
        maxmm::IRCalculator calc(ir);

        std::cout << "5 years amount with 100 initial amount:"
                  << calc.interestValue(5, 100)
                  << std::endl;
        std::cout << "Initial amount required so that after 5 years "
                  << "the final amount is 100: "
                  << calc.discountedValue(5, 100)
                  << std::endl;
    }
    return 0;
}
