//
//  OctAdBanner.h
//  FBSnapshotTestCase
//
//  Created by guantou on 2023/12/9.
//

#import "OctopusBaseAd.h"

NS_ASSUME_NONNULL_BEGIN

@protocol OctAdBannerDelegate;

@interface OctAdBanner : OctopusBaseAd

// controller 展示view的控制器或者弹出落地页的需要的控制器，此参数不能为空
@property (nonatomic, weak) UIViewController *controller;

// 广告加载成功后获得的 View广告（数组内部为加载完成的View形式广告，直接add到目标View）
@property (nonatomic, strong, readonly) NSArray *adViews;

// 设置广告的代理
@property (nonatomic, weak)id<OctAdBannerDelegate> delegate;

// 横幅初始化
- (instancetype)initWithSlotId:(NSString *)slotId;

// 加载广告
- (void)loadAd;

// 获取广告返回价格(价格单位：分)
- (int)getPrice;

// 获取广告时效(单位：毫秒)
- (NSInteger)getValidTime;

// 广告是否加载成功
- (BOOL)isLoaded;


@end

@protocol OctAdBannerDelegate <NSObject>

// 加载成功
- (void)octad_bannerSuccessToLoad:(OctAdBanner *)bannerAd;

// 加载失败
- (void)octad_banner:(OctAdBanner *)bannerAd didFailWithError:(NSError * _Nullable)error;

// 广告曝光
- (void)octad_bannerDidVisible:(OctAdBanner *)bannerAd;

// 广告点击
- (void)octad_bannerAdViewDidClick:(OctAdBanner *)bannerAd;

// 广告关闭
- (void)octad_bannerDidClose:(OctAdBanner *)bannerAd;

// 广告落地页关闭
- (void)octad_bannerWillCloseOtherView:(OctAdBanner *)bannerAd;

@end

NS_ASSUME_NONNULL_END
