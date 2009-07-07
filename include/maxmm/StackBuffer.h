/********************************/
/* copyright (c) 2008           */
/* maxime ransan - jan 2008     */
/* all rights reserved.         */
/********************************/


#ifndef maxmm_StackBuffer_h 
#define maxmm_StackBuffer_h


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

}

        
#endif
