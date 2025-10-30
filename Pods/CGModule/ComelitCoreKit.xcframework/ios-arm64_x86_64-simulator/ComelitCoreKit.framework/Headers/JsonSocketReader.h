//
//  JsonSocketReader.h
//  Module
//
//  Created by Mutti Simone on 18/09/17.
//  Copyright © 2017 ComelitGroup. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ComelitCoreKit/CMGLogger.h>
#import "JsonSocketHandler.h"

@class ComelitEventBus;

@interface JsonSocketReader : NSObject<JsonSocketHandlerReceiveDataDelegate>

@property (nonatomic, readonly, strong) ComelitEventBus *comelitEventBus;

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithMessageParser:(id<UnitMessageParserProtocol>)messageParser comelitEventBus:(ComelitEventBus *)comelitEventBus;

@end
