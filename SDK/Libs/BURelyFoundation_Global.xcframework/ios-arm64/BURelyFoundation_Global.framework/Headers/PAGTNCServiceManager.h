//
// Created by bytedance on 2020/12/15.
//

#import <Foundation/Foundation.h>

@class PAGNetworkRequest,PAGTNCRequestParam;

@interface PAGTNCServiceConfig : NSObject

@property (nonatomic, copy, readonly) NSString *appKey;

@property(nonatomic, copy, readonly) NSString *tncPath;

@property(nonatomic, copy, readonly) NSArray<NSString *> *tncDomains;

@property(nonatomic, copy, readonly) PAGTNCRequestParam *(^tncRequestParam)(void);
@end

__attribute__((objc_subclassing_restricted))
@interface PAGTNCServiceManager : NSObject

+ (void)registerTNCServiceWithAppKey:(NSString *)appKey tncDomains:(NSArray<NSString *> *)tncDomains tncPath:(NSString *)tncPath requestParam:(PAGTNCRequestParam * (^)(void))param;

+ (NSString *)TNCUrlWithBaseUrl:(NSString *)baseUrl forRequest:(PAGNetworkRequest *)request;

+ (void)refreshTNCDomainsWithAppKey:(NSString *)appKey;
@end
