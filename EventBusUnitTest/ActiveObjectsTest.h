//
//  ActiveObjectsTest.hpp
//  crookie
//
//  Created by Paolo Bernini on 23/08/16.
//  Copyright © 2016 Paolo Bernini. All rights reserved.
//

#ifndef ActiveObjectsTest_hpp
#define ActiveObjectsTest_hpp


namespace crookie { class EventBus; }


class ActiveObjectsTest
{
public:
  
  ActiveObjectsTest();
  
  bool testBasicFunctionality();
  
  bool testConcurrentDelivery();
  
private:
  
  crookie::EventBus* m_bus;
};






#endif /* ActiveObjectsTest_hpp */
