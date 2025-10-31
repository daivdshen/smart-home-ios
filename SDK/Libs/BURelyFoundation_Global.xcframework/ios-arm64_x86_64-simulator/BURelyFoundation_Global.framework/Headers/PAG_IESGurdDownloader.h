//
//  PAG_IESGurdDownloader.h
//  Pods
//
//  Created by liuhaitian on 2020/4/24.
//

#import "PAG_IESGeckoDefines.h"

@interface PAG_IESGurdDownloader : NSObject

+ (void)downloadPackageWithIdentity:(NSString *)identity
                         URLStrings:(NSArray <NSString *> *)URLStrings
                         completion:(IESGurdDownloadResourceCompletion)completion;

+ (void)cancelDownloadWithIdentity:(NSString *)identity;

@end
