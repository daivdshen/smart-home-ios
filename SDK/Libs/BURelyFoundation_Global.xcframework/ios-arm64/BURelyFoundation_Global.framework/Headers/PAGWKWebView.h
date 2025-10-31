//
//  PAGWKWebView.h
//  BURexxar
//
//  Created by muhuai on 2017/5/5.
//  Copyright © 2017年 muhuai. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <WebKit/WebKit.h>
#import "PAGWebViewDefine.h"

@interface PAGWKWebView : WKWebView<PAGWebView>

- (instancetype)initWithFrame:(CGRect)frame
                configuration:(WKWebViewConfiguration *)configuration
        configurationModifier:(void(^)(WKWebViewConfiguration *configuration))modifier;

/// 白屏检测
- (void)bu_detectBlankWebViewCompleteBlock:(void(^)(BOOL, NSError *))block;

/// 白屏检测百分比
- (void)bu_detectBlankPercentCompleteBlock:(void(^)(CGFloat bgColorPercent, NSError *error))block;
/// jsbridge 白屏检测百分比
- (void)bu_detectBlankPercentWithImageStr:(NSString *)imageStr completeBlock:(void(^)(CGFloat bgColorPercent, NSError *))block;

- (void)ttr_fireEvent:(NSString *)event data:(NSDictionary *)data completion:(void (^)(id result, NSError *error))completionHandler;

@property (nonatomic, assign) BOOL reallySuccess;
@end
