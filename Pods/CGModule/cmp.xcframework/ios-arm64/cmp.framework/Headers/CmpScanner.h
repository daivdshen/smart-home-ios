//
//  CmpScanner.h
//  Pods
//
//  Created by Cornelli Fabio on 23/11/21.
//

#import <Foundation/Foundation.h>

#import "CmpScanDevice.h"

NS_ASSUME_NONNULL_BEGIN

@interface CmpScanner : NSObject

- (instancetype)init;

- (NSArray<CmpScanDevice *> *) startScan:(NSString *)broadcast;
- (void)setIpFilter:(NSString *)ip mask:(NSString *)mask;
- (void)setVipAddressFilter:(NSString *)vipAddress mask:(NSString *)mask;
- (void)setMacAddressFilter:(NSString *)macAddress mask:(NSString *)mask;
- (void)setModelIdFilter:(NSString *)modelId;

@end

NS_ASSUME_NONNULL_END
