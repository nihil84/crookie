//  StaticBus.h
//
//  Created by Paolo Bernini on 02/01/13.
//  Copyright (c) 2013 IDS. All rights reserved.
//


#ifndef STATIC_EVENTBUS_H
#define STATIC_EVENTBUS_H

#include "../Dynamic/EventBus.hpp"


namespace crookie {

  // forward declarations
  template <unsigned code> class AEventHandler;

  // code

  template < unsigned code >
  class StaticBus : public EventBus
  {
  public /*types*/:

    static const unsigned CODE = code;

    typedef AEventHandler< code > IBusHandler;


  public /*interface*/:

    static StaticBus< code >& instance();

    ~StaticBus() { }
    

  private /*types and data*/:

    static std::mutex creation_;

    static volatile std::unique_ptr< StaticBus< code > > instance_;

    StaticBus() { }
    
    StaticBus(const StaticBus&);
    StaticBus operator =(const StaticBus&);
  };


//- member instances ---------------------------------------------------------//

  template < unsigned int code >
  std::mutex StaticBus< code >::creation_;

  template < unsigned int code >
  volatile std::unique_ptr< StaticBus< code > > StaticBus< code >::instance_;


//- method definitions -------------------------------------------------------//

  template < unsigned int code >
  StaticBus< code >& StaticBus< code >::instance()
  {
    if (!instance_)
    {
      std::unique_lock<std::mutex> lock(creation_);
      if (!instance_)
        instance_.reset(new StaticBus< code >());
    }

    return *instance_;
  }

//- template instance --------------------------------------------------------//

  enum Id { Main = 0 };

  typedef StaticBus< Main > MainBus;

  
}

#endif
