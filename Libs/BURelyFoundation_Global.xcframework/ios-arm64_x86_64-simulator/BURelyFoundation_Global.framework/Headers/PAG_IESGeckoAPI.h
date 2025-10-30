//
//  BU_IESGurdAPI.h
//  PAG_IESGurdKit
//
//  Created by willorfang on 2018/5/17.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

extern NSString *PAG_IESGurdAPIVersion;

@interface PAG_IESGeckoAPI : NSObject

+ (NSString *)packagesStatus;

+ (NSString *)packageInfo;

//+ (NSString *)pushTaskStatWithTaskID:(long long)taskID;

@end

@interface PAG_IESGeckoAPI (BusinessModule)

+ (NSString *)packageInfoWithBusinessDomain:(NSString *)businessDomain;

@end

NS_ASSUME_NONNULL_END
