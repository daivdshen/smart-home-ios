//
//  PAGWebViewDefine.h
//  BURexxar
//
//  Created by muhuai on 2017/5/17.
//  Copyright © 2017年 muhuai. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import "PAGRexxarEngine.h"

typedef NS_ENUM(NSInteger, PAGWebViewNavigationType) {
    PAGWebViewNavigationTypeLinkClicked,
    PAGWebViewNavigationTypeFormSubmitted,
    PAGWebViewNavigationTypeBackForward,
    PAGWebViewNavigationTypeReload,
    PAGWebViewNavigationTypeFormResubmitted,
    PAGWebViewNavigationTypeOther
};

@protocol PAGWebView;

@protocol PAGWebViewDelegate <NSObject>

@optional
- (BOOL)webView:(UIView<PAGWebView> *)webView shouldStartLoadWithRequest:(NSURLRequest *)request navigationType:(PAGWebViewNavigationType)navigationType;

- (void)webViewDidStartLoad:(UIView<PAGWebView> *)webView;

- (void)webViewDidFinishLoad:(UIView<PAGWebView> *)webView;

- (void)webView:(UIView<PAGWebView> *)webView didFailLoadWithError:(NSError *)error;

- (void)webViewWebContentProcessDidTerminate:(UIView<PAGWebView> *)webView API_AVAILABLE(macosx(10.11), ios(9.0));

//二方页面有 domReady回调
- (void)webViewDomReady:(UIView<PAGWebView> *)webView;
@end


/**
  PAGWKWebView 都会实现此协议, 用来对平两个容器之间API的差异
 */
@protocol PAGWebView <PAGRexxarEngine>

@property (nonatomic, strong ,readonly) UIScrollView *ttr_scrollView;

#pragma mark - Loading Content

- (void)ttr_loadRequest:(NSURLRequest *)request;

- (void)ttr_loadHTMLString:(NSString *)string baseURL:(NSURL *)baseURL;

- (void)ttr_reload;

#pragma mark - Moving Back and Forward
- (BOOL)ttr_canGoBack;

- (void)ttr_goBack;

#pragma mark - Multi Delegate
/**
BUWKWebView内部实现成多路代理, 按注册的顺序来依次询问.

 @param delegate webview代理
 */
- (void)ttr_addDelegate:(id<PAGWebViewDelegate>)delegate;

/**
 移除指定代理

 @param delegate 需要移除的代理
 */
- (void)ttr_removeDelegate:(id<PAGWebViewDelegate>)delegate;

@end

