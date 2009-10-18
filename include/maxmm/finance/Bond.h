/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#ifndef maxmm_finance_Bond_h 
#define maxmm_finance_Bond_h

#include <maxmm/finance/InterestRate.h>

namespace maxmm
{
namespace finance
{

class Bond
{
public:

    

    //! \brief Constructor
    //!
    //! \param[in] ir: interest rate of the bond.
    //! \param[in] frequency: frequency of the coupon
    //! \param[in] fraction_of_year_duration: the duration of the bond expressed
    //! in fraction of year.
    //! \param[in] principal: the bond principal
    //! 
    Bond(
        InterestRate const& ir,
        unsigned int frequency,
        double fraction_of_year_duration,
        double principal);

    //! \brief interest rate accessor.
    //!
    InterestRate const& ir(void) const;

    //! \brief frequency accessor.
    //!
    unsigned int frequency(void) const;
    
    //! \brief duration accessor.
    //!
    double fraction_of_year_duration(void) const;

    //! \brief principal accessor.
    //!
    double principal(void) const;

private:
    
    InterestRate _ir;
    unsigned int _frequency;
    double _fraction_of_year_duration;
    double _principal;

};


} // namespace finance
} // namespace maxmm

#endif
