/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef __maxmm_Random_h__
#define __maxmm_Random_h__

#include <boost/random.hpp>
#include <maxmm/Time.h>

namespace maxmm
{
namespace random
{

template< typename T >
class Uniform
{
public:

    Uniform(T lower_bound , T upper_bound)
    : _nb_generator(static_cast< unsigned int >(Time::nano())) , 
      _distribution(lower_bound , upper_bound) , 
      _var_generator(_nb_generator , _distribution)
    {
        // No - Op.
    }

    ~Uniform(void)
    {
    
    }

    T operator()(void)
    {
        return _var_generator();
    }

private:
    
    //! \brief number generator.
    //!
    boost::mt19937 _nb_generator;
    
    //! \brief distribution.
    //!
    boost::uniform_int< T > _distribution;
    
    //! \brief variable generator.
    //!
    boost::variate_generator
                <boost::mt19937,
                 boost::uniform_int< T >
                >    _var_generator; 
};
}
}


#endif

