//
//  CMGLogger.h
//  ComelitCoreKit
//
//  Created by Mutti Simone on 03/10/17.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

#import <CocoaLumberjack/CocoaLumberjack.h>

#if DEBUG
static const DDLogLevel ddLogLevel = DDLogLevelVerbose;
#else
static const DDLogLevel ddLogLevel = DDLogLevelWarning;
#endif

@interface CMGLogger : NSObject


@end
