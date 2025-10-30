//
//  RtspMessageParser.h
//  Module
//
//  Created by Cornelli Fabio on 26/03/18.
//  Copyright © 2018 ComelitGroup. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "UnitMessageParserProtocol.h"

@interface RtspMessageParser : NSObject<UnitMessageParserProtocol>

/**
 Parse the received data and return the parsed object.
 
 *Note*: In order to mantain the compatibility with the existing Vip module, this method
 also post the result as NSNotification

 @param json JSON data to parse
 @return Parsed object, or nil if the message could not be handled
 */
- (nullable UnitEvt *)eventFromJson:(nonnull NSData *)json;

@end
