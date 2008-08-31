/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef __maxmm_LockFreeWrapper_h__
#define __maxmm_LockFreeWrapper_h__

#include <boost/utility.hpp>
#include <apr_atomic.h>

#include <list>
#include <vector>
#include <cassert>

namespace maxmm
{
    
    
    //! \brief container for all the hazard pointer.
    //!
    //! This class encapsulate t single linked list.
    //! Could not use STL container to do so since we need to do modification of
    //! the list ATOMICALLY.
    //!
    //! This class is also a singleton since the only way to access it is via
    //! its ::head( ) method.
    //!
    class HzPtrRec
    {
        //! \brief indicate that hazard pointer is in use.
        //!
        volatile uint32_t           _active;

        //! \brief constructor.
        //!
        HzPtrRec( void );

        //! \brief pointer to the next harzard pointer in the list.
        //!
        HzPtrRec           *_next;
        
        //! \brief pointer to the actual object to protect.
        //!
        void               *_hazard_ptr;
        
        //! \brief head of the list.( first element )
        //!
        static HzPtrRec     *_head;
        
        //! \brief total length of the list.
        //!
        static volatile uint32_t      _length;
       
    public:

        //! \brief return the underlying hazard pointer.
        //!
        //! \return the underlying hazard pointer.
        //!
        const void * hazard_ptr( void ) const ;
        
        
        //! \brief set the value of the hazard pointer.
        //!
        //! \param ptr the new ptr to read-protect.
        //!
        void hazard_ptr( void * ptr );
       
        //! \brief assign the given hazard pointer to its next value.( i.e.
        //! increment it ).
        //!
        //! \param hz_ptr ref to hazard pointer rec to increment.
        //!
        static void  next( HzPtrRec *& hz_ptr);
        
        //! \brief return the head of the list of hazard pointer.
        //!
        //! \return head of the list.
        //!
        static HzPtrRec* head( void );
        
        //! \brief get an hazard pointer for reading a Wait free object.
        //!
        //! \return  a unique hazard pointer to be used for reading a lock free
        //!  structure.
        //!
        static HzPtrRec* acquire( void );
       
        //! \brief release a previously acquired hazard pointer 
        //!
        //! \param p the pointer to release.
        //!
        static void release( HzPtrRec *p );
    };
    


    //! \brief this class allows any object to be a lock free structure.
    //!
    template< typename T >
    class LockFreeWrapper : public boost::noncopyable
    {
        private:

            //! \brief pointer to the current object
            //!
            T     *_ptr;
            
            //! \brief limit of number of allocated and unused object to keep.
            //!
            int    _limit;
        
        public:

            //! \brief retired list for unused object.
            //!
            typedef std::list< T * >                TRetiredList;
            
            //! \brief iterator for the retired list.
            //! 
            typedef typename TRetiredList::iterator TRetiredListItr;

            
            //! \brief constructor.
            //!
            //! \param limit the maximum number of allocated and retired object.
            //! \param ptr the pointer to data to manage. This is a transfer of
            //!  ownership.
            //!
            LockFreeWrapper( int limit, T * ptr )
            {
                assert( ptr != 0 );
                _ptr   = ptr;
                _limit = limit; 
            }

            //! \brief Constructor.
            //!
            //! \param limit maximum number of allocated and retired object.
            //!
            LockFreeWrapper( int limit )
            {
                _ptr   = new T( );
                _limit = limit; 
            }
            
            //! \brief Destructor.
            //!
            ~LockFreeWrapper( void )
            {
                if( _ptr ) 
                {
                    delete _ptr;
                }
            }
            
            //! \brief update the current pointer with new data. Retire the
            //!  previous pointer.
            //!
            //! \param new_ptr the new pointer to manage.
            //! \param retired_list the list of retired pointer for the current
            //!  thread.
            //!
            void update( T* new_ptr, TRetiredList &retired_list )
            {
                T* old_ptr;
                do
                {
                    old_ptr   = _ptr;   
                }
                while( old_ptr != apr_atomic_casptr( ( volatile void** )( &_ptr ) , new_ptr, old_ptr ) );

                retired_list.push_back( old_ptr );
                
                if( retired_list.size( ) > _limit )
                {
                    LockFreeWrapper< T >::remove_retired( retired_list );
                }
            }
            

            //! \brief obtain a read_ptr for the stored object.
            //!
            //! \param record the hazard pointer obtain from the global HzPtrRec
            //!  object.
            //! \return a read pointer correctly register with the hazard
            //!  pointer.
            //!
            T* get_read_ptr( HzPtrRec *record ) const 
            {
                T* read_ptr = 0 ;
                do
                {
                    read_ptr = _ptr;
                    record->hazard_ptr( read_ptr );
                }
                while( read_ptr != _ptr );
                return read_ptr;
            }

            //! \brief perform retired list cleanup. This method deallocate
            //!  object that are no longer in used.
            //!
            //! \param retired list to cleanup.
            //! 
            static void remove_retired( TRetiredList &retired_list )
            {
                std::vector< const void * > hazardPtrList;
                HzPtrRec *head = HzPtrRec::head();
                while( head )
                {
                    const void *p = head->hazard_ptr( );
                    if( p )
                    {
                        hazardPtrList.push_back(p);
                    }
                    HzPtrRec::next( head ); 
                }
                std::sort(  hazardPtrList.begin(), 
                            hazardPtrList.end()  , 
                            std::less<const void *>()   );
                for ( TRetiredListItr 
                        itr  = retired_list.begin();
                        itr != retired_list.end()  ;
                        itr ++)
                {
                    if( std::binary_search(
                                hazardPtrList.begin(), 
                                hazardPtrList.end(), 
                                *itr) == false )
                    {
                        delete *itr;
                        itr = retired_list.erase(itr);
                    }
                }
            }       
    };
        
    //! \brief resource management for obtaining a read-only pointer of the
    //!  lock-free object.
    //! 
    //! This class take care of acquiring the record and releasing it on
    //! destruction.
    //! 
    template< typename T >
    class LockFreePtr
    {
    
        private:
            //! \brief record to keep the object from being deallocated.
            //!
            HzPtrRec *_record;
            
            //! \brief underlying ptr.
            //!
            T* _ptr;
        public:
            //! \brief constructor
            //! 
            //! Acquire the record as well as the pointer to the object.
            //!
            //! \param[in]  data the lock free object.
            //! \param[out] read-only pointer.
            //!
            LockFreePtr( LockFreeWrapper< T > &data )
            {
                _record = HzPtrRec::acquire();
                _ptr = data.get_read_ptr( _record );
            }

            T* operator->( void ) 
            {
                assert( _ptr );
                return _ptr;
            }
            
            T& operator*( void ) 
            {
                assert( _ptr );
                return *_ptr;
            }
           
            //! \brief release the lock
            //!
            ~LockFreePtr()
            {
                HzPtrRec::release(_record);
        }   
    };
   
}//namespace maxmm


#endif
