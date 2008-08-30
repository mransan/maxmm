#include <maxmm/Observable.h>
#include <maxmm/Observer.h>
#include <maxmm/Logger.h>

using namespace maxmm;
Observable::Observable()
{
}
Observable::~Observable()
{
}

bool Observable::registerObs(Observer *_obs)
{
    ObserverListRet ret = m_observers.insert(_obs);
    if(!ret.second)
    {
        LOG_ERROR << "impossible to insert the observer" << std::endl;
    }
    return ret.second;
}

bool Observable::unregisterObs(Observer *_obs)
{
    ObserverListItr itr = m_observers.find(_obs);
    if (itr == m_observers.end())
    {
        LOG_WARNING << "trying to remove an unknown obsever" << std::endl;
        return false;
    }
    m_observers.erase(itr);
    return true;
}

void Observable::Notifier::operator()(Observer *_obs)
{
    _obs->notify();
}

void Observable::notifyAll()
{
    std::for_each(m_observers.begin(),m_observers.end(), Notifier());

}
std::size_t Observable::size()
{
    return m_observers.size();
}

