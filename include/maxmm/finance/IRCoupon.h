/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_IRCoupon_h 
#define maxmm_IRCoupon_h

namespace maxmm 
{
namespace finance
{

//! \brief Coupon representation for interest rate gains.
//!
class IRCoupon
{
public:
    //! \brief Constructor
    //! 
    //! \param[in] number: the coupon number
    //! \param[in] is_final: indicate wether or not this coupon is the final
    //! one.
    //! \param[in] value: the value of the coupon.
    //!
    IRCoupon(unsigned int number, bool is_final, double value);
    
    //! \brief number accessor.
    //!
    //! \return the coupon number.
    //!
    unsigned int number(void) const;

    //! \brief is final accessor
    //!
    //! \return true if the coupon is the final one, false otherwise.
    //!
    bool is_final(void) const;

    //! \brief amount accessor.
    //!
    //! \return the amount of the coupon.
    //!
    double amount(void) const;

private:
    unsigned int _number;
    bool _is_final;
    double _amount;
};

bool operator<(IRCoupon const& lhs, IRCoupon const& rhs);


} // namespace finance
} // namespace maxmm 

#endif
