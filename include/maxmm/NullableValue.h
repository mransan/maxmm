/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#ifndef maxmm_NullableValue_h 
#define maxmm_NullableValue_h

#include <stdexcept>
#include <string>

namespace maxmm
{

class NullableValueException : public std::runtime_error
{
public:
    NullableValueException(const std::string &what)
    :   std::runtime_error(what)
    {
    
    }
};


template<typename T>
class NullableValue
{
public:
    NullableValue(void);

    NullableValue(const T& value);

    NullableValue(const NullableValue<T> &nullableValue);
    
    NullableValue &operator=(const NullableValue<T> &nullableValue);
    
    bool null(void) const;

    const T& value(void) const;

    T& value(void);

    T& makeValue(void);

    T& makeValue(const T &value);

private:
    T _value;
    bool _null;
};


//
// IMPLEMENTATION
// --------------
//

template<typename T>
NullableValue<T>::NullableValue(void)
:   _value(),
    _null(true)
{

}


template<typename T>
NullableValue<T>::NullableValue(const T& value)
:   _value(value)
   ,_null(false)
{

}

template<typename T>
NullableValue<T>::NullableValue(const NullableValue<T> &nullableValue)
:   _value(nullableValue._value)
   ,_null(nullableValue._null)
{

}

template<typename T>
NullableValue<T>& NullableValue<T>::operator=(const NullableValue<T> &nullableValue)
{
    if(&nullableValue == this)
    {
        return *this;
    }

    _value = nullableValue._value;
    _null  = nullableValue._null;

    return *this;
}

template<typename T>
bool NullableValue<T>::null(void) const
{
    return _null;
}

template<typename T>
const T& NullableValue<T>::value(void) const
{
    if(true == _null)
    {
        throw maxmm::NullableValueException("invalid access");    
    }
    
    return _value;
}

template<typename T>
T &NullableValue<T>::value(void)
{
    if(true == _null)
    {
        throw maxmm::NullableValueException("invalid excess");
    }

    return _value;
}

template<typename T>
T &NullableValue<T>::makeValue(void)
{
    _null = false;
    return _value;
}

template<typename T>
T &NullableValue<T>::makeValue(const T &value)
{
    _null  = false;
    _value = value;
    return _value;
}


}

#endif
