//
//  EventBusUnitTest.m
//  EventBusUnitTest
//
//  Created by Paolo Bernini on 20/08/16.
//  Copyright © 2016 Paolo Bernini. All rights reserved.
//

#import <XCTest/XCTest.h>

#include "BasicFunctionalTest.h"


@interface EventBusUnitTest : XCTestCase
{
    BasicFunctionalTest test;
}

@end

@implementation EventBusUnitTest

- (void)setUp {
    [super setUp];
}

- (void)tearDown {
    [super tearDown];
}

- (void)testBasicFunctionality {
    XCTAssert(test.testBasicFunctionality(), "basic functionality broken");
    
    [self measureBlock:^{
        test.testBasicFunctionality();
    }];
}

- (void)testMultipleDelivery {
    XCTAssert(test.testMultipleDelivery(), "unable to deliver same event to "
              "multiple instances of the same handler");
}

- (void)testTowEventsInARow {
    XCTAssert(test.testTwoEventsInARow(), "unable to deliver two events to "
              "the same handler");
}

- (void)testBusSubscriptions {
    XCTAssert(test.testBusSubscriptions(), "bus switching of the handler does"
              "not work");
}


@end
