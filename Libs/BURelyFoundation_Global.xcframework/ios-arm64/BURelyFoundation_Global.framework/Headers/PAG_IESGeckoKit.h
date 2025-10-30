//
//  IESGeckoKit.h
//  IESGeckoKit
//
//  Created by willorfang on 2017/8/7.
//
//

#import <Foundation/Foundation.h>
#import "PAG_IESGeckoResourceModel.h"
#import "PAG_IESGeckoDefines.h"
#import "PAG_IESGurdProtocolDefines.h"
#import "PAG_IESGurdCacheConfiguration.h"

NS_ASSUME_NONNULL_BEGIN

#define IESGeckoKit PAG_IESGurdKit
#define IESGeckoNetworkDelegate PAG_IESGurdNetworkDelegate
#define IESGeckoKitSDKVersion PAG_IESGurdKitSDKVersion
#define IESGeckoKitDidRegisterAccessKeyNotification PAG_IESGurdKitDidRegisterAccessKeyNotification

FOUNDATION_EXTERN NSString *PAG_IESGurdKitSDKVersion(void);

@interface PAG_IESGurdKit : NSObject

@property (class, assign) BOOL enable; // default YES

@property (class, nonatomic, copy) NSString *deviceID; // DeviceID。统计用。 注意：需要在syncResources之前进行设置。

@property (class, nonatomic, copy) NSString *platformDomain; // Gecko 系统的domain; 注意：需要在syncResources之前进行设置。

@property (class, nonatomic, strong) id<PAG_IESGurdNetworkDelegate> _Nullable networkDelegate; // 设置网络代理

@property (class, nonatomic, strong) id<PAG_IESGurdDownloaderDelegate> _Nullable downloaderDelegate; // 设置下载代理

@property (class, nonatomic, strong) id<PAG_IESGurdAppLogDelegate> _Nullable appLogDelegate;

#pragma mark - Config

/**
* @brief 设置基本参数
* @param appId                  应用id
* @param appVersion             应用版本
* @param cacheRootDirectory     Gecko系统的cache根目录；可为空，默认 Library/Caches/IESWebCache/
*
* 注意：尽可能提前设置
*/
+ (void)setupWithAppId:(NSString * _Nonnull)appId
            appVersion:(NSString * _Nonnull)appVersion
    cacheRootDirectory:(NSString * _Nullable)cacheRootDirectory;

/**
 注册事件代理；内部弱持有
 */
+ (void)registerEventDelegate:(id<PAG_IESGurdEventDelegate>)eventDelegate;

/**
 * @brief 注册要更新的accessKey
 *        其中，一个accessKey代表一个business（业务线）。一个business里容纳多个channel（频道）。
 *        一般情况下，一个App里面配置一个business就可以了。用channel来区分不同的业务。
 *        对于平台型App，里面可能存在多个隔离的业务线。这个业务线之间也需要隔离。这种情况下，每个业务线可以当成一个business。
 *
 * 注意 : 需要在syncResources之前进行设置。
 * 注意 : 会根据 +[PAG_IESGurdKit setCacheConfiguration:forAccessKey:] 限制channels数量
 */
+ (void)registerAccessKey:(NSString *)accessKey;

/**
开启数据上报
*/
//+ (void)enableReportStatus;

#pragma mark - Sync Resource

/**
 * @brief 将所有未激活的缓存进行激活
 */
+ (void)applyInactivePackages:(IESGurdSyncStatusBlock _Nullable)completion;

/**
 * @brief 更新指定的包，准备好缓存环境。
 *
 * @param accessKey     business的部署key
 * @param channels      指定的channel
 * @param completion    completion
 */
+ (void)syncResourcesWithAccessKey:(NSString *)accessKey
                          channels:(NSArray<NSString *> * _Nullable)channels
                        completion:(IESGurdSyncStatusDictionaryBlock _Nullable)completion;

/**
 * @brief 更新指定的包，准备好缓存环境。
 *
 * @param accessKey         business的部署key
 * @param channels          指定的channel
 * @param customParams      自定义参数 @{ customKey : customValue }
 * @param completion        completion
 */
+ (void)syncResourcesWithAccessKey:(NSString *)accessKey
                          channels:(NSArray<NSString *> * _Nullable)channels
                      customParams:(NSDictionary * _Nullable)customParams
                        completion:(IESGurdSyncStatusDictionaryBlock _Nullable)completion;

#pragma mark - Cache

/**
 * @brief 是否有缓存
 *
 * @param path      文件相对路径
 * @param accessKey 包名
 * @param channel   渠道名
 *                  文件绝对路径：Library/Caches/IESWebCache/<accessKey>/<channel>/<path>
 */
+ (BOOL)hasCacheForPath:(NSString *)path accessKey:(NSString *)accessKey channel:(NSString *)channel;

/**
 * @brief 读取缓存数据
 */
+ (NSData *)dataForPath:(NSString *)path accessKey:(NSString *)accessKey channel:(NSString *)channel;

/**
* @brief 读取缓存数据
*/
+ (NSData *)dataForPath:(NSString *)path accessKey:(NSString *)accessKey channel:(NSString *)channel options:(NSDataReadingOptions)options;

/**
* @brief 异步读取缓存数据
*/
+ (void)asyncGetDataForPath:(NSString *)path
                  accessKey:(NSString *)accessKey
                    channel:(NSString *)channel
                 completion:(IESGurdAccessResourceCompletion)completion;

/**
 * @brief 返回文件类型；如果文件未激活，则返回-1
 */
+ (IESGurdChannelFileType)fileTypeForAccessKey:(NSString *)accessKey channel:(NSString *)channel;

/**
* @brief 返回文件版本；如果文件未激活，则返回0
*/
+ (uint64_t)packageVersionForAccessKey:(NSString *)accessKey channel:(NSString *)channel;

/**
 * @brief 获取指定accessKey根目录
 */
+ (NSString *)rootDirForAccessKey:(NSString *)accessKey;

/**
 * @brief 获取指定channel根目录
 */
+ (NSString *)rootDirForAccessKey:(NSString *)accessKey channel:(NSString *)channel;

/**
 返回channel缓存的状态
 */
+ (IESGurdChannelCacheStatus)cacheStatusForAccessKey:(NSString *)accessKey channel:(NSString *)channel;

/**
 清除cache
 */
+ (void)cleanCache;

/**
 根据accessKey和channel清理对应的缓存;
 */
+ (void)cleanCacheForAccessKey:(NSString *)accessKey
                       channel:(NSString *)channel;

/**
 清理临时文件
*/
+ (void)cleanTempFiles;

#pragma mark - Clean

/**
 设置缓存策略
 */
+ (void)setCacheConfiguration:(PAG_IESGurdCacheConfiguration *)configuration
                 forAccessKey:(NSString *)accessKey;

/**
 添加channel白名单，不被清理
 */
+ (void)addChannelsWhitelist:(NSArray<NSString *> *)channels
                forAccessKey:(NSString *)accessKey;

@end

NS_ASSUME_NONNULL_END
