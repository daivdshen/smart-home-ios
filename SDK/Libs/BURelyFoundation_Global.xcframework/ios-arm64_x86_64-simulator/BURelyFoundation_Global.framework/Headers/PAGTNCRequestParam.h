//
// Created by bytedance on 2020/12/16.
//

#import <Foundation/Foundation.h>
#import "PAGCommonMacros.h"

@interface PAGTNCRequestParam : NSObject <PAGDictionarify>

@property (nonatomic, copy) NSString *version;

@property (nonatomic, copy) NSString *deviceDid;

@property (nonatomic, copy) NSString *ssAppID;

@end