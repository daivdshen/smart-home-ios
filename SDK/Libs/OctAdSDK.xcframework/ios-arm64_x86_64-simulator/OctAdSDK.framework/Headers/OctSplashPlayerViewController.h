//
//  OctSplashPlayerViewController.h
//  OctopusAdSDK
//
//  Created by guantou on 2020/1/13.
//

#import <UIKit/UIKit.h>


NS_ASSUME_NONNULL_BEGIN

@class OctSpaceInfoModel;
@class OCTStrategyHelper;
@protocol OctWorkerDelegate;
@class OctAdRecordModel;

@interface OctSplashPlayerViewController : UIViewController

- (instancetype)initWithSpaceInfo:(OctSpaceInfoModel *)spaceInfo BottomView:(UIView *)bottomView SkipView:(UIView *)skipView;

@property (nonatomic, weak) id<OctWorkerDelegate> delegate;

//@property (nonatomic, weak) UIWindow *containerWindow;

@property (nonatomic, copy) NSString *randomUUID;

- (void)oct_addCloseView;

- (void)reportDeepLink:(BOOL)isSuccess;

// 上报竞价成功
- (void)oct_reportWinPrice:(NSString*)secondPrice;

// 上报竞价失败
- (void)oct_reportLossPrice:(NSString*)winPrice loseReason:(NSString*)reason winBidder:(NSString *)bidder;

//- (void)isCloseAdNextButton;

@property (nonatomic, strong) OCTStrategyHelper *strategyHelper;

@property (nonatomic, strong) OctAdRecordModel *adRecordModel;

@end

NS_ASSUME_NONNULL_END
