/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_LockFreeQueue_h
#define maxmm_LockFreeQueue_h

#include <apr_atomic.h>
#include <assert.h>
namespace maxmm
{

namespace test { class LockFreeQueueTest; }

//! \brief Lock-free implementation of a Queue. The queue is implemented
//! using the Michael and Scott paper as a single link list.
//!
//! This is single-link list implementation is not efficient and is not the
//! one use for the STL. It is more efficient to use preallocated memory in a
//! std::vector fashion.
//!
template< typename T >
class LockFreeQueue
{
friend class test::LockFreeQueueTest;
public:

    //! \brief Constructor.
    //!
    LockFreeQueue(void)
    :   _tail(0) , _head(0) 
    {
        _head = new ElementBase();
        _head->_next = 0;
        _tail = _head;
    }
    
    //! \brief Destructor.
    //!
    //! Delete the remaining nodes.
    //!
    ~LockFreeQueue(void)
    {
        volatile ElementBase * it = _head;
        while(it->_next)
        {
            volatile ElementBase * it_next = it->_next;
            delete it;
            it = it_next;
        }
        delete it;
    }

    
    //! \brief Mutlti threaded push which append a node at the end 
    //! of the list.
    //!
    //! \param t the value to insert.
    //!
    void push_mt(const T &t)
    {
        Element< T > * new_elmnt = new Element< T >(t);
        
        // only insert the new node when tails points to the last node(
        // ie _next == 0
        // 
        // If the CAS fails that mean that another push is being
        // executed and it has not yet updated the _tail pointer to the
        // last element. The while loop is here to "wait" for it to be
        // done.
        //
        volatile ElementBase *local_tail = 0;
        do
        {
            local_tail = _tail;
        }
        while(apr_atomic_casptr(
                (volatile void **)(&(local_tail->_next)), 
                (void *)(new_elmnt) ,
                (void *)(0)) != 0);

        apr_atomic_casptr(
            (volatile void **)(&_tail) ,
            (void *)(new_elmnt) , 
            (void *)(local_tail)) == local_tail ;
        
    }

    //! \brief single threaded push which append the value at the end of
    //! the list.
    //!
    //! \param t the value to append.
    //!
    void push_st(const T &t)
    {
        Element< T > * new_elmnt = new Element< T >(t);

        _tail->_next = new_elmnt ;
        _tail = new_elmnt ;
    }
    
    
    //! \brief Multi threaded retrieval of the value at the top of the queue. 
    //!
    //! \param[out] t the value at the top of the list.
    //! \return true if a value was returned false otherwise.
    //!
    bool pop_mt(T& t)
    {
        volatile ElementBase *local_head = 0 ;
        do
        {
            local_head = _head;
            if(local_head->_next == 0) { return false; }
            t = static_cast< volatile Element< T > * >(local_head->_next)->_data;
        }
        while(apr_atomic_casptr(
            (volatile void **) (&_head) , 
            (void *) (local_head->_next) , 
            (void *) (local_head)) != local_head);
        delete local_head;
        
        return true;
    }
    
    
    //! \brief Single threaded retrieval of the value at the top of the queue. 
    //!
    //!
    //! \param[out] t the value at the top of the list.
    //! \return true if a value was returned false otherwise.
    //!
    bool pop_st(T &t)
    {
        if(_head->_next == 0) 
        { 
            return false; 
        }
        
        volatile ElementBase * old_head = _head; 
        t = static_cast< volatile Element< T > * >(_head->_next)->_data;
        _head = _head->_next;
        delete old_head;
        return true;
    }
    
    //! \brief indicate if the queue is empty.
    //!
    //! \return true if the list is empty.
    //!
    bool empty(void)  const 
    {
        return (0 == _head->_next) ? true : false;
    }
    
private:
    
    struct ElementBase
    {
        ElementBase(void)
        :   _next(0)
        { }
        virtual ~ElementBase(void)
        { }

        volatile ElementBase *_next;
    };
    
    template< typename TS >
    struct Element : public ElementBase 
    {
        Element(void)
        : ElementBase() , _data() 
        { }
        
        Element(const TS& data)
        : ElementBase() , _data(data) 
        { }

        virtual ~Element(void)
        { }
        
        TS _data;
    };

    volatile ElementBase *_tail;
    volatile ElementBase *_head;
};

} // namespace maxmm



#endif

