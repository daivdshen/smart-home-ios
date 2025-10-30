//
//  ViperTunnelMessageParser.h
//  ComelitVipKit
//
//  Created by Cornelli Fabio on 20/03/18.
//  Copyright © 2018 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <ComelitCoreKit/UnitMessageParserProtocol.h>


@interface ViperTunnelMessageParser : NSObject<UnitMessageParserProtocol>

- (nullable UnitEvt *)eventFromJson:(nonnull NSData *)json;

@end
