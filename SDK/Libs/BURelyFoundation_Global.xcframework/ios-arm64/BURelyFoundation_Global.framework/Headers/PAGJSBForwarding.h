//
//  PAGJSBForwarding.h
//  BURexxar
//
//  Created by muhuai on 2017/4/27.
//  Copyright © 2017年 muhuai. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "PAGJSBCommand.h"
#import "PAGRexxarEngine.h"
#import "PAGJSBDefine.h"

@interface PAGJSBForwarding : NSObject

+ (instancetype)sharedInstance;


/**
 转发到对应的插件

 @param command JSB命令
 @param engine Hybrid容器, 可是webview, RNView, weex. 实现此协议即可
 @param completion 完成回调
 */
- (void)forwardJSBWithCommand:(PAGJSBCommand *)command engine:(id<PAGRexxarEngine>)engine completion:(PAGJSBResponse)completion;

/**
 注册JSBridge别名
 
 @param alias 新名
 @param orig 原名
 */
- (void)registeJSBAlias:(NSString *)alias for:(NSString *)orig;

@end

