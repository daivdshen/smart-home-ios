//
//  PAGActionWorker.h
//  BUFoundation
//
//  Created by bytedance on 2021/3/12.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class PAGActionWorker;
@class PAGCacheAction;
@class PAGMediaCacheWorker;
@class PAGURLSessionDelegateObject;

@protocol PAGActionWorkerDelegate <NSObject>

- (void)actionWorker:(PAGActionWorker *)actionWorker didReceiveResponse:(NSURLResponse *)response;

- (void)actionWorker:(PAGActionWorker *)actionWorker didReceiveData:(NSData *)data isLocal:(BOOL)isLocal;

- (void)actionWorker:(PAGActionWorker *)actionWorker didReceiveLength:(NSInteger)receiveLength didFinishWithError:(nullable NSError *)error responseDesc:(NSString *)responseDesc;

@end

@class PAGActionWorker;

@interface PAGActionWorker : NSObject

@property (nonatomic, weak) id<PAGActionWorkerDelegate> delegate;

/// 是否都是本地缓存
@property (nonatomic, assign) BOOL localCacheOnly;

- (instancetype)initWithActions:(NSArray<PAGCacheAction *> *)actions url:(NSURL *)url cacheWorker:(PAGMediaCacheWorker *)cacheWorker;

- (void)start;

- (void)cancel;

+ (void)test;
@end


NS_ASSUME_NONNULL_END
