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

- (void)testExample {
    // This is an example of a functional test case.
    // Use XCTAssert and related functions to verify your tests produce the correct results.
}

- (void)testBasicFunctionality {
    XCTAssert(test.testBasicFunctionality(), "basic functionality broken");
    
    [self measureBlock:^{
        test.testBasicFunctionality();
    }];
}

@end
