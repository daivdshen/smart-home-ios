//
//  NSError+BUPAG_IESGurdKit.h
//  IESGeckoKit
//
//  Created by chenyuchuan on 2019/6/10.
//

#import <Foundation/Foundation.h>

#import "PAG_IESGeckoDefines.h"

NS_ASSUME_NONNULL_BEGIN

extern NSString * const kPAG_IESGurdKitErrorDomain;

@interface NSError (PAG_IESGurdKit)

+ (instancetype)pag_ies_errorWithCode:(IESGurdSyncStatus)status description:(NSString *)description;

@end

NS_ASSUME_NONNULL_END
