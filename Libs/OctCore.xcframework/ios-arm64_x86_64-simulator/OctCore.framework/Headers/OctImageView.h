//
//  OctImageView.h
//  OctCoreSDK
//
//  Created by guantou on 2019/12/9.
//

#import <UIKit/UIKit.h>
#import "OctAdvertisingDelegate.h"

NS_ASSUME_NONNULL_BEGIN

//该协议仅sdk内部使用
@protocol OctImageCatchDelegate <NSObject>

/**
  获取缓存data
 */
- (NSData*)oct_getImageCatchData:(NSString*)fileName;

/**
   保存图片数据
 */
- (void)oct_saveImageData:(NSData*)data fileName:(NSString*)fileName;

/**
   移除缓存数据
 */
- (void)oct_removeCatchData:(NSString*)fileName;

@end

/**
 Image类型广告View
*/
@class OctSpaceInfoModel;

@interface OctImageView : UIImageView

@property (nonatomic, weak) id<OctAdvertisingDelegate> delegate;
//该协议仅sdk内部使用
@property (nonatomic, weak) id<OctImageCatchDelegate> catchDelegate;

/**
 点击位置，相对于广告
*/
@property (nonatomic, assign, readonly) CGPoint adClickPoint;
/**
 点击位置，相对于屏幕
*/
@property (nonatomic, assign, readonly) CGPoint srClickPoint;

- (instancetype)initWithFrame:(CGRect)frame;
- (instancetype)initWithFrame:(CGRect)frame enableClick:(BOOL)isCanClick;

/**
 渲染广告
 @param spaceInfo 广告数据模型
 @param index 加载第几个广告
*/
- (void)loadImageViewWithOctSpaceInfo:(OctSpaceInfoModel *)spaceInfo index:(NSInteger)index;

/**
 移除本身，在NSObject中无法释放
 */
- (void)closeSelf;

//获取图片的url
- (NSString*)getImageUrl:(OctSpaceInfoModel *)spaceInfo index:(NSInteger)index;

//预加载图片
- (void)preloadImageViewWithOctSpaceInfo:(OctSpaceInfoModel *)spaceInfo index:(NSInteger)index isCallback:(BOOL)isCallback;

@end

NS_ASSUME_NONNULL_END
