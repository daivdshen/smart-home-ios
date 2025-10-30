//
//  NSUserDefaults+BUCrypt.h
//  BUFoundation
//
//  Created by Willie on 2020/9/11.
//  Copyright © 2020 bytedance. All rights reserved.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface NSUserDefaults (PAGCrypt)

- (void)pag_synchronize;

@end

NS_ASSUME_NONNULL_END
