#include "EventBus.h"

#include "AEventHandler.h"
#include <stdexcept>

using namespace crookie;

typedef BucketException< IEventHandler > BusException;



//------------------------------------------------------------------------------
EventBus::~EventBus()
{
  std::unique_lock<std::recursive_mutex> lock(m_mutex);

  if (m_firingDepth > 0)
  {
    throw std::logic_error("ill-formed program: EventBus has been destroyed "
                           "while the dispatching events");
  }
  
  for (const auto & pair : m_handlers)
  {
    List::Iterable iterable = pair.second->getIterable();
    List::Iterable::const_iterator it = iterable.begin();
    for (; it != iterable.end(); ++it)
    {
      (*it)->dismiss();
    }
  }
  
  m_handlers.clear();
}

//------------------------------------------------------------------------------
bool EventBus::unsubscribe(IEventHandler* handler)
{
  if (handler == nullptr)
      return false;

  std::unique_lock<std::recursive_mutex> lock(m_mutex);
  Registry::iterator it = m_handlers.find(handler->type());
  if (it == m_handlers.end()) // no registered handlers for given type
      return false;

  it->second->remove(handler);
  return true;
}

//------------------------------------------------------------------------------
void EventBus::dispatch(const Event& event)
{
  std::unique_lock<std::recursive_mutex> lock(m_mutex);

  Registry::iterator subscribers = m_handlers.find(event->type());
  
  if (subscribers == m_handlers.end())
  {
    // no handlers for that type
    return; // that's not an error but it should be logged
  }

  m_firingDepth++;

  List::Iterable iterable = subscribers->second->getIterable();

  BusException bucket(m_firingDepth);
  List::Iterable::const_iterator it = iterable.begin();
  for (; it != iterable.end(); ++it) try
  {
    (*it)->handle(event);
  }
  catch (std::exception& ex)
  {
    bucket.add(*it, ex);
  }
  catch (...)
  {
    bucket.add(*it, std::logic_error(
        "StaticBus caught an exception of an unknown type "
        "(you should derive all your exceptions from std::exception,"
        " throw them by value and catch by reference)"
    ));
  }

  m_firingDepth--;

  if (!bucket.empty())
      throw bucket;
}

//- protected ------------------------------------------------------------------
void EventBus::subscribe(int type, IEventHandler* handler)
{
  std::unique_lock<std::recursive_mutex> lock(m_mutex);
  
  if (m_firingDepth > 0)
  {
    throw std::logic_error("new subscriptions during event dispatching are "
                           "not supported (yet)");
  }

  Registry::iterator it = m_handlers.find(type);
  
  std::shared_ptr<List> subscribers;
  if (it == m_handlers.end()) // null pointer on insertion
  {
    subscribers.reset(new List());
    m_handlers.insert(std::make_pair(type, subscribers));
  }
  else
    subscribers = it->second;
  
  subscribers->add(handler);
}

