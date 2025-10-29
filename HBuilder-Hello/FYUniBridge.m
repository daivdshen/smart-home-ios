//
//  FYUniBridge.m
//  HBuilder
//
//  Created by flynn.yang on 2024/5/4.
//  Copyright © 2024 DCloud. All rights reserved.
//

#import "FYUniBridge.h"
UniModuleKeepAliveCallback tcallback;

@implementation FYUniBridge

+ (void) notifyEvent: (NSString *) event {
    tcallback(event, NO);
}


// 通过宏 UNI_EXPORT_METHOD_SYNC 将同步方法暴露给 js 端
UNI_EXPORT_METHOD_SYNC(@selector(gotoNativeTestPage:))
UNI_EXPORT_METHOD_SYNC(@selector(sendNotificationToNative:))
UNI_EXPORT_METHOD_SYNC(@selector(printString:))
UNI_EXPORT_METHOD_SYNC(@selector(getCurrentH5Path))

/// 同步方法（注：同步方法会在 js 线程执行）
/// @param options js 端调用方法时传递的参数   支持：String、Number、Boolean、JsonObject 类型
- (NSString *)gotoNativeTestPage:(NSString *)options {
    NSLog(@"---gotoNativeTestPage js端传递的参数==%@ --",options);
    NSData *jsonData = [options dataUsingEncoding:NSUTF8StringEncoding];
    NSError *error = nil;
    NSDictionary *dictionary = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&error];
     
    if (dictionary != nil) {
        NSLog(@"Dictionary: %@", dictionary);
        [[NSNotificationCenter defaultCenter] postNotificationName:@"FYUniBridgeGotoPage" object:nil userInfo:dictionary];
    } else {
        NSLog(@"Error parsing JSON: %@", error);
    }
    
    
    //可以在该方法中实现原生功能，然后直接通过 return 返回参数给 js
    // 同步返回参数给 js 端  支持：NSString、NSDictionary（只能包含基本数据类型）、NSNumber 类型
    return @"success";
}

- (NSString *) printString: (NSString *) content {
    NSLog(@"--- js --- printString:%@", content);
    return @"success";
}

/// 同步方法（注：同步方法会在 js 线程执行）
/// @param options js 端调用方法时传递的参数, 示例
///     {
///         "type": "logout",       // 消息类型
///         "params":{}             // 消息参数
///     }
- (NSString *)sendNotificationToNative:(NSString *)options {
    NSLog(@"--- sendNotificationToNative js端传递的参数==%@ --",options);
    NSData *jsonData = [options dataUsingEncoding:NSUTF8StringEncoding];
    NSError *error = nil;
    NSDictionary *dictionary = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:&error];
     
    if (dictionary != nil) {
        NSLog(@"Dictionary: %@", dictionary);
        [[NSNotificationCenter defaultCenter] postNotificationName:@"FYUniBridgeNotification" object:nil userInfo:dictionary];
    } else {
        NSLog(@"Error parsing JSON: %@", error);
    }
    
    
    //可以在该方法中实现原生功能，然后直接通过 return 返回参数给 js
    // 同步返回参数给 js 端  支持：NSString、NSDictionary（只能包含基本数据类型）、NSNumber 类型
    return @"success";
}

// 通过宏 UNI_EXPORT_METHOD 将异步方法暴露给 js 端
UNI_EXPORT_METHOD(@selector(testAsyncFunc:callback:))

/// 异步方法（注：异步方法会在主线程（UI线程）执行）
/// @param options js 端调用方法时传递的参数
/// @param callback 回调方法，回传参数给 js 端
- (void)testAsyncFunc:(NSDictionary *)options callback:(UniModuleKeepAliveCallback)callback {
    // options 为 js 端调用此方法时传递的参数
    NSLog(@"%@",options);
    
    // 可以在该方法中实现原生能力，然后通过 callback 回调到 js

    // 回调方法，传递参数给 js 端 注：只支持返回 String 或 NSDictionary (map) 类型
//    if (callback) {
//        // 第一个参数为回传给js端的数据，第二个参数为标识，表示该回调方法是否支持多次调用，如果原生端需要多次回调js端则第二个参数传 YES;
//        callback(@"success",NO);
//    }
    tcallback = callback;
}

// 通过宏 UNI_EXPORT_METHOD_SYNC 将同步方法暴露给 js 端
UNI_EXPORT_METHOD_SYNC(@selector(testSyncFunc:))

/// 同步方法（注：同步方法会在 js 线程执行）
/// @param options js 端调用方法时传递的参数
- (NSString *)testSyncFunc:(NSDictionary *)options {
    // options 为 js 端调用此方法时传递的参数
    NSLog(@"%@",options);

    /*
     可以在该方法中实现原生功能，然后直接通过 return 返回参数给 js
     */

    // 同步返回参数给 js 端 注：只支持返回 String 或 NSDictionary (map) 类型
    return @"success";
}
@end
