//
//  OctAdManager.h
//  FBSnapshotTestCase
//
//  Created by guantou on 2023/10/22.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface OctPrivacyModel : NSObject

/**  是否允许个性化广告推荐 默认允许 0 允许，1 不允许  **/
@property (nonatomic, assign) BOOL isLimitPersonalAds;

@end


@interface OctAdManager : NSObject

/**  初始化SDK  **/
+ (void)configureWithApplicationID:(NSString *)appId;

/**  初始化SDK，并配置隐设置  **/
+ (void)configureWithApplicationID:(NSString *)appId andPrivacy:(OctPrivacyModel *)model;

/**  设置idfa  **/
+ (void)setupIDFA:(NSString *)idfa;

/**  设置caid
 caid = "[{\"caid\":\"f1ab6f44a868aa02b11475c2566daadd\",\"version\":\"20220111\"},{\"caid\":\"4f2fe9ea7811f5808fbcfe1f6110be90\",\"version\":\"20230330\"}]"
 **/
+ (void)setupCAID:(NSString *)caid;

/**  SDK版本号  **/
+ (NSString *)sdkVersion;

/**  IDFA  **/
+ (NSString *)getIdfa;


@end

NS_ASSUME_NONNULL_END
