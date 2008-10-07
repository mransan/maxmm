/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_ScopeTimer_h
#define maxmm_ScopeTimer_h

#include <maxmm/Time.h>

namespace maxmm
{
    
    //! \brief provides time measurement for a scope.
    //! 
    //! The scope timer uses RAII schema for scope time measurement.
    //!
    class ScopeTimer
    {
        public:

            //! \brief RAII class that initialize the start and end time in
            //! constructor/destructor.
            //!
            class Ressource
            {
                public:
                    
                    //! \brief set the start time of the timer object.
                    //!
                    //! \param timer the timer defined outside of the measured
                    //!  scope.
                    //!
                    Ressource( maxmm::ScopeTimer &timer)
                    :   _timer( timer )
                    {
                        _timer.start( );
                    }

                    //! \brief destructor set the end time of the timer.
                    //!
                    ~Ressource( void )
                    {
                        _timer.end( );
                    }
                
                private:
                    
                    //! \brief the scope timer reference.
                    maxmm::ScopeTimer &_timer;
            };

            friend class Ressource;

            //! \brief Constructor.
            //!
            ScopeTimer( void )
            :   _start( 0 ) , 
                _end( 0 )
            {
                
            }

            //! \brief returns the time measured by the timer.
            //!
            Time elapsed( void )
            {
                return _end - _start; 
            }
            
            //! \brief return a ressource for use in a scope.
            //!
            //! \return appropriate resource object.
            //!
            Ressource ressource( void )
            {
                return Ressource( *this );
            }
        private:
            
            void start( void )
            {
                _start = Time::now( );    
            }
            void end( void )
            {
                _end = Time::now( );
            }
            
            Time _start;
            Time _end;
    };

}


#endif
