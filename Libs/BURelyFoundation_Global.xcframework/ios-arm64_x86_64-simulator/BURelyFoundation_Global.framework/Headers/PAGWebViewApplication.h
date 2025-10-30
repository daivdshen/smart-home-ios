//
//  BUJSApplication.h
//  BURexxar
//
//  Created by muhuai on 2017/4/26.
//  Copyright © 2017年 muhuai. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

#import "PAGRexxarEngine.h"
#import "PAGWebViewDefine.h"

extern NSString *const kPAGPangleScheme;
extern NSString *const kPAGPangleDomReadyHost;

@interface PAGWebViewApplication : NSObject


+ (BOOL)handleRequest:(NSURLRequest *)request withWebView:(UIView<PAGWebView> *)webView viewController:(UIViewController *)viewController;

+ (void)fireEvent:(NSString *)eventName data:(NSDictionary *)data withWebView:(UIView<PAGWebView> *)webview;

+ (void)fireEvent:(NSString *)eventName data:(NSDictionary *)data withWebView:(UIView<PAGWebView> *)webview completion:(void (^)(id result, NSError *error))completionHandler;
/**
 注册JSBridge别名
 @warning 会优先查找别名
 @param alias 新名
 @param orig 原名
 */
+ (void)registeJSBAlias:(NSString *)alias for:(NSString *)orig;
@end
