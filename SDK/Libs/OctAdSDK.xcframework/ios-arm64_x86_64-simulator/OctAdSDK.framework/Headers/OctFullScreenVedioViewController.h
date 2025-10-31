//
//  OctFullScreenVedioViewController.h
//  OctAdSDK
//
//  Created by guantou on 2023/11/20.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@class OctDisposeResponseModel;
@class OctAdRecordModel;
@class OctBusinessReport;
@class OCTStrategyHelper;
@class OctBuyerModel;

@protocol OctFullScreenVedioVCDelegate <NSObject>

// 插屏展示页被曝光
- (void)oct_fullScreenVedioVCDidExpose;

// 插屏展示页被点击
- (void)oct_fullScreenVedioVCDidClick;

// 插屏展示页被关闭
- (void)oct_fullScreenVedioVCDidClose;

// 插屏落地页被关闭
- (void)oct_fullScreenVedioVCDidCloseOtherViewController;

/**
资源加载状态
 */
- (void)oct_fullScreenVedioDownloadState:(BOOL)isSuccess;

@end

@interface OctFullScreenVedioViewController : UIViewController

@property (nonatomic, weak)id<OctFullScreenVedioVCDelegate> delegate;

@property (nonatomic, strong) OctDisposeResponseModel *disposeModel;

@property (nonatomic, strong) OctAdRecordModel *adRecordModel;

@property (nonatomic, strong) OctBusinessReport *businessReport;

@property (nonatomic, strong) OCTStrategyHelper *strategyHelper;

@property (nonatomic, strong) OctBuyerModel *buyerModel;
//是否是视频广告
@property (nonatomic, assign) BOOL isVideoAd;

- (void)setupAutoClick;

@end

NS_ASSUME_NONNULL_END
