//
//  OctWorkerDelegate.h
//  Pods
//
//  Created by guantou on 2020/2/20.
//

#import <Foundation/Foundation.h>
#import "OctAdRequestError.h"

NS_ASSUME_NONNULL_BEGIN

@protocol OctWorkerDelegate <NSObject>

@optional

/**
 图片加载完成
 */
- (void)oct_workerDidDownload;

/**
 图片加载失败
 */
- (void)oct_workerResourceDownloadFail;

/**
 广告加载完成
 */
- (void)oct_workerDidFinish;

/**
 广告关闭
 */
- (void)oct_workerDidClose;

/**
 广告加载失败
 */
- (void)oct_workerDidFailWithError:(OctAdRequestError *)error;

/**
 广告点击
 */
- (void)oct_workerDidClickType:(NSString * _Nullable)type param:(NSString * _Nullable)param;

/**
 player广告缓冲完成，可以开始播放广告
 */
- (void)oct_workerDidReadyAndStart;

/**
 player广告播放完毕
 */
- (void)oct_workerDidFinishPlay;

/**
 player广告播放失败
 */
- (void)oct_workerDidFailPlay;

/**
 视频广告总时长
 */
- (void)oct_workerPlayerTotalLenght:(CGFloat)time;

/**
 开屏剩余时长
 */
- (void)oct_workerAdLifeTime:(NSUInteger)time;

/**
 h5页面完成按钮回调
 */
- (void)oct_normalDoneClick;

@end

NS_ASSUME_NONNULL_END
