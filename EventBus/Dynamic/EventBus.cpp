#include "EventBus.h"

#include "AEventHandler.h"
#include <stdexcept>

using namespace crookie;


int EventType::m_typecounter = 0;

typedef BucketException< IEventHandler > BusException;


//----------------------------------------------------------------------------//
EventBus::~EventBus()
{
  std::unique_lock<std::recursive_mutex> lock(m_mutex);
  m_handlers.clear();
}

//----------------------------------------------------------------------------//
bool EventBus::unsubscribe(IBusHandler* handler)
{
  if (handler == NULL)
      return false;

  std::unique_lock<std::recursive_mutex> lock(m_mutex);
  Registry::iterator it = m_handlers.find(handler->type());
  if (it == m_handlers.end()) // no registered handlers for given type
      return false;

  it->second->remove(handler);
  return true;
}

//----------------------------------------------------------------------------//
void EventBus::fire(const Event& event)
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

