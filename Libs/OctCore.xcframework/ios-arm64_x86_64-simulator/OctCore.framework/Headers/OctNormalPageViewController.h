//
//  OctNormalPageViewController.h
//  OctCoreSDK
//

//

#import <UIKit/UIKit.h>
#import "OctBaseViewController.h"

NS_ASSUME_NONNULL_BEGIN

@protocol OctNormaPageCloseDelegate <NSObject>

@optional

- (void)oct_NormaPageCloseButton;

@end

@interface OctNormalPageViewController : OctBaseViewController

@property (nonatomic, copy) NSString *loadURL;

@property (nonatomic, weak) id<OctNormaPageCloseDelegate> delegate;

@end

NS_ASSUME_NONNULL_END
