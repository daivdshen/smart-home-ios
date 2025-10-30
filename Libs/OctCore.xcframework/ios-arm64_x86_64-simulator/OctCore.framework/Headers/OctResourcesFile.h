//
//  OctResourcesFile.h
//  OctCoreSDK
//
//  Created by guantou on 2019/12/10.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

/*
 *  获取静态文件
 */
@interface OctResourcesFile : NSObject

+ (NSString *)sdkjs_js;

+ (NSString *)oct_js;

+ (NSString *)mraid_js;

//  关闭声音图片
+ (UIImage *)voice_off;

//  开启声音图片
+ (UIImage *)voice_on;

//  插屏关闭按钮图片
+ (UIImage *)interstitial_close;

//  左角标
+ (UIImage *)oct_icon_left;
//  右角标
+ (UIImage *)oct_icon_right;
//  左角标竖
+ (UIImage *)oct_icon_left_vertical;
//  左角标横
+ (UIImage *)oct_icon_left_horizontal;

//  摇一摇图标1
+ (UIImage *)oct_icon_shake_1;

//  摇一摇图标2
+ (UIImage *)oct_icon_shake_2;

//  信息流关闭按钮
+ (UIImage *)oct_icon_native_close;

// 手指图标
+ (UIImage *)oct_icon_hand;

// 上划轨道
+ (UIImage *)oct_icon_scrollbar;

@end

NS_ASSUME_NONNULL_END
