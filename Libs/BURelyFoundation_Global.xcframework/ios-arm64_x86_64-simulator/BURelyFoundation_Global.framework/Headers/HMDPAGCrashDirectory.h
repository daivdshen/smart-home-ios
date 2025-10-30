//
//  HMDPAGCrashDirectory.h
//  CaptainAllred
//
//  Created by sunrunwang on 2019/7/9.
//  Copyright © 2019 sunrunwang. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface HMDPAGCrashDirectory : NSObject

+ (void)setup;

@end

@interface HMDPAGCrashDirectory (Path)

@property(class, readonly, atomic) NSString *UUID;

@property(class, readonly, atomic) NSString *preparedDirectory;

@property(class, readonly, atomic) NSString *processingDirectory;

@property(class, readonly, atomic) NSString *currentDirectory;

+ (BOOL)checkAndMarkLaunchState;

+ (void)removeLaunchState;

@end
