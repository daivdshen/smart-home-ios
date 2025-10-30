//
//  RtspFsmStatusChangeParser.h
//  Module
//
//  Created by Cornelli Fabio on 26/03/18.
//  Copyright © 2018 ComelitGroup. All rights reserved.
//

#import <Foundation/Foundation.h>

@class RtspFsmStatusChangeEvt;

@interface RtspFsmStatusChangeParser : NSObject

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithSysId:(NSInteger)sysId unitId:(NSInteger)unitId;

- (RtspFsmStatusChangeEvt *)parseJSONDictionary:(NSDictionary *)jsonDictionary;

@end
