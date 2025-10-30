//
//  ConnError.h
//  ComelitVipKit
//
//  Created by Mutti Simone on 01/10/17.
//  Copyright © 2017 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, ConnError) {
    ConnErrorUnexpectedMsg = 0,
    ConnErrorCtpDisconn
};
