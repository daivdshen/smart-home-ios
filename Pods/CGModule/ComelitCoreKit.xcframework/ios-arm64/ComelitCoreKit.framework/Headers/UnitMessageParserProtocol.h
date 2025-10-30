//
//  UnitMessageParserProtocol.h
//  ComelitCoreKit
//
//  Created by Busi Andrea on 01/08/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import "UnitEvt.h"

@protocol UnitMessageParserProtocol <NSObject>

- (nullable UnitEvt *)eventFromJson:(nonnull NSData *)json;

@end
