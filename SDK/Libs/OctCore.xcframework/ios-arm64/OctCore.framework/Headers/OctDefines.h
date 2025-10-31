//
//  OctDefines.h
//  OctAD
//
//  Created by zwf on 2020/12/9.
//

#import "OctDevice.h"
 
#ifndef OctDefines_h
#define OctDefines_h

// 广告平台类型
typedef NS_ENUM(NSUInteger, OctAdPlatformType) {
    OctAdPlatformTypeGDT = 1,    // 广点通
    OctAdPlatformTypeCSJ = 2,    // 穿山甲
    OctAdPlatformTypeKS  = 3,    // 快手
    OctAdPlatformTypeSigmob  = 4,    // Sigmob
    OctAdPlatformTypeBaiDu  = 6,    // baidu
    OctAdPlatformTypeGromore  = 8,    // gromore
    OctAdPlatformTypeOctAd  = 9,    // OctAd
    #pragma TODO:暂时写9
};

// 错误码
typedef NS_ENUM(NSInteger, OctAdErrorCode) {
    OctAdErrorCode100 = -100,    // 参数错误
    OctAdErrorCode101 = -101,    // 平台初始化错误
    OctAdErrorCode102 = -102,    // 广告位错误
    OctAdErrorCode103 = -103,    // 方法调用错误
    OctAdErrorCode104 = -104,    // 未找到视频
    OctAdErrorCode105 = -105,    // 未找到广告
    OctAdErrorCode106 = -106,    // 未找到视图
    OctAdErrorCode200 = -200,    // 网络错误
    OctAdErrorCode301 = -301,    // 加载广告失败
    OctAdErrorCode302 = -302,    // 平台没有返回广告
    OctAdErrorCode303 = -303,    // 广告加载超时
};

typedef NS_ENUM(NSUInteger,OctRewardedVideoAdType) {
    OctRewardedVideoAdTypeEndcard         = 0,  // video + endcard
    OctRewardedVideoAdTypeVideoPlayable   = 1,  // video + playable
    OctRewardedVideoAdTypePurePlayable    = 2,  // pure playable
};

typedef NS_ENUM(NSInteger, OctInteractionType) {
    OctInteractionTypeCustorm = 0,
    OctInteractionTypeNO_INTERACTION = 1,  // pure ad display
    OctInteractionTypeURL = 2,             // open the webpage using a browser
    OctInteractionTypePage = 3,            // open the webpage within the app
    OctInteractionTypeDownload = 4,        // download the app
    OctInteractionTypePhone = 5,           // make a call
    OctInteractionTypeMessage = 6,         // send messages
    OctInteractionTypeEmail = 7,           // send email
    OctInteractionTypeVideoAdDetail = 8    // video ad details page
};

typedef NS_ENUM(NSInteger, OctPlayerPlayState) {
    OctPlayerStateFailed    = 0,
    OctPlayerStateBuffering = 1,
    OctPlayerStatePlaying   = 2,
    OctPlayerStateStopped   = 3,
    OctPlayerStatePause     = 4,
    OctPlayerStateDefalt    = 5
};

typedef NS_ENUM(NSInteger, OctAdState) {
    OctAdStateLoadSuccess   = 0,
    OctAdStateLoadTimeOut   = 1,
    OctAdStateClose         = 2,
    OctAdStateClick         = 3,
    OctAdStateSkip          = 4,
    OctAdStateShow          = 5,
    OctAdStateLoadError     = 6,
    OctAdStateVideoComplete = 7,
    OctAdStateVideoReward   = 8,
    OctAdStateIsTimeover    = 9
};

typedef NS_ENUM(NSInteger, OctAdType) {
    OctAdTypeSplashAds = 0,
    OctAdTypeRewardAds,
    OctAdTypeBannerAds,
    OctAdTypeNativeAds,
    OctAdTypeDrawAds,
    OctAdTypeInterstitialAds,
    OctAdTypeFullScreenAds,
    OctAdTypeNativeExpressAds,
    OctAdTypeDrawCustomAds
};

typedef NS_ENUM(NSInteger, OctAdFlowCustomType) {
    OctAdFlowCustomTypeSingleImg = 0,
    OctAdFlowCustomTypeThreeImg,
    OctAdFlowCustomTypeVideo
};

typedef NS_ENUM(NSInteger, OctACSJCustomFlowType) {
    OctACSJCustomFlowTypeUnknown = 0,
    OctACSJCustomFlowTypeBanner,
    OctACSJCustomFlowTypeInterstitial,
    OctACSJCustomFlowTypeSplash,
    OctACSJCustomFlowTypeSplash_Cache,
    OctACSJCustomFlowTypeFeed,
    OctACSJCustomFlowTypePaster,
    OctACSJCustomFlowTypeRewardVideo,
    OctACSJCustomFlowTypeFullscreenVideo,
    OctACSJCustomFlowTypeDrawVideo
};

typedef NS_ENUM(NSUInteger, OctAdvertScene) {
    OctAdvertSceneSplash = 1,        // 开屏
    OctAdvertSceneInfoFlow = 2,      // 信息流
    OctAdvertSceneReward = 3,        // 激励视频
    OctAdvertSceneBanner = 4,        // banner
    OctAdvertSceneInter = 5,         // 插屏
    OctAdvertSceneVideoPaster = 6,   // 贴片视频
    OctAdvertSceneDraw = 7,          // Draw模板
    OctAdvertSceneContent = 8,       // 内容
    OctAdvertSceneFullVideo = 9,     // 全屏视频
    OctAdvertSceneInfoFlowExp = 10,  // 模板信息流
    OctAdvertSceneInfoFlowExp2 = 11, // 模板信息流2.0
    OctAdvertSceneReward2 = 12,      // 激励视频2.0
    OctAdvertSceneInterFull = 13,    // 插屏-全屏2.0
    OctAdvertSceneInterHalf = 14,    // 插屏-半屏2.0
    OctAdvertSceneDrawCustom = 15    // Draw自渲染
};

#define OctWeakSelf __weak __typeof(self)weakSelf = self;

#define KOctScreenWidth    [UIScreen mainScreen].bounds.size.width
#define KOctScreenHeight   [UIScreen mainScreen].bounds.size.height
#define KOctStatusHeight [OctDevice oct_statusBarHeight]
#define KOctNavigationHeight [OctDevice oct_navigationBarHeight]
#define KOctTopHeight [OctDevice oct_topHeight]
#define KOctTopOffset [OctDevice oct_safeHeight]
#define KOctBottomOffset [OctDevice oct_bottomOffset]
#define KOctTabbarHeight [OctDevice oct_tabbarHeight]
#define KOctDeviceWidthScale ([UIScreen mainScreen].bounds.size.width)/375.0

#define KOctTokenAuthorization @"Authorization"
#define KOctLogHeaderLevel1 @"OctAD Protocol: "

/// FLS
#define KFLSConfigPrefix @"fulishe"
#define KFLSAdAppKey @"KFlsAdAppKey"    // fls内聚合appkey
#define KFLSAdConfig @"KFLSAdConfig"    // fls内config
#define KFLSAdConfigCreateTime @"KFLSAdConfigCreateTime" // fls内记录Config下载时间

#endif
