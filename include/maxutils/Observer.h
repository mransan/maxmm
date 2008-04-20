/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef MAXUTILS_OBSERVER_H
#define MAXUTILS_OBSERVER_H


namespace maxutils
{
    class Observable;

    class Observer
    {
    friend class Observable;
    public:
        Observer()
        {
        }
        virtual ~Observer()
        {
        }
    protected:
        virtual void notify() = 0;
    };
}

#endif
