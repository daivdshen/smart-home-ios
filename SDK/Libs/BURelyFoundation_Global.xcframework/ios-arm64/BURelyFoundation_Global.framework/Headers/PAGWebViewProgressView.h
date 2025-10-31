//
//  PAGWebViewProgressView.h
//  BURexxar
//
//  Created by muhuai on 2017/6/13.
//  Copyright © 2017年 muhuai. All rights reserved.
//

#import "PAGWebViewDefine.h"

#import <UIKit/UIKit.h>
#import <WebKit/WebKit.h>


/**
 假进度条
 使用方法:
 1.addSubView到 WebView中
 2.通过多路代理将此控件设为其中一个子代理
 */
@interface PAGWebViewProgressView : UIView<PAGWebViewDelegate>

@property (nonatomic, strong) UIColor *lineFillColor;

@end
