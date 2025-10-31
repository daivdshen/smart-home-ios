//
//  PAG_IESGurdKit+CustomParams.h
//  IESGeckoKit
//
//  Created by 陈煜钏 on 2020/3/1.
//

#import "PAG_IESGeckoKit.h"

NS_ASSUME_NONNULL_BEGIN

@interface PAG_IESGurdSyncResourcesParams : NSObject

- (PAG_IESGurdSyncResourcesParams *(^)(NSString *groupName))groupName;

- (PAG_IESGurdSyncResourcesParams *(^)(NSString *businessDomain))businessDomain;

- (PAG_IESGurdSyncResourcesParams *(^)(IESGurdSyncStatusDictionaryBlock completion))completion;
    
@end

NS_ASSUME_NONNULL_END
