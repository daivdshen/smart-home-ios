//
//  Role.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 15/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, Role) {
    RolePorter = 0,
    RoleIntUnit,
    RoleCps,
    RoleCfp,
    RoleCamera,
    RoleVidOut,
    RoleIO,
    RoleUnknown = 100,
    RoleInvalidValue
};
