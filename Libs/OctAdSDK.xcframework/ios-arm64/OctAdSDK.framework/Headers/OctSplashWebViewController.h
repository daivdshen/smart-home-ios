//
//  OctSplashWebViewController.h
//  OctopusAdSDK
//
//  Created by guantou on 2020/1/13.
//

#import <UIKit/UIKit.h>

@protocol OctWorkerDelegate;

NS_ASSUME_NONNULL_BEGIN

@class OctSpaceInfoModel;
@class OCTStrategyHelper;
@class OctAdRecordModel;

@interface OctSplashWebViewController : UIViewController

- (instancetype)initWithSpaceInfo:(OctSpaceInfoModel *)spaceInfo BottomView:(UIView *)bottomView SkipView:(UIView *)skipView;

@property (nonatomic, weak) id<OctWorkerDelegate> delegate;

//@property (nonatomic, weak) UIWindow *containerWindow;

@property (nonatomic, copy) NSString *randomUUID;

@property (nonatomic, strong) OCTStrategyHelper *strategyHelper;

@property (nonatomic, strong) OctAdRecordModel *adRecordModel;

- (void)oct_addCloseView;

//- (void)isCloseAdNextButton;

- (void)reportDeepLink:(BOOL)isSuccess;

// 上报竞价成功
- (void)oct_reportWinPrice:(NSString*)secondPrice;

// 上报竞价失败
- (void)oct_reportLossPrice:(NSString*)winPrice loseReason:(NSString*)reason winBidder:(NSString *)bidder;

@end

NS_ASSUME_NONNULL_END
