//
//  PAG_IESGurdResourceManager+Business.h
//  IESGeckoKit
//
//  Created by chenyuchuan on 2019/6/26.
//

#import "PAG_IESGeckoResourceManager.h"

#import "PAG_IESGurdPackagesConfigRequest.h"
#import "PAG_IESGurdPackagesConfigResponse.h"

NS_ASSUME_NONNULL_BEGIN

typedef void(^PAG_IESGurdPackagesConfigCompletion)(IESGurdSyncStatus status, PAG_IESGurdPackagesConfigResponse * _Nullable response);

@interface PAG_IESGurdResourceManager (Business)

+ (void)ies_fetchConfigWithRequest:(PAG_IESGurdPackagesConfigRequest *)request
                        completion:(PAG_IESGurdPackagesConfigCompletion)completion;

@end

NS_ASSUME_NONNULL_END
