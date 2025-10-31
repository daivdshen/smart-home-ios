//
//  PAG_IESGurdFileMetaManager.h
//  Pods
//
//  Created by 陈煜钏 on 2019/9/29.
//

#import <Foundation/Foundation.h>
#import "PAG_IESGurdFileMetaManager+Private.h"
#import "PAG_IESGurdInactiveCacheMeta.h"
#import "PAG_IESGurdActivePackageMeta.h"

NS_ASSUME_NONNULL_BEGIN

@interface PAG_IESGurdFileMetaManager : NSObject


/**
 返回未激活的包 meta数据
 */
+ (PAG_IESGurdInactiveCacheMeta * _Nullable)inactiveMetaForAccessKey:(NSString *)accessKey channel:(NSString *)channel;

/**
 返回已激活的包 meta数据
 */
+ (PAG_IESGurdActivePackageMeta * _Nullable)activeMetaForAccessKey:(NSString *)accessKey channel:(NSString *)channel;


/**
 返回所有未激活的数据
 accessKey : @[ channel, ... ]
 */
+ (NSDictionary<NSString *, NSArray<NSString *> *> *)inactiveChannelsDictionary;

/**
 返回所有已激活的数据
 accessKey : @[ channel, ... ]
 */
+ (NSDictionary<NSString *, NSArray<NSString *> *> *)activeChannelsDictionary;

@end

NS_ASSUME_NONNULL_END
