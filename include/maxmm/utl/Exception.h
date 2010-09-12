/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/

#ifndef maxmm_Exception_h
#define maxmm_Exception_h

#include <stdexcept>


namespace maxmm
{

class Exception : public std::runtime_error
{
    Exception(const std::string &str);
};


}

#endif
