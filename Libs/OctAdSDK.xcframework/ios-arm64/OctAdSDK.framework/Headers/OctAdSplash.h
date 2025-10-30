//
//  OctAdSplash.h
//  OctopusAdSDK
//
//  Created by guantou on 2023/10/24.
//

#import "OctopusBaseAd.h"
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@protocol OctAdSplashDelegate;

@interface OctAdSplash : OctopusBaseAd

// 设置开屏广告的代理
@property (nonatomic, weak)id<OctAdSplashDelegate> delegate;
//底部视图开关
@property (nonatomic, assign) BOOL isShowBottomView;
//从落地页回到广告页时， 如果此时倒计时还未结束，是否关闭广告
@property (nonatomic, assign) BOOL isCloseAfterBackToAdvert;
//标识是否是UniApp的适配器
@property (nonatomic, assign) BOOL isUniAppAdapter;

// 开屏初始化
- (instancetype)initWithSlotId:(NSString *)slotId;
// 开屏初始化, 可以指定超时时间，单位：s
- (instancetype)initWithSlotId:(NSString *)slotId timeOut:(double)timeoutInterval;

// 加载开屏广告
- (void)loadAd;

// 展示广告。该方法已经弃用，请使用showAdsInViewController进行广告曝光展示
- (void)showAdsInWindow:(UIWindow *)window;

// 展示广告。开屏广告基于viewController弹出
- (void)showAdsInViewController:(UIViewController *)viewController;

// uniapp适配器，展示开屏广告
- (void)showUniAdsInViewController:(UIViewController *)viewController bottomView:(UIView*)bottomView;

// 获取广告返回价格(价格单位：分)
- (int)getPrice;

// 获取广告时效(单位：毫秒)
- (NSInteger)getValidTime;

@end

/**
 代理方法
 */
@protocol OctAdSplashDelegate <NSObject>

@optional

/**
 @return 展示下部logo位置，需要给传入view设置尺寸。
 */
- (UIView *)octad_splashBottomView;

/**
 广告加载成功
 */
- (void)octad_splashAdSuccess:(OctAdSplash *)splashAd;

/**
 广告展示
 */
- (void)octad_splashAdDidVisible:(OctAdSplash *)splashAd;

/**
 广告加载失败
 */
- (void)octad_splashAd:(OctAdSplash *)splashAd didFailWithError:(NSError * _Nullable)error;

/**
 SDK渲染开屏广告点击回调
 */
- (void)octad_splashAdDidClick:(OctAdSplash *)splashAd;

/**
 SDK渲染开屏广告关闭回调，当用户点击广告时会直接触发此回调，建议在此回调方法中直接进行广告对象的移除操作
 */
- (void)octad_splashAdDidClose:(OctAdSplash *)splashAd;

/**
 倒计时为0时会触发此回调
 */
- (void)octad_splashAdCountdownToZero:(OctAdSplash *)splashAd;

/**
 广告落地页关闭
 */
- (void)octad_splashWillCloseOtherView:(OctAdSplash *)splashAd;

/**
 图片/视频资源预加载 YES: 预加载成功；NO：预加载失败
 */
- (void)octad_splashResourcePreLoadState:(OctAdSplash *)splashAd preLoadState:(BOOL)isSuccess;

@end


NS_ASSUME_NONNULL_END
