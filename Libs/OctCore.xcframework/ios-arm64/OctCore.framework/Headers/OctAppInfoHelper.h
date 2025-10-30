//
//  OctAppInfoHelper.h
//  OctCoreSDK
//
//  Created by guantou on 2020/1/8.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/*
 *  APP信息类
 */
@interface OctAppInfoHelper : NSObject

+ (OctAppInfoHelper *)shareInstance;

@property (nonatomic, readonly) NSString *packageName;
@property (nonatomic, readonly) NSString *installTime;
@property (nonatomic, readonly) NSString *updateTime;
@property (nonatomic, readonly) NSString *appVersion;
@property (nonatomic, readonly) NSString *appVersionCode;
@property (nonatomic, readonly) NSString *ua;

@end

NS_ASSUME_NONNULL_END
