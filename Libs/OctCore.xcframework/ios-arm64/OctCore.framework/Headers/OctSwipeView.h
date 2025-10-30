//
//  OctSwipeView.h
//  OctopusAdSDK
//
//  Created by zwf on 2023/5/17.
//

#import <UIKit/UIKit.h>

@class OctSwipeViewModel;

NS_ASSUME_NONNULL_BEGIN

@protocol OctSwipeViewDelegate <NSObject>

@optional

- (void)oct_adSwipeAction;

@end

@interface OctSwipeView : UIImageView

@property (nonatomic, weak) id<OctSwipeViewDelegate> delegate;
@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSString *details;
@property (nonatomic, strong) OctSwipeViewModel *swipeViewModel;

- (instancetype)initWithFrame:(CGRect)frame;
- (instancetype)initWithFrame:(CGRect)frame zoomScale:(CGFloat)zoomScale;

@end

NS_ASSUME_NONNULL_END
