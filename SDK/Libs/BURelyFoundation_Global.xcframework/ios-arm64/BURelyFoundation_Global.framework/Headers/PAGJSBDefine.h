//
//  BUJSBDefine.h
//  BURexxar
//
//  Created by muhuai on 2017/5/5.
//  Copyright © 2017年 muhuai. All rights reserved.
//

#import <Foundation/Foundation.h>

#define _PAG_HANDLER_SET(NAME, HANDLERS) \
- (NSSet<NSString *> *)NAME { \
if (![HANDLERS isKindOfClass:[NSArray class]]) {\
return nil;\
}\
return [NSSet setWithArray:HANDLERS];\
}

#define ARRAY(...) [NSArray arrayWithObjects: (id []){ __VA_ARGS__ } count: sizeof((id []){ __VA_ARGS__ }) / sizeof(id)]

#define PAG_PROTECTED_HANDLER(...) \
_PAG_HANDLER_SET(protectedHandlerSet, ARRAY(__VA_ARGS__))

#define PAG_PRIVATE_HANDLER(...) \
_PAG_HANDLER_SET(privateHandlerSet, ARRAY(__VA_ARGS__))

#define PAG_CALLBACK_SUCCESS \
if (callback) {\
callback(BUJSBMsgSuccess, @{@"code": @"1"});\
}\

#define PAG_CALLBACK_FAILED \
if (callback) {\
callback(BUJSBMsgFailed, @{@"code": @"0"});\
}\

#define PAG_CALLBACK_FAILED_MSG(msg) \
if (callback) {\
callback(BUJSBMsgFailed, @{@"code": @"0", @"msg": [NSString stringWithFormat:msg]? :@""});\
}\

#define PAG_CALLBACK_WITH_MSG(status, msg) \
if (callback) {\
callback(status, @{@"code": status == BUJSBMsgSuccess? @"1": @"0", @"msg": [NSString stringWithFormat:msg]? [NSString stringWithFormat:msg] :@""});\
}\


typedef NS_ENUM(NSUInteger, PAGJSBInstanceType) {
    PAGJSBInstanceTypeNormal, //每次调用都是不同实例(默认, 推荐)
    PAGJSBInstanceTypeGlobal, //全局单例, 需要实现 +(instance)sharedPlugin;
    PAGJSBInstanceTypeWebView, //对同一个webview复用一个实例
};

typedef enum : NSUInteger {
    BUJSBMsgSuccess,
    BUJSBMsgFailed,
    BUJSBMsgParamError,
    BUJSBMsgNoHandler,
    BUJSBMsgNoPermission
} BUJSBMsg;

typedef NS_ENUM(NSInteger, PAGJSBResultCode){
    PAGJSBResultCodeError = -2,      // jsb结果错误cod回传
    PAGJSBResultCodeSuccess = 200,   // jsb结果成功cod回传
};

typedef void(^PAGJSBResponse)(BUJSBMsg, NSDictionary *);
