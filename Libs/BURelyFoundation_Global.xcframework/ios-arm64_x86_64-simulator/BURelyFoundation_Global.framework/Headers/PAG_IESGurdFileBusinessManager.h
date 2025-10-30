//
//  PAG_IESGurdFileBusinessManager.h
//  Pods
//
//  Created by 陈煜钏 on 2019/9/29.
//

#import <Foundation/Foundation.h>

#import "PAG_IESGurdFilePaths.h"

NS_ASSUME_NONNULL_BEGIN

typedef void(^PAG_IESGurdFileOperationCompletion)(BOOL succeed, NSDictionary * _Nullable info, NSError * _Nullable error);

@interface PAG_IESGurdFileBusinessManager : NSObject

+ (void)setup;

#pragma mark - Data

/**
 是否有缓存数据
 */
+ (BOOL)hasCacheForAccessKey:(NSString *)accessKey channel:(NSString *)channel path:(NSString *)path;

/**
 根据accessKey和channel返回数据
 */
+ (NSData * _Nullable)dataForAccessKey:(NSString *)accessKey
                               channel:(NSString *)channel
                                  path:(NSString *)path
                               options:(NSDataReadingOptions)options;

#pragma mark - Create Directory

/**
 创建accessKey目录
 */
//+ (NSString * _Nullable)createDirectoryForAccessKey:(NSString *)accessKey;

/**
 创建channel目录
 */
+ (NSString * _Nullable)createDirectoryForAccessKey:(NSString *)accessKey channel:(NSString *)channel;

/**
 根据accessKey、channel、version、md5创建zip包存放路径
 */
+ (NSString * _Nullable)createInactiveZipPathForAccessKey:(NSString *)accessKey
                                                  channel:(NSString *)channel
                                                  version:(long long)version
                                                      md5:(NSString *)md5
                                                    error:(NSError **)error;

/**
 创建备份目录
*/
+ (void)createBackupDirectoryIfNeeded;

#pragma mark - Paths

/**
 根据accessKey、channel返回备份包路径
 */
+ (NSString * _Nullable)backupPathForAccessKey:(NSString *)accessKey channel:(NSString *)channel;

#pragma mark - Business

/**
 异步执行任务
 */
+ (void)asyncExecuteBlock:(dispatch_block_t)block
                accessKey:(NSString *)accessKey
                  channel:(NSString *)channel;

/**
 同步执行任务
 */
//+ (void)syncExecuteBlock:(dispatch_block_t)block
//               accessKey:(NSString *)accessKey
//                 channel:(NSString *)channel;

#pragma mark - Clean

/**
 清理缓存
 */
+ (void)cleanCache;

/**
 根据accessKey和channel清理对应的缓存
 */
+ (void)cleanCacheForAccessKey:(NSString *)accessKey
                       channel:(NSString *)channel
                    completion:(PAG_IESGurdFileOperationCompletion)completion;

/**
 根据accessKey和channel清理zip包
 */
+ (void)cleanInactiveCacheForAccessKey:(NSString *)accessKey channel:(NSString *)channel;

@end

NS_ASSUME_NONNULL_END
