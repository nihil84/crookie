#include "EventBus.hpp"

#include "AEventHandler.hpp"
#include <stdexcept>


namespace GEB {

  int EventType::typecounter_ = 0;

  typedef Core::BucketException< Core::IEventHandler > BusException;
  
  //----------------------------------------------------------------------------//
  EventBus::~EventBus()
  {
    std::unique_lock<std::recursive_mutex> lock(mutex_);
    handlers_.clear();
  }

  //----------------------------------------------------------------------------//
  void EventBus::unsubscribe(IBusHandler* handler)
  {
    if (handler == NULL) return;

    std::unique_lock<std::recursive_mutex> lock(mutex_);
    Registry::iterator it = handlers_.find(handler->type());
    if (it == handlers_.end()) // no  registered handlers for given type
        return;

    it->second->remove(handler);
  }

  //----------------------------------------------------------------------------//
  void EventBus::fire(const Event& event)
  {
    std::unique_lock<std::recursive_mutex> lock(mutex_);

    Registry::iterator subscribers = handlers_.find(event->type());
    
    if (subscribers == handlers_.end())
    {
      // no handlers for that type
      return; // that's not an error but it should be logged
    }

    firingDepth_++;

    List::Iterable iterable = subscribers->second->getIterable();

    BusException bucket(firingDepth_);
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
      bucket.add(*it, std::runtime_error(
          "StaticBus caught an exception of an unknown type "
          "(you should derive all your exceptions from std::exception,"
          " throw them by value and catch by reference)"
      ));
    }

    firingDepth_--;

    if (!bucket.empty())
        throw bucket;
  }

} // end of namespace GEB
