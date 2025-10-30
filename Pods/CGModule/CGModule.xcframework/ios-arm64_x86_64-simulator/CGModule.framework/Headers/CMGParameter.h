//
//  CMGParameter.h
//  CGModule
//
//  Created by Cornelli Fabio on 09/07/18.
//  Copyright © 2018 Cornelli Fabio. All rights reserved.
//
#import <Foundation/Foundation.h>
#import "CGCallback.h"


@interface CMGParameterBuilder : NSObject

@property(nonatomic) NSString* hostname;
@property(nonatomic) int port;
@property(nonatomic) NSString* activationCode;
@property(nonatomic) id<CGCallback> callback;
@property(nonatomic) NSString* pushToken;

@property(nonatomic) BOOL withUI;
@property(nonatomic) BOOL debug;
@property(nonatomic) BOOL softwareDecode;

@property(nonatomic) int alertTime;
@property(nonatomic) int connectionTimeExtUnit;
@property(nonatomic) int connectionTimeIntUnit;
@property(nonatomic) int closeTime;

@end

static const int DEFAULT_ALERT_TIME = 60;
static const int DEFAULT_CONNECTION_TIME_EXTERNAL_UNIT = 90;
static const int DEFAULT_CONNECTION_TIME_INTERNAL_UNIT = 60;
static const int DEFAULT_CLOSE_TIME = 5;

@interface CMGParameter : NSObject

/**
 * connection params
 */
@property(nonatomic) NSString* hostname;
@property(nonatomic) int port;
@property(nonatomic) NSString* activationCode;
@property(nonatomic) id<CGCallback> callback;
@property(nonatomic) NSString* pushToken;

/**
 * mode
 */
@property(nonatomic) BOOL withUI;
@property(nonatomic) BOOL debug;
@property(nonatomic) BOOL softwareDecode;

/**
 * call timer
 */
@property(nonatomic) int alertTime;
@property(nonatomic) int connectionTimeExtUnit;
@property(nonatomic) int connectionTimeIntUnit;
@property(nonatomic) int closeTime;

- (CMGParameter*)init;
+ (CMGParameter*)makeWithBuilder:(void (^)(CMGParameterBuilder *))buildBlock;

@end
