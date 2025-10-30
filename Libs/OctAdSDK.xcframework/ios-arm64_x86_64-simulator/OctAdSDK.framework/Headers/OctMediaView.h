//
//  OctMediaView.h
//  OctAdSDK
//
//  Created by hongji cai on 2024/4/13.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

typedef enum : NSUInteger {
    OctVedioShowModelResize,
    OctVedioShowModelResizeAspect,
    OctVedioShowModelResizeFill,
} OctVedioShowModel;

@class OctSpaceInfoModel;

@interface OctMediaView : UIView
//视频的显示模式，默认OctVedioShowModelResizeAspect
@property (nonatomic, assign) OctVedioShowModel showModel;
//声音控制视图, 外部可以改变frame。 width / height = 1 / 1
@property (nonatomic, strong, readonly) UIView *voiceControlView;

- (instancetype)initWithFrame:(CGRect)frame spaceInfo:(OctSpaceInfoModel*)spaceInfo;
@end

NS_ASSUME_NONNULL_END
