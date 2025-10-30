//
//  VipCallFsmStatusChangeParser.h
//  Module
//
//  Created by Cornelli Fabio on 20/03/18.
//  Copyright © 2018 ComelitGroup. All rights reserved.
//

#import <Foundation/Foundation.h>

@class UnitEvt;

@interface VipCallFsmStatusChangeParser : NSObject

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithSysId:(NSInteger)sysId unitId:(NSInteger)unitId;

- (UnitEvt *)parseJSONDictionary:(NSDictionary *)jsonDictionary;

@end
