//
//  PAG_IESGurdFilePaths.h
//  Pods
//
//  Created by 陈煜钏 on 2019/9/29.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NSString *_Nonnull(^PAG_IESGurdAppendPathBlock)(NSString *name);
@interface NSString (PAGIESGurdPath)
//路径拼接
- (PAG_IESGurdAppendPathBlock)pag_ies_appendPath;
@end

@interface PAG_IESGurdFilePaths : NSObject

@property (class, nonatomic, copy) NSString *cacheRootDirectoryPath;    //缓存根目录

@property (class, nonatomic, readonly, copy) NSString *inactiveDirectoryPath;   //未激活根目录

@property (class, nonatomic, readonly, copy) NSString *backupDirectoryPath;     //未激活根目录

@property (class, nonatomic, readonly, copy) NSString *inactiveMetaDataPath;    //未激活元数据路径

@property (class, nonatomic, readonly, copy) NSString *activeMetaDataPath;      //已激活元数据路径

+ (NSString *)directoryPathForAccessKey:(NSString *)accessKey;

+ (NSString *)directoryPathForAccessKey:(NSString *)accessKey channel:(NSString *)channel;

+ (NSString *)directoryPathForAccessKey:(NSString *)accessKey channel:(NSString *)channel path:(NSString *)path;

+ (NSString *)inactivePathForAccessKey:(NSString *)accessKey channel:(NSString *)channel;

/**
 返回未解压的zip包路径
*/
+ (NSString *)inactiveZipPathForAccessKey:(NSString *)accessKey
                                  channel:(NSString *)channel
                                  version:(long long)version
                                      md5:(NSString *)md5;

/**
 返回备份包路径
*/
+ (NSString *)backupPathForMd5:(NSString *)md5;

@end

@interface PAG_IESGurdFilePaths (Helper)

/**
 返回文件大小
 */
+ (uint64_t)fileSizeAtPath:(NSString *)filePath;

/**
 返回文件大小字符串
 */
+ (NSString *)fileSizeStringAtPath:(NSString *)filePath;

/**
 返回文件路径在Gurd里的相对路径
 如：传入 Library/Caches/IESWebCache/accessKey/channel/path 会返回 accessKey/channel/path
 */
+ (NSString *)briefFilePathWithFullPath:(NSString *)filePath;

@end

NS_ASSUME_NONNULL_END
