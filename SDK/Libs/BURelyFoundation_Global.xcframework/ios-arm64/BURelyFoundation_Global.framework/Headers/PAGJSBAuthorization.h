//
//  PAGJSBAuthorization.h
//  BURexxar
//
//  Created by muhuai on 2017/4/27.
//  Copyright © 2017年 muhuai. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "PAGJSBDefine.h"
#import "PAGJSBCommand.h"
#import "PAGRexxarEngine.h"

@protocol PAGJSBAuthorization <NSObject>


/**
 验证是否有权限执行这个JSB

 @param engine 上下文engine
 @param command JSBCommand
 @param domain 所在页面
 @return 是否有权限
 */
- (BOOL)engine:(id<PAGRexxarEngine>)engine isAuthorizedJSB:(PAGJSBCommand *)command domain:(NSString *)domain;


/**
 fireEvent发送之前 验证是否有权限发送这个事件

 @param engine 上下文engine
 @param eventName 事件名字
 @param domain 所在页面
 @return 是否有权限
 */
- (BOOL)engine:(id<PAGRexxarEngine>)engine isAuthorizedEvent:(NSString *)eventName domain:(NSString *)domain;

@end
