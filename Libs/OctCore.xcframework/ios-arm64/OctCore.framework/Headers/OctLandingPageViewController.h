//
//  OctLandingPageViewController.h
//  OctCoreSDK
//
//  Created by guantou on 2020/2/24.
//

#import <UIKit/UIKit.h>
#import "OctBaseViewController.h"

NS_ASSUME_NONNULL_BEGIN

@protocol OctLandingPageCloseDelegate <NSObject>

@optional

- (void)oct_landingPageCloseButton;

@end

@interface OctLandingPageViewController : OctBaseViewController

@property (nonatomic, copy) NSString *loadURL;

@property (nonatomic, copy) NSString *nextloadURL;

@property (nonatomic, assign) int32_t interactType;
@property (nonatomic, assign) BOOL removeSecondaryConfirmation;

@property (nonatomic, assign) BOOL isAlert;

@property (nonatomic, weak) id<OctLandingPageCloseDelegate> delegate;

@end

NS_ASSUME_NONNULL_END
