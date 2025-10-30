//
//  OctDevice.h
//  OctAD
//
//  Created by zwf on 2020/12/8.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface OctDevice : NSObject

/**
 状态栏高度
  */
+ (CGFloat)oct_statusBarHeight;

+ (CGFloat)oct_safeHeight;

/**
 导航栏高度
  */
+ (CGFloat)oct_navigationBarHeight;


/**
 导航栏加状态栏高度
  */
+ (CGFloat)oct_topHeight;


/**
 X底部高度
  */
+ (CGFloat)oct_bottomOffset;

/**
 底部高度+tabbar高度
 */
+ (CGFloat)oct_tabbarHeight;

+ (BOOL)oct_isPhoneX;

@end

NS_ASSUME_NONNULL_END
