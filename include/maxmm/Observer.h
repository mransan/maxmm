/********************************/
/* Copyright (c) 2008           */
/* Maxime Ransan - Jan 2008     */
/* All rights reserved.         */
/********************************/

#ifndef maxmm_Observer_h
#define maxmm_Observer_h


namespace maxmm
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
