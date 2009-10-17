/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#include <maxmm/IRCoupon.h>

namespace maxmm
{

IRCoupon::IRCoupon(unsigned int number, bool is_final, double amount)
:   _number(number),
    _is_final(is_final),
    _amount(amount)
{

}

unsigned int
IRCoupon::number(void) const
{
    return _number;
}

bool
IRCoupon::is_final(void) const
{
    return _is_final;
}

double
IRCoupon::amount(void) const
{
    return _amount;
}


bool 
operator<(IRCoupon const& lhs, IRCoupon const& rhs)
{
    return lhs.number() < rhs.number();
}

} // namespace maxmm

