//
//  RtspFsmEventParser.h
//  Module
//
//  Created by Cornelli Fabio on 26/03/18.
//  Copyright © 2018 ComelitGroup. All rights reserved.
//

#import <Foundation/Foundation.h>

@class RtspFsmEventEvt;

@interface RtspFsmEventParser : NSObject

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithSysId:(NSInteger)sysId unitId:(NSInteger)unitId;

- (RtspFsmEventEvt *)parseJSONDictionary:(NSDictionary *)jsonDictionary;

@end
