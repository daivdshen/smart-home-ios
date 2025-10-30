//
//  UIViewController+BUUtilities.h
//  BUAdSDK
//
//  Created by Siwant on 2019/4/11.
//  Copyright © 2019 bytedance. All rights reserved.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface UIViewController (PAGUtilities)

- (void)pag_safelyPresentViewController:(UIViewController *)viewControllerToPresent animated: (BOOL)flag completion:(void (^ __nullable)(void))completion;

// 获取当前最后一个被present出的VC，如果没有返回self
- (UIViewController *)pag_presentingViewController;
// 获取当前最后一个被present出的VC，如果没有返回window的最上的topVC
+ (UIViewController *)pag_presentingViewController;

// 获取广义mainWindow的rootViewController
+ (nullable UIViewController*)pag_mainWindowRootViewController;

// 获取指定UIResponder的链下游第一个ViewController对象
+ (nullable UIViewController*)pag_nextViewControllerFor:(UIResponder* _Nullable)responder;

/** 查找当前显示的ViewController*/
+ (UIViewController *)pag_topViewController;

+ (UIViewController *)pag_recursiveFindCurrentShowViewControllerFromViewController:(UIViewController *)fromVC;

/** 检查当前Controller是否在主Window上 */
- (BOOL)isOnWindow;
@end

NS_ASSUME_NONNULL_END
