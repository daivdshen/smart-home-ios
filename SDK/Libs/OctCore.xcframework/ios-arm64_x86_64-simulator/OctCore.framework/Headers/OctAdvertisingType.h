//
//  OctAdvertisingType.h
//  Pods
//
//  Created by guantou on 2023/10/18.
//

#ifndef OctAdvertisingType_h
#define OctAdvertisingType_h

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSUInteger, OctAdvertisingViewType) {
    /** 错误*/
    kOctAdvertisingViewTypeUnknow      = -1,
    /** webView广告*/
    kOctAdvertisingViewTypeWebView     = 0,
    /** 视频广告*/
    kOctAdvertisingViewTypePlayerView  = 1,
    /** 图片广告*/
    kOctAdvertisingViewTypeImageView   = 2,
    /** 空白View*/
    kOctAdvertisingViewTypeNULL        = 3,
};

/* 定义请求类型的枚举 */
typedef NS_ENUM(NSUInteger, OctAdvertisingType) {
    kOctAdvertTypeRewardVideo             = 1,
    kOctAdvertTypeSplash                  = 2,
    kOctAdvertTypeInterstitial            = 3,
    kOctAdvertTypeBanner                  = 4,
    kOctAdvertTypeFullScreenVedio         = 5,
    kOctAdvertTypeNative                  = 6,
    kOctAdvertTypeDrawVedio               = 7,
};

/* 渠道枚举枚举 */
typedef NS_ENUM(NSUInteger, OctChannelType) {
    kOctChannelTypeUnknow                      = 0,
    kOctChannelTypeOct                     = 1,
    kOctChannelTypeKs                      = 2,
    kOctChannelTypeGdt                     = 3,
    kOctChannelTypeCsj                    = 4,
    kOctChannelTypeBd                      = 5,
    kOctChannelTypeLy                      = 6,
};

/* oct进度枚举 */
typedef NS_ENUM(NSUInteger, OctProgressType) {
    //  默认状态未匹配到
    kOctExecutionProgressTypeUnknow      = 0,
    //  匹配到当前渠道
    kOctExecutionProgressTypeMatching    = 1,
    //  广告请求
    kOctExecutionProgressTypeRequest     = 2,
    //  广告返回
    kOctExecutionProgressTypeSuccess     = 3,
    //  广告展示
    kOctExecutionProgressTypeShow        = 4,
    //  广告消失
    kOctExecutionProgressTypeDismiss     = 5,
    //  广告失败
    kOctExecutionProgressTypeFail        = 6,
    //  广告流程结束
    kOctExecutionProgressTypeDealloc     = 7,
};


/* 是否需要比价枚举 */
typedef NS_ENUM(NSUInteger, OctNeedComparePrices) {
    kOctNeedComparePricesUnknow               = 0,
    kOctNeedComparePricesYES                  = 1,    // 等最高价格
    kOctNeedComparePricesNo                   = 2,    // 不等最高价格，直接返回
};

static NSString *const kOctBuyerId = @"OCTOPUS";
static NSString *const kGdtBuyerId = @"GDT";
static NSString *const kBdBuyerId = @"BD";
static NSString *const kCsjBuyerId = @"CSJ";
static NSString *const kKsBuyerId = @"KS";
static NSString *const kLyBuyerId = @"RH";



#endif /* OctAdvertisingType_h */
