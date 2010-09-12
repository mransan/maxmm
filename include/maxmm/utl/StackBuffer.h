/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/


#ifndef maxmm_StackBuffer_h 
#define maxmm_StackBuffer_h


#include <boost/type_traits/is_class.hpp>

#include <stdint.h>

namespace maxmm
{

template<typename T>
struct StackBuffer
{
    union Buffer 
    {
        uint32_t _alignement;
        char _begin[sizeof(T)];
    };

    Buffer _buffer;
};

template<typename T>
class StackBufferAccessor
{
public:    
    StackBufferAccessor(StackBuffer<T> *buffer);
 

    T* get(void);
    const T* get(void) const;

    T* make(void);
    
    T* make(const T& value);

    void destroy(void);

private:
    void destroy(boost::true_type is_class);
    void destroy(boost::false_type is_class);

private:
    StackBuffer<T> *_buffer;
};



// 
// IMPLEMENTATION
// -------------
//

template<typename T>
StackBufferAccessor<T>::StackBufferAccessor(StackBuffer<T> *buffer)
:   _buffer(buffer)
{

}


template<typename T>
T* StackBufferAccessor<T>::get(void)
{
    return reinterpret_cast<T*>(&_buffer->_buffer._begin[0]);
}

template<typename T>
const T* StackBufferAccessor<T>::get(void) const
{
    return reinterpret_cast<const T*>(&_buffer->_buffer._begin[0]);
}

template<typename T>
T* StackBufferAccessor<T>::make(void)
{
    T* tmp = new (&_buffer->_buffer._begin[0]) T();
    return tmp;
}

template<typename T>
T* StackBufferAccessor<T>::make(const T& value)
{
    T* tmp = new (&_buffer->_buffer._begin[0]) T(value);
    return tmp;
}

template<typename T>
void StackBufferAccessor<T>::destroy(void) 
{
    destroy(typename boost::is_class<T>::type());
}

template<typename T>
void StackBufferAccessor<T>::destroy(boost::true_type is_class)
{
    T* tmp = get();
    tmp->~T();
}

template<typename T>
void StackBufferAccessor<T>::destroy(boost::false_type is_class)
{
    // do nothing.
}


}

        
#endif
