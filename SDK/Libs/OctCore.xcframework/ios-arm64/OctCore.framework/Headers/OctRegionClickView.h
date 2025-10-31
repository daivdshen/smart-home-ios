//
//  OctRegionClickView.h
//  OctopusAdSDK
//
//  Created by zwf on 2023/5/18.
//

#import <UIKit/UIKit.h>
#import "OctConfigureModel.h"

NS_ASSUME_NONNULL_BEGIN

@protocol OctRegionClickDelegate <NSObject>

@optional

- (void)oct_adRegionClickAction;

@end

@interface OctRegionClickView : UIView

@property (nonatomic, weak) id<OctRegionClickDelegate> delegate;

- (instancetype)initWithFrame:(CGRect)frame;
- (instancetype)initWithFrame:(CGRect)frame zoomScale:(CGFloat)zoomScale;

- (void)setRegionClick:(OctRegionalClick *)regionClick;

@end

NS_ASSUME_NONNULL_END
