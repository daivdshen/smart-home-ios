//
//  CMGParsingUtils.h
//  ComelitCoreKit
//
//  Created by Busi Andrea on 30/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface CMGParsingUtils : NSObject

+ (nonnull NSString *)getVipAddressFromLogaddr:(nonnull NSString *)logaddr NS_SWIFT_NAME(getVipAddress(from:));
+ (nullable NSString *)getVipSubAddressFromLogaddr:(nonnull NSString *)logaddr NS_SWIFT_NAME(getVipSubAddress(from:));

/// Creating NSString from NSData by fixing invalid UTF8 characters
/// This is needed to properly parse languages like Spanish
+ (nullable NSString *)parseUTF8ByHealingStringForData:(NSData *)data;

@end
