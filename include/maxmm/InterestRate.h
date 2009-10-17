/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#ifndef maxmm_InterestRate_h 
#define maxmm_InterestRate_h


namespace maxmm
{

class InterestRate
{
public:

    //! \brief Constructor with rate of a continuous
    //! compounding.
    //!
    //! \param[in] rate: rate with continuous compounding.
    //!
    InterestRate(double rate);

    //! \brief Constructor with frequency (per annum) and associated
    //! rate.
    //!
    //! \param[in] frequency: the frequency per annum at which the interest
    //! rate is compounding.
    //!
    InterestRate(unsigned int frequency, double rate);

    //! \brief return the interest rate with continuous compounding.
    //! 
    //! \return interest rate with continous compounding.
    //!
    double rate_continuous_compounding(void) const;
    
    //! \brief return the intereset rate with frequency compounding.
    //!
    //! \return interest rate with frequency compounding.
    double rate_frequency_compounding(unsigned int frequency) const;
    
private:
    
    //! \brief continuous compounding rate
    double _rate;

};
} // namespace maxmm

#endif
