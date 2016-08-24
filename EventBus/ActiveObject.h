#ifndef DYNAMIC_ACTIVEOBJECT_H
#define DYNAMIC_ACTIVEOBJECT_H

#include "AEventHandler.h"

#include <list>
#include <mutex>


namespace crookie {


template <class... EventTypes>
class ActiveObject : public AEventHandler<EventTypes>...
{
public:

  ActiveObject() { }
  
  explicit ActiveObject(EventBus& bus)
    : AEventHandler<EventTypes>(bus)...
  { }

  virtual void handle(const Event& event) override
  {
    std::unique_lock<std::mutex> lock(mutex_);
    queue_.push_back(event);
  }

protected:

  virtual void dispatch()
  {
    std::unique_lock<std::mutex> lock(mutex_);
    
    while (queue_.size() > 0)
    {
      Event event = queue_.front();
      queue_.pop_front();
      
      // any exception thrown by the handler will break the message
      // dispatching but you can eventually resume it later.
      event->dispatch(*this);
    }
  }
  
private:

  std::mutex mutex_;
  std::list< Event > queue_;
};


}

#endif // DYNAMIC_ACTIVEOBJECT_H
