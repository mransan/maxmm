/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <boost/function.hpp>

#ifndef maxmm_finance_BondPricer_h 
#define maxmm_finance_BondPricer_h

//
// Forward declaration
// -------------------
//
namespace maxmm { namespace finance {  class Bond; } }

namespace maxmm
{
namespace finance
{

//! \brief Pricing utility for bonds.
//!
class BondPricer
{
public:
    
    //! \brief price the given bond
    //!
    //! \param[in] bond: the bond to price.
    //! 
    //! \return the price which will be in the same unit as the bond principal
    //!
    static double price(
        boost::function1<double, double> zero_rate_accessor,
        Bond const& bond); 

    //! \brief bond yield calculation (Hull 4.4 Bond Yield)
    //!
    //! \param[in] bond: the bond on which to calculate the bond yield.
    //! \param[in] price: the bond price
    //!
    //! \return the bond yield.
    //!
    static double bond_yield(Bond const& bond, double price);

private:
    
    BondPricer(void);
    BondPricer(BondPricer const&);
    Bond& operator=(BondPricer const&);
};

} // namespace finance
} // BloombergLP

#endif
