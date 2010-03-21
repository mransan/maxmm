/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/


#ifndef maxmm_NullableValue_h 
#define maxmm_NullableValue_h

#include <stdexcept>
#include <string>


#include <maxmm/StackBuffer.h>

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
    ~NullableValue(void);

    NullableValue(void);

    NullableValue(const T& value);

    NullableValue(const NullableValue<T> &nullableValue);
    
    NullableValue &operator=(const NullableValue<T> &nullableValue);
    
    bool null(void) const;

    const T& value(void) const;

    T& value(void);

    T& make_value(void);

    T& make_value(const T &value);
    
    void make_null(void);
private:
    typedef StackBufferAccessor<T> Accessor;
    
    StackBuffer<T> _buffer;
    Accessor _accessor;
    bool _null;
};

//
// IMPLEMENTATION
// --------------
//

template<typename T>
NullableValue<T>::~NullableValue(void)
{
    if(!_null)
    {
        _accessor.destroy();
    }
}


template<typename T>
NullableValue<T>::NullableValue(void)
:   _accessor(&_buffer)
    ,_null(true)
{

}


template<typename T>
NullableValue<T>::NullableValue(const T& value)
:   _accessor(&_buffer),
    _null(false)
{
    _accessor.make(value);
}

template<typename T>
NullableValue<T>::NullableValue(const NullableValue<T> &nullableValue)
:   _accessor(&_buffer),
    _null(nullableValue._null)
{
    if(false == _null)
    {
       _accessor.make(nullableValue.value());
    }
}

template<typename T>
NullableValue<T>& NullableValue<T>::operator=(const NullableValue<T> &nullableValue)
{
    if(&nullableValue == this)
    {
        return *this;
    }
    if(_null == nullableValue._null)
    {
        if(!_null)
        {
            value() = nullableValue.value();
        }
    }
    else
    {
        _null  = nullableValue._null;
        if(!_null)
        {
            // this means that before we were null and not anymore.
            _accessor.make(nullableValue.value());
        }
        else
        {
            // we should destroy the buffer.
        }
    }
    
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
    
    return *(_accessor.get());
}

template<typename T>
T &NullableValue<T>::value(void)
{
    if(true == _null)
    {
        throw maxmm::NullableValueException("invalid excess");
    }
        
    return *(_accessor.get());
}

template<typename T>
T &NullableValue<T>::make_value(void)
{
    if(!_null)
    {
        _accessor.destroy();
    }
    else
    {
        _null = false;
    }
    
    return *(_accessor.make());
}

template<typename T>
T &NullableValue<T>::make_value(const T &value)
{
    if(!_null)
    {
        _accessor.destroy();
    }
    else
    {
        _null  = false;
    }
    return *(_accessor.make(value));
}

template<typename T>
void NullableValue<T>::make_null(void)
{
    if(!_null)
    {
        _accessor.destroy();
    }
    _null = true;
}

}

#endif
