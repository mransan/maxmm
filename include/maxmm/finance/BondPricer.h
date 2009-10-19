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
//! This class needs the zero rate accessor to be able to price 
//! a bond.
//!
class BondPricer
{
public:
    
    //! \brief Constructor
    //! 
    //! \param[in] zero_rate_accessor: functor allowing the access to 
    //! zero rate value. Given a number of year returns the zero rate
    //!
    BondPricer(
        boost::function1<double, double> zero_rate_accessor);
    
    //! \brief price the given bond
    //!
    //! \param[in] bond: the bond to price.
    //! 
    //! \return the price which will be in the same unit as the bond principal
    //!
    double price(Bond const& bond) const;

private:
    
    boost::function1<double, double> _zero_rate_accessor;
};

} // namespace finance
} // BloombergLP

#endif
