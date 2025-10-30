//
//  JsonSocketHandler.h
//  Module
//
//  Created by Mutti Simone on 18/09/17.
//  Copyright © 2017 ComelitGroup. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CocoaAsyncSocket/GCDAsyncSocket.h>
#import <ComelitCoreKit/CMGLogger.h>
#import "UnitMessageParserProtocol.h"

NS_ASSUME_NONNULL_BEGIN

@class JsonSocketHandler;

@protocol JsonSocketHandlerStatusDelegate <NSObject>
@optional
- (void)jsonSocketHandler:(JsonSocketHandler *)handler didStartOnPort:(NSInteger)port;
- (void)jsonSocketHandlerDidStop:(JsonSocketHandler *)handler;
- (void)jsonSocketHandlerDidOpenConnection:(JsonSocketHandler *)handler;
- (void)jsonSocketHandlerDidCloseConnection:(JsonSocketHandler *)handler;
- (void)jsonSocketHandler:(JsonSocketHandler *)handler didFailOnCreationWithError:(NSError *)error;
@end


@protocol JsonSocketHandlerReceiveDataDelegate <NSObject>
@optional
- (void)jsonSocketHandler:(JsonSocketHandler *)handler didReceiveData:(NSData *)data;
@end


@interface JsonSocketHandler : NSObject

@property (nonatomic, weak, nullable) id<JsonSocketHandlerStatusDelegate> socketStatusDelegate;
@property (nonatomic, weak, nullable) id<JsonSocketHandlerReceiveDataDelegate> dataReceiverDelegate;
@property (nonatomic, readonly) BOOL isRunning;
@property (nonatomic, readonly) NSInteger port;

- (instancetype)init;

- (void)startServer;
- (void)stopServer;

@end

NS_ASSUME_NONNULL_END
