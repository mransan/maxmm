/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/

#include <maxmm/Exception.h>


namespace maxmm
{

Exception::Exception(const std::string &str)
:   std::runtime_error(str.c_str())
{

}

}
