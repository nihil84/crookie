#include "EventBus.h"
#include "../../Utils/ScopeGuard.h"

#include <stdexcept>

using namespace crookie;



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

//------------------------------------------------------------------------------
bool EventBus::unsubscribe(int type, IEventHandler* handler)
{
  if (handler == nullptr)
      return false;

  std::unique_lock<std::recursive_mutex> lock(m_mutex);
  Registry::iterator it = m_handlers.find(type);
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
  ScopeGuard guard = [&] () { --m_firingDepth; };
  
  List::Iterable iterable = subscribers->second->getIterable();
  List::Iterable::const_iterator it = iterable.begin();
  
  std::exception_ptr uncaught_ex;
  
  for (; it != iterable.end(); ++it)
  {
    try
    {
      (*it)->handle(event);
    }
    catch (...)
    {
      uncaught_ex = std::current_exception();
    }
  
    if (uncaught_ex) try
    {
      // if the concrete handler as a custom implementation of the exception
      // handler, the bus will recover and continue to dispatch events...
      (*it)->handle(uncaught_ex);
    }
    catch (...)
    {
      // ...otherwise, or if even this handler raises an exception, the
      // exception is forwarded to the outer level interrupting the dispatching
      std::throw_with_nested(std::runtime_error("EventBus dispatching "
          "interrupted abruptly by unhandled exception"));
    }
  }
  
  // not dismissing guard because depth has to be decremented anyway
}
