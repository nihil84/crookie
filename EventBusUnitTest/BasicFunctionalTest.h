//
//  BasicFunctionalTest.h
//  crookie
//
//  Created by Paolo Bernini on 20/08/16.
//  Copyright © 2016 Paolo Bernini. All rights reserved.
//

#ifndef BasicFunctionalTest_h
#define BasicFunctionalTest_h

namespace crookie { class EventBus; }


class BasicFunctionalTest
{
public:
    
    BasicFunctionalTest();
    
    bool testBasicFunctionality();
    
private:
    
    crookie::EventBus* m_bus;
};




#endif /* BasicFunctionalTest_h */
