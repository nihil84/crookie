#ifndef DYNAMIC_ACTIVEOBJECT_H
#define DYNAMIC_ACTIVEOBJECT_H

#include "Core/IEventHandler.h"

#include <list>
#include <mutex>


namespace crookie {


template <class ...EventClasses>
class ActiveObject : public IEventHandler
{
public:

  typedef typename EventBus::Event Event;

  ActiveObject(EventBus& bus) { }

  virtual void handle(Event event) override
  {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push_back(event);
  }

protected:

  virtual void dispatch();

  
private:

  std::mutex mutex_;
  std::list< Event > queue_;
};

void ActiveObject::dispatch()
{
  std::unique_lock<std::mutex> lock(mutex_);

  while (queue_.size() > 0)
  {
    Event event = queue_.front();
    queue_.pop_front();

    // any exception thrown by the handler will break the message
    // dispatching but you can resume it later.
    event->dispatch(*this);
  }
}


}

#endif // DYNAMIC_ACTIVEOBJECT_H
