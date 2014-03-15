//
//  LockPolicy.h
//  crookie
//
//  Created by Paolo Bernini on 15/03/14.
//  Copyright (c) 2014 Paolo Bernini. All rights reserved.
//

#ifndef crookie_LockPolicy_h
#define crookie_LockPolicy_h

#include <mutex>


namespace crookie {
  
  struct SimpleLock
  {
    typedef std::mutex Mutex;
    
    template <class T>
    struct Traits { typedef T& DataPtr; }
    
    void lock(const std::mutex& mutex) { mutex.lock(); }
    void unlock(const std::mutex& mutex) { mutex.unlock(); }
  };
  
} // end of namespace

#endif
