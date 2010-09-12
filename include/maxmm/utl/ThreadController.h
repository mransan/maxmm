/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_ThreadController_h
#define maxmm_ThreadController_h

#include <maxmm/Time.h>
#include <maxmm/Condition.h>

#include <functional>

namespace maxmm
{
    
//! \brief run the loop an infinite time.
//!
class NoWaitController
{
public:
   bool execute(void)
   {
      return true;    
   }
};

//! \brief Run the thread loop Once.
//!
class OnceController
{
public:
    OnceController(void)
    :   _run(true)
    {
            
    }

    bool execute(void)
    {
        bool ret = _run;
        _run = false;
        return ret;
    }
private:
    bool _run;
};

//! \brief controller for real time execution of the thread loop.
//!
class TimedController
{
public:
    TimedController(const Time &period)
    :   _period(period),
        _next(0)
    {
        
    }

    bool execute(void)
    {
        Time now = Time::now();
        if(_next == 0)
        {
            _next = now +  _period;
            return true;
        }
        Time::sleep_until(_next);
        _next += _period;
    }
private:
    Time _period;
    Time _next;
};

class ConditionController
{
    public:
        ConditionController(Condition &condition)
        :    _condition(condition),
            _continue(true)
        {
             
        }
        
        ConditionController(const ConditionController& controller)
        :   _condition(controller._condition),
            _continue(controller._continue)
        {}
        bool execute(void)
        {
            if(_continue == true)
            {
                _continue = false;
                return true;
            }
            _condition.wait();
            return true;
        }
        void no_wait(void)
        {
            _continue = true;
        }
    private:
       Condition &_condition;
       bool _continue;

};


//! \brief Composite Controller which allows 2 controller to be run at each
//! loop. 
//!
//! The aggregation of the controller result is decided by the thir template
//! parameter which can either be std::logical_and< bool > (default) 
//! and std::logical_or< bool >.
//!
//! Note that the contructor takes 2 const reference to the controllers and
//! it makes a copy of it.
//!
//! \code
//! // Here is how to declare such a controller.
//! maxmm::CompositeController<
//!     maxmm::OnceController ,
//!     maxmm::NoWaitController > controller(
//!         (OnceController()) ,
//!         (NoWaitController()));
//! \endcode
//!
template< 
    typename Controller1 , 
    typename Controller2 , 
    typename BinaryPredicate = std::logical_and<bool> >
class CompositeController
{
public:
    CompositeController(const Controller1 &c1 , const Controller2 &c2)
    :   _controller1(c1),
        _controller2(c2) ,
        _binary_predicate(BinaryPredicate())
    {
        
    }
    
    bool execute(void)
    {
        bool ret_c1 = _controller1.execute();
        bool ret_c2 = _controller2.execute();
        
        return _binary_predicate(ret_c1 , ret_c2); 
    }
private:
    
    Controller1 _controller1;
    Controller2 _controller2;
    BinaryPredicate _binary_predicate;
};

}



#endif
