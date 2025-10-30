//
//  OctVedioCountDownView.h
//  OctCore
//
//  Created by ZY on 2019/12/9.
//  ***需要完善内容---按钮需要根据返回的广告内容去修改其逻辑。目前完成的是可在按钮外部自定义按钮的大小及其位置。

#import <UIKit/UIKit.h>
#import "OctConfigureModel.h"
#import "OctAdvertisingType.h"


NS_ASSUME_NONNULL_BEGIN

/**
 首页倒计时，可点击关闭按钮
*/
@protocol OctVedioCountDownCloseDelegate <NSObject>

@optional
/**
 圆角点击关闭按钮
 */
- (void)oct_countDownCloseView:(BOOL)isClose autoClose:(BOOL)isAutoClose;

/**
 剩余时长
 */
- (void)oct_splashAdLifeTime:(NSUInteger)time;

@end

@interface OctVedioCountDownView : UIView

@property (nonatomic, weak) id<OctVedioCountDownCloseDelegate> delegate;

- (instancetype)initWithFrame:(CGRect)frame;
- (void)setCustomCountDownView:(UIView * _Nullable)view;
- (void)setRenderView:(OctRenderView * _Nullable)view;
- (void)closeSelf;
- (void)forceShowCloseButton;

@property (nonatomic, assign) BOOL autoClose;
@property (nonatomic, assign) BOOL manualClosable;
@property (nonatomic, assign) NSInteger maxTime;
@property (nonatomic, assign) NSInteger minTime;
@property (nonatomic, strong) UIColor *startTitleColor;
//视频相关
@property (nonatomic, assign) BOOL isVedioType;
@property (nonatomic, assign) NSInteger vedioTotalTime;
@property (nonatomic, assign) NSInteger vedioMaxTime;
@property (nonatomic, readonly) NSInteger leftVedioTime;

- (void)updateContent:(NSString*)content;
@end

NS_ASSUME_NONNULL_END
