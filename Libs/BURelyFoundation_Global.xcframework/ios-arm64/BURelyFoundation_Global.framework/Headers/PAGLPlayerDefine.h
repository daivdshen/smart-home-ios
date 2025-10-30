//
//  PAGPlayerDefine.h
//  BUAdSDK
//
//  Created by carl on 2017/12/24.
//  Copyright © 2017年 bytedance. All rights reserved.
//

#import <Foundation/Foundation.h>

/**
 * playerLayer的填充模式
 */
typedef NS_ENUM(NSInteger, PAGPlayerLayerGravity) {
    PAGPlayerLayerGravityResize,           // 非均匀模式。两个维度完全填充至整个视图区域
    PAGPlayerLayerGravityResizeAspect,     // 等比例填充，直到一个维度到达区域边界
    PAGPlayerLayerGravityResizeAspectFill  // 等比例填充，直到填充满整个视图区域，其中一个维度的部分区域会被裁剪
};

/**
 * 手势交互操作
 */
typedef NS_OPTIONS(NSInteger, PAGPlayerGestureOption) {
    PAGPlayerGestureOptionNone              = 0,
    PAGPlayerGestureOptionVolumeEnabled     = 1 << 0,
    PAGPlayerGestureOptionBrightnessEnabled = 1 << 1,
    PAGPlayerGestureOptionFastSkipEnabled   = 1 << 2,
    PAGPlayerGestureOptionSigleTapEnabled   = 1 << 3,
    PAGPlayerGestureOptionDoubleTapEnabled  = 1 << 4,
};
/**
 * 默认的控制视图或完成视图的元素
 */
typedef NS_OPTIONS(NSInteger, PAGPlayerDefaultControlElement) {
    PAGPlayerControlElementNone     = 0,
    PAGPlayerControlElementTop      = 1 << 0,
    PAGPlayerControlElementBottom   = 1 << 1,
    PAGPlayerControlElementActivity = 1 << 2,
    PAGPlayerControlElementFailHint = 1 << 3,
    PAGPlayerControlElementProgress = 1 << 4,
    PAGPlayerControlElementPlay     = 1 << 5,
    PAGPlayerControlElementReplay   = 1 << 6,
};

typedef NSString * PAGPlayerUIControlImage;
typedef NSString * PAGPlayerUIControlLocalizedString;

#define PAGPlayerUIControlImage_LeftBack       @"bu_leftback"
//#define PAGPlayerUIControlImage_TopShadow      @"bu_topShadow"
#define PAGPlayerUIControlImage_BottomShadow   @"bu_bottomShadow"
#define PAGPlayerUIControlImage_BottomPlay     @"bu_bottomPlay"
#define PAGPlayerUIControlImage_BottomPause    @"bu_bottomPause"
#define PAGPlayerUIControlImage_FullClose      @"bu_fullClose"
//#define PAGPlayerUIControlImage_SliderDot      @"bu_sliderDot"
//#define PAGPlayerUIControlImage_FullScreen     @"bu_fullScreen"
//#define PAGPlayerUIControlImage_ShrinkScreen   @"bu_shrinkScreen"
#define PAGPlayerUIControlImage_Replay         @"bu_replay"
#define PAGPlayerUIControlImage_Play           @"bu_play"
//#define PAGPlayerUIControlImage_Pause          @"bu_pause"
#define PAGPlayerUIControlImage_FastForward    @"bu_fastForward"
#define PAGPlayerUIControlImage_FastBackward   @"bu_fastBackward"

#define PAGPlayerUIControlLocalizedString_Close @"bu_LoStr_Close"

@protocol PAGLPlayerControlViewProtocol <NSObject>

- (instancetype)initWithContrlResourceBundle:(NSBundle *)bundle
                                      images:(NSDictionary <PAGPlayerUIControlImage, NSString *> *)images
                            localizedStrings:(NSDictionary <PAGPlayerUIControlLocalizedString, NSString *>*)localizedStrings;

/**
 * 设置默认控制视图或完成视图的元素组合方式
 * 默认显示全部
 */
- (void)setPlayerDefaultControlElement:(PAGPlayerDefaultControlElement)element;
/**
 * 获取播放完成视图容器
 * 若需自定义可先移除BUPlayerControlElementReplay,再添加自定义视图
 */
- (UIView *)finishedContainer;

@end
