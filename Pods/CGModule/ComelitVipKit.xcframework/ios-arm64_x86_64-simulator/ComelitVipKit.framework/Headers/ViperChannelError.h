//
//  ViperChannelError.h
//  ComelitVipKit
//
//  Created by Cornelli Fabio on 12/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, ViperChannelError) {
    ViperChannelErrorOk = 0,
    ViperChannelErrorGenericError,
    ViperChannelErrorForbidden,
    ViperChannelErrorUnknown
};

