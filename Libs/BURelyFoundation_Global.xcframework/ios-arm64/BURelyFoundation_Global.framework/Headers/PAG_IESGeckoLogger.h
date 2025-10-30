//
//  PAG_IESGurdLogger.h
//  PAG_IESGurdKit
//
//  Created by li keliang on 2019/3/13.
//

#import <Foundation/Foundation.h>
#import "PAG_IESGeckoDefines.h"
#import "PAG_IESGurdProtocolDefines.h"

NS_ASSUME_NONNULL_BEGIN

@interface PAG_IESGurdLogger : NSObject

@property (class, nonatomic, strong) id<PAG_IESGurdAppLogDelegate> appLogDelegate;

@property (class, nonatomic, assign) BOOL shouldReportParams;

+ (void)recordStatsWithType:(IESGurdStatsType)type
                     status:(IESGurdSyncStatus)status
                  accessKey:(NSString *)accessKey
                    channel:(NSString *)channel
                  packageID:(uint64_t)packageID
                      extra:(nullable NSDictionary *)extra;

+ (void)recordNetworkWithParams:(NSDictionary *)params
                networkResponse:(PAG_IESGurdNetworkResponse *)networkResponse;

@end

NS_ASSUME_NONNULL_END
