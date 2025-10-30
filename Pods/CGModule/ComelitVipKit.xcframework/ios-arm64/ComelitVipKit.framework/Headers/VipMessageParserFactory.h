//
//  VipMessageParserFactory.h
//  ComelitVipKit
//
//  Created by Cornelli Fabio on 20/08/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ComelitCoreKit/UnitMessageParserProtocol.h>

NS_ASSUME_NONNULL_BEGIN

@interface VipMessageParserFactory : NSObject<UnitMessageParserProtocol>

- (nullable UnitEvt *)eventFromJson:(nonnull NSData *)json;

@end

NS_ASSUME_NONNULL_END
