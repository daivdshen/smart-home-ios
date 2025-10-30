//
//  PAGEnvironment.h
//  BUFoundation
//
//  Created by bytedance on 2020/10/29.
//  Copyright © 2020 Union. All rights reserved.
//

#ifndef PAGEnvironment_h
#define PAGEnvironment_h

#import <Foundation/Foundation.h>
#import <CoreTelephony/CTTelephonyNetworkInfo.h>

CTTelephonyNetworkInfo *PAGDefaultTelephonyNetworkInfo(void);

@interface PAGEnvironment : NSObject

@end

#endif /* PAGEnvironment_h */
