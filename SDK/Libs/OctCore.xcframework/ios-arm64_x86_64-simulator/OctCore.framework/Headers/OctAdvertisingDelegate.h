//
//  OctAdvertisingDelegate.h
//  Pods
//
//  Created by guantou on 2019/12/11.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol OctAdvertisingDelegate <NSObject>

@optional

/**
 图片加载完成
 */
- (void)oct_advertisingDidDownload;

/**
 图片加载失败
 */
- (void)oct_adResourceDownloadFail;

/**
 广告加载完成
 */
- (void)oct_advertisingDidFinish;

/**
 广告关闭
 */
- (void)oct_advertisingDidClose;

/**
 广告加载失败
 */
- (void)oct_advertisingDidFail;

/**
 广告点击
 */
- (void)oct_advertisingDidClickType:(NSString * _Nullable)type param:(NSString * _Nullable)param;

/**
 player广告缓冲完成，可以开始播放广告
 */
- (void)oct_advertisingDidReadyAndStart;

/**
 player广告播放完毕
 */
- (void)oct_advertisingDidFinishPlay;

/**
 player广告播放失败
 */
- (void)oct_advertisingDidFailPlay;

/**
 视频广告总时长
 */
- (void)oct_advertisingPlayerTotalLenght:(CGFloat)time;

@end

NS_ASSUME_NONNULL_END

