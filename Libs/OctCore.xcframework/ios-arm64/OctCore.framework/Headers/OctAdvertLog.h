//
//  OctAdvertLog.h
//  OctAD
//
//  Created by zwf on 2020/12/29.
//

#import <UIKit/UIKit.h>

#ifdef DEBUG
# define OctLog(fmt, ...) [[OctAdvertLog sharedInstance] logDebugMessage:[NSString stringWithFormat:fmt, ##__VA_ARGS__]];
#else
# define OctLog(fmt, ...) NSLog(@"" fmt);
#endif

NS_ASSUME_NONNULL_BEGIN

@interface OctAdvertLog : NSObject

@property (nonatomic, assign) BOOL showDebugLog;
@property (nonatomic, strong) UITextView *textView;

+ (OctAdvertLog *)sharedInstance;

- (void)logDebugMessage:(NSString *)message;

@end

NS_ASSUME_NONNULL_END
