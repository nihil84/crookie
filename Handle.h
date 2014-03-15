//
//  Handle.h
//  crookie
//
//  Created by Paolo Bernini on 15/03/14.
//  Copyright (c) 2014 Paolo Bernini. All rights reserved.
//

#ifndef crookie_Handle_h
#define crookie_Handle_h

namespace crookie {
  
  /**
   * @brief Locked reference to complex data structures
   */
  template <class T, class LockPolicy>
  class Handle
  {
  public:
    
    typedef T Data;
    
    typedef LockPolicy Locker;
    
    typedef LockPolicy::Mutex Mutex;
    
    typedef typename LockPolicy::Traits<T>::DataPtr DataPtr;
    
    
    // constructor and destructor
    
    Handle(DataPtr data, Mutex& mutex)
      : data_(data), mutex_(mutex)
    { Locker::lock(mutex); }
    
    ~Handle()
    {
      if (data_ != NULL) Locker::unlock(mutex);
    }
    
    
    // move constructor and move assignement operator
    
    Handle(Handle&& temp)
      : data_(temp.data_), mutex_(temp.mutex_)
    {
      temp.data_ = NULL;
    }
    
    Handle& operator =(Handle&& temp)
    {
      this->data_ = temp.data_;
      this->mutex_ = temp.mutex_;
      temp.data_ = NULL;
    }
    
    // data access operators
    
    const Data* get() const { return data_; }
    Data* get() { return data_; }
    
    const Data* operator ->() const { return data_; }
    Data* operator ->() { return data_; }
    
  private:
    
    DataPtr data_;
    
    Mutex& mutex_;
    
  };

  
} // end of namespace

#endif
