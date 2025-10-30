//
//  PAG_IESGurdConfigManager.h
//  IESGeckoKit
//
//  Created by chenyuchuan on 2019/6/14.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface PAG_IESGurdConfigManager : NSObject

+ (instancetype)sharedManager;

- (void)registerAccessKey:(NSString *)accessKey;

- (BOOL)isAccessKeyRegistered:(NSString *)accessKey;

//- (NSArray<NSString *> *)allAccessKeys;

@end

NS_ASSUME_NONNULL_END
