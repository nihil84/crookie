//
//  EventBusUnitTest.m
//  EventBusUnitTest
//
//  Created by Paolo Bernini on 20/08/16.
//  Copyright © 2016 Paolo Bernini. All rights reserved.
//

#import <XCTest/XCTest.h>

#include "BasicFunctionalTest.h"
#include "ActiveObjectsTest.h"


@interface EventBusUnitTest : XCTestCase
{
    BasicFunctionalTest basicTest;
    ActiveObjectsTest aobjTest;
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
    XCTAssert(basicTest.testBasicFunctionality(), "basic functionality broken");
    
    [self measureBlock:^{
        basicTest.testBasicFunctionality();
    }];
}

- (void)testMultipleDelivery {
    XCTAssert(basicTest.testMultipleDelivery(), "unable to deliver same event "
              "to multiple instances of the same handler");
}

- (void)testTowEventsInARow {
    XCTAssert(basicTest.testTwoEventsInARow(), "unable to deliver two events "
              "to the same handler");
}

- (void)testBusSubscriptions {
    XCTAssert(basicTest.testBusSubscriptions(), "bus switching of the handler does "
              "does not work");
}

- (void)testActiveObjectThreadless {
    XCTAssert(aobjTest.testBasicFunctionality(), "ActiveObjects does not work");
    
}


@end
