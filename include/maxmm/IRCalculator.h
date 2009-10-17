/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#ifndef maxmm_IRCalculator_h 
#define maxmm_IRCalculator_h

#include <maxmm/InterestRate.h>

namespace maxmm
{

//! \brief Calculate received interest on an initial amount
//!
class IRCalculator
{
public:

    //! \brief Constructor
    //! 
    //! \param[in] ir: interest rate to use during calculation.
    IRCalculator(InterestRate const& ir);

    //! \brief calculate the value of an amount after a period time 
    //! 
    //! \param[in] fraction_of_year: period over which interests are gained.
    //! \param[in] initial_amount: the initial amount invested.
    //! 
    //! \return the final value of the investment.
    //!
    double interestValue(
        double fraction_of_year,
        double initial_amount) const;
    
    //! \brief calculate the discounted value of a final amount. (ie the amount 
    //! to invest so to have the given amount at the end of the period of time)
    //!
    //! \param[in] fraction_of_year: period over which interest are gained.
    //! \param[in] final_amout: the expected amount at the end of the given 
    //! period of time.
    //!
    //! \return the initial amount required.
    //!
    double discountedValue(
        double fraction_of_year,
        double final_amount) const;
private:
    
    InterestRate _ir;
};

} // namespace maxmm

#endif
