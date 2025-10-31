//
//  IESGurdFileMetaManager+Private.h
//  Pods
//
//  Created by 陈煜钏 on 2020/7/18.
//

#ifndef PAG_IESGurdFileMetaManager_Private_h
#define PAG_IESGurdFileMetaManager_Private_h

#import "PAG_IESGurdFileMetaManager.h"

@interface PAG_IESGurdFileMetaManager ()

/**
 检查是否更新
*/
+ (BOOL)checkIfVersionUpdate;

/**
 同步 meta数据到本地
*/
+ (void)synchronizeMetaData;

/**
 保存未激活的包 meta数据
 */
+ (void)saveInactiveMeta:(PAG_IESGurdInactiveCacheMeta *)meta;

/**
 保存已激活的包 meta数据
 */
+ (void)saveActiveMeta:(PAG_IESGurdActivePackageMeta *)meta;

/**
 删除未激活的包 meta数据
 */
+ (void)deleteInactiveMetaForAccessKey:(NSString *)accessKey channel:(NSString *)channel;

/**
 删除已激活包 meta数据
 */
+ (void)deleteActiveMetaForAccessKey:(NSString *)accessKey channel:(NSString *)channel;

/**
 删除所有meta数据
 */
//+ (void)cleanCacheMetaData;

@end

#endif /* IESGurdFileMetaManager_Private_h */
