//
//  OctClickWorker.h
//  OctCore
//
//  Created by ZY on 2020/2/24.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@protocol OctWebViewPageCloseDelegate <NSObject>

@optional

- (void)oct_webViewPageCloseButton:(NSString *)pageType;
- (void)oct_deepLinkSuccess:(BOOL)isSuccess;
@end

@interface OctClickWorker : NSObject

@property (nonatomic, copy) NSString *landingPageURL;
@property (nonatomic, copy) NSString *phoneNumber;
@property (nonatomic, copy) NSString *deeplinkURL;
@property (nonatomic, copy) NSString *spaceID;

@property (nonatomic, assign) int32_t interactType;
//为NO时，跳到外部浏览器
@property (nonatomic, assign) BOOL useBuiltInBrow;
@property (nonatomic, assign) BOOL removeSecondaryConfirmation;

@property (nonatomic, weak) id<OctWebViewPageCloseDelegate> delegate;

- (void)presentClickWithRootViewController:(UIViewController *)rootViewController;

@end

NS_ASSUME_NONNULL_END
