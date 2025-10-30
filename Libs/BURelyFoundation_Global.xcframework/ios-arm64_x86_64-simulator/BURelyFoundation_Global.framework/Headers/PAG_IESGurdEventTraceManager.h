//
//  PAG_IESGurdEventTraceManager.h
//  IESGeckoKit
//
//  Created by chenyuchuan on 2019/7/4.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface PAG_IESGurdEventTraceManager : NSObject

@end

#pragma mark - Message

@interface PAG_IESGurdTraceMessageInfo : NSObject

@property (nonatomic, copy) NSString *accessKey;

@property (nonatomic, copy) NSString *channel;

@property (nonatomic, copy) NSString *message;

@property (nonatomic, assign) BOOL hasError;

+ (instancetype)messageInfoWithAccessKey:(NSString *)accessKey
                                 channel:(NSString *)channel
                                 message:(NSString *)message
                                hasError:(BOOL)hasError;

@end

@interface PAG_IESGurdEventTraceManager (Message)

//+ (void)traceEventWithGlobalMessage:(NSString *)message;

//+ (NSArray<NSString *> *)allGlobalMessagesArray;

//+ (void)cleanAllGlobalMessages;

+ (void)traceEventWithMessageInfo:(PAG_IESGurdTraceMessageInfo *)messageInfo;
// accessKey : @{ channel : @[ message, ... ] }
//+ (NSDictionary<NSString *, NSDictionary<NSString *, NSArray<NSString *> *> *> *)allMessagesDictionary;
// accessKey : @{ channel : @[ message, ... ] }
//+ (NSDictionary<NSString *, NSDictionary<NSString *, NSArray<NSString *> *> *> *)errorMessagesDictionary;

//+ (void)cleanMessagesForAccessKey:(NSString *)accessKey
//                          channel:(NSString *)channel;

@end

#pragma mark - Network

@interface PAG_IESGurdTraceNetworkInfo : NSObject

@property (nonatomic, copy) NSString *method;

@property (nonatomic, copy) NSString *URLString;

@property (nonatomic, copy) NSDictionary *params;

@property (nonatomic, strong) id responseObject;

@property (nonatomic, strong) NSError *error;

@property (nonatomic, strong) NSDate *startDate;

@property (nonatomic, strong) NSDate *endDate;

+ (instancetype)infoWithMethod:(NSString *)method
                     URLString:(NSString *)URLString
                        params:(NSDictionary *)params;

@end

@interface PAG_IESGurdEventTraceManager (Network)

+ (void)traceNetworkWithInfo:(PAG_IESGurdTraceNetworkInfo *)networkInfo;

//+ (NSArray<PAG_IESGurdTraceNetworkInfo *> *)allNetworkInfos;

@end

NS_ASSUME_NONNULL_END
