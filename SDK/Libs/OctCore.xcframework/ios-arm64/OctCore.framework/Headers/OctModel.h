//
//  OctModel.h
//  OctCoreSDK
//
//  Created by guantou on 2020/1/6.
//

#import <Foundation/Foundation.h>

#if __has_include(<OctModel/OctModel.h>)
FOUNDATION_EXPORT double OctModelVersionNumber;
FOUNDATION_EXPORT const unsigned char OctModelVersionString[];
#import <OctModel/NSObject+OctModel.h>
#import <OctModel/OctClassIvarInfo.h>
#else
#import "NSObject+OctModel.h"
#import "OctClassInfo.h"
#endif
