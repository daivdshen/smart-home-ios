//
//  OctAdIntersitital.h
//  FBSnapshotTestCase
//
//  Created by guantou on 2023/11/20.
//

#import "OctopusBaseAd.h"

NS_ASSUME_NONNULL_BEGIN

@protocol OctAdIntersititalDelegate;

@interface OctAdIntersitital : OctopusBaseAd

// 设置广告的代理
@property (nonatomic, weak)id<OctAdIntersititalDelegate> delegate;

//从落地页回到广告页时， 如果此时倒计时还未结束，是否关闭广告
@property (nonatomic, assign) BOOL isCloseAfterBackToAdvert;

// 插屏初始化
- (instancetype)initWithSlotId:(NSString *)slotId;

// 加载广告
- (void)loadAd;

// 获取广告返回价格(价格单位：分)
- (int)getPrice;

// 获取广告时效(单位：毫秒)
- (NSInteger)getValidTime;

// 广告是否加载成功
- (BOOL)isLoaded;

// 展示广告
- (void)showAdsInViewController:(UIViewController *)viewController;

@end

@protocol OctAdIntersititalDelegate <NSObject>

// 加载成功
- (void)octad_intersititalSuccessToLoad:(OctAdIntersitital *)intersititalAd;

// 加载失败
- (void)octad_intersitital:(OctAdIntersitital *)intersititalAd didFailWithError:(NSError * _Nullable)error;

// 广告曝光
- (void)octad_intersititalDidVisible:(OctAdIntersitital *)intersititalAd;

// 广告点击
- (void)octad_intersititalAdViewDidClick:(OctAdIntersitital *)intersititalAd;

// 广告关闭
- (void)octad_intersititalDidClose:(OctAdIntersitital *)intersititalAd;

// 广告落地页关闭
- (void)octad_intersititalWillCloseOtherView:(OctAdIntersitital *)intersititalAd;

// 图片/视频资源预加载 YES: 预加载成功；NO：预加载失败
- (void)octad_intersititalResourcePreLoadState:(OctAdIntersitital *)intersititalAd preLoadState:(BOOL)isSuccess;

@end

NS_ASSUME_NONNULL_END
