#ifndef DYNAMIC_ACTIVEOBJECT_HPP
#define DYNAMIC_ACTIVEOBJECT_HPP

#include "AEventHandler.hpp"

#include <list>
#include <mutex>


namespace GEB {


  template < class Event>
  class ActiveObject : public AEventHandler<Event>
  {
  public:

      typedef AEventHandler< Event > HandlerClass;

      typedef typename EventBus::Event Message;

      ActiveObject(EventBus& bus) : HandlerClass(bus) { }

      virtual void handle(Message message)
      {
        std::unique_lock<std::mutex> lock(mutex_);
        queue_.push_back(message);
      }

  protected:

      void dispatch();

  private:

    std::mutex mutex_;
    std::list< Message > queue_;

  };

  template < class Event >
  void ActiveObject< Event >::dispatch()
  {
    std::unique_lock<std::mutex> lock(mutex_);

    while (queue_.size() > 0)
    {
      Message message = queue_.front();
      queue_.pop_front();

      // any exception thrown by the handler will break the message
      // dispatching but you can resume it later.
      message->dispatch(*this);
    }
  }


} // end of namespace SBus

#endif // DYNAMIC_ACTIVEOBJECT_HPP
