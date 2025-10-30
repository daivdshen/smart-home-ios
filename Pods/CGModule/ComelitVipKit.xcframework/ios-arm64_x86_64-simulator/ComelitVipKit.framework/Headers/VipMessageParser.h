//
//  VipMessageParser.h
//  Module
//
//  Created by Cornelli Fabio on 20/03/18.
//  Copyright © 2018 ComelitGroup. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ComelitCoreKit/UnitMessageParserProtocol.h>

@interface VipMessageParser : NSObject<UnitMessageParserProtocol>

- (nullable UnitEvt *)eventFromJson:(nonnull NSData *)json;

@end
