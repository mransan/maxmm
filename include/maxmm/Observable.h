/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_Observable_h
#define maxmm_Observable_h

#include <set>

class ObservableTest;

namespace maxmm
{
    class Observer;
    
    class Observable
    {
    public:
    friend class ::ObservableTest;

    protected:
        Observable();
        virtual ~Observable();
        bool        registerObs(Observer *_obs);
        bool        unregisterObs(Observer *_obs);
        struct Notifier
        {
            Notifier()
            {
            }
            void operator()(Observer *_obs);
        };
        void        notifyAll();
        std::size_t size();
    
    private:
        typedef        std::set<Observer *>             ObserverList;
        typedef        ObserverList::iterator           ObserverListItr;
        typedef        std::pair<ObserverListItr, bool> ObserverListRet ;      
        ObserverList   m_observers;
    };





}



#endif
