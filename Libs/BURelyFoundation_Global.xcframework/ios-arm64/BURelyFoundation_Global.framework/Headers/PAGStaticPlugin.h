//
//  PAGStaticPlugin.h
//  BURexxar
//
//  Created by muhuai on 2017/4/27.
//  Copyright © 2017年 muhuai. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "PAGJSBDefine.h"

@protocol PAGRexxarEngine;
@class PAGJSBCommand;
typedef void(^PAGJSBStaticHandler)(NSDictionary *params, PAGJSBResponse completion);

@interface PAGStaticPlugin : NSObject

- (void)registerHandlerBlock:(PAGJSBStaticHandler)handler forMethodName:(NSString*)method;

- (BOOL)callHandlerWithCommand:(PAGJSBCommand *)command engine:(id<PAGRexxarEngine>)engine completion:(PAGJSBResponse)completion;
@end
