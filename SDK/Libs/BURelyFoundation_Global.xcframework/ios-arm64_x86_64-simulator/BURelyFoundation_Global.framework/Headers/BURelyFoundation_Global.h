//
//  BURelyFoundation.h
//  BURelyFoundation
//
//  Created by zth on 2022/7/8.
//

#import <Foundation/Foundation.h>

/// 依赖层版本号
FOUNDATION_EXPORT NSString *_Nonnull const BURelyFoundationGlobalVersion;



/**
 @Warning 请注意. 在这里import的文件需要
                       (1) 是独立模块的header文件. 不应该是独立模块中某一个具体的.h文件
                       (2) 请添加__has_include的判断
 */
#ifndef BURelyFoundation_Global_h
#define BURelyFoundation_Global_h

#if __has_include(<BURelyFoundation_Global/BURelyFoundation_Global.h>)  // 海外

# pragma mark - NET

#if __has_include(<BURelyFoundation_Global/PAGNetWorkHeader.h>)
#import <BURelyFoundation_Global/PAGNetWorkHeader.h>
#endif

# pragma mark - Foundation

#if __has_include(<BURelyFoundation_Global/PAGFoundationHeader.h>)
#import <BURelyFoundation_Global/PAGFoundationHeader.h>
#endif

# pragma mark - AFN

#if __has_include(<BURelyFoundation_Global/PAGAFNetworkingHeader.h>)
#import <BURelyFoundation_Global/PAGAFNetworkingHeader.h>
#endif


# pragma mark - SDWebImage

#if __has_include(<BURelyFoundation_Global/PAGSDWebImageHeader.h>)
#import <BURelyFoundation_Global/PAGSDWebImageHeader.h>
#endif


# pragma mark - YYModel

#if __has_include(<BURelyFoundation_Global/PAGYYModelHeader.h>)
#import <BURelyFoundation_Global/PAGYYModelHeader.h>
#endif

# pragma mark - ZFPlayer

#if __has_include(<BURelyFoundation_Global/PAG_ZFPlayer.h>)
#import <BURelyFoundation_Global/PAG_ZFPlayer.h>
#endif


# pragma mark - Zip

#if __has_include(<BURelyFoundation_Global/PAGZipHeader.h>)
#import <BURelyFoundation_Global/PAGZipHeader.h>
#endif

# pragma mark - Gecko

#if __has_include(<BURelyFoundation_Global/PAGGeckoHeader.h>)
#import <BURelyFoundation_Global/PAGGeckoHeader.h>
#endif

# pragma mark - SVGKit

#if __has_include(<BURelyFoundation_Global/PAGSVGKitHeader.h>)
#import <BURelyFoundation_Global/PAGSVGKitHeader.h>
#endif

#else                   // .a

# pragma mark - NET

#if __has_include("PAGNetWorkHeader.h")
#import "PAGNetWorkHeader.h"
#endif

# pragma mark - Foundation

#if __has_include("PAGFoundationHeader.h")
#import "PAGFoundationHeader.h"
#endif

# pragma mark - AFN

#if __has_include("PAGAFNetworkingHeader.h")
#import "PAGAFNetworkingHeader.h"
#endif

# pragma mark - SDWebImage

#if __has_include("PAGSDWebImageHeader.h")
#import "PAGSDWebImageHeader.h"
#endif


# pragma mark - YYModel

#if __has_include("PAGYYModelHeader.h")
#import "PAGYYModelHeader.h"
#endif

# pragma mark - ZFPlayer
#if __has_include("PAG_ZFPlayer.h")
#import "PAG_ZFPlayer.h"
#endif

# pragma mark - Zip

#if __has_include("PAGZipHeader.h")
#import "PAGZipHeader.h"
#endif

# pragma mark - Gecko

#if __has_include("PAGGeckoHeader.h")
#import "PAGGeckoHeader.h"
#endif

# pragma mark - APM

#if __has_include("PAGAPMHeader.h")
#import "PAGAPMHeader.h"
#endif

# pragma mark - SVGKit

#if __has_include("PAGSVGKitHeader.h")
#import "PAGSVGKitHeader.h"
#endif

#endif

#endif /* BURelyFoundation_Global_h */

