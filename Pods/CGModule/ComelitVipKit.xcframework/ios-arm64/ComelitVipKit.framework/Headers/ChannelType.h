//
//  ChannelType.h
//  ComelitVipKit
//
//  Created by Cornelli Fabio on 12/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, ChannelType) {
   ChannelTypeUndef = -1,
   ChannelTypeInfo = 0,
   ChannelTypePush,
   ChannelTypeEcho,
   ChannelTypeUaut,
   ChannelTypeUadm,
   ChannelTypeUcfg,
   ChannelTypeFact,
   ChannelTypeCtpp,
   ChannelTypeCspb,
   ChannelTypeEchoSrv,
   ChannelTypeRtpc,
   ChannelTypeFrcg,
   ChannelTypeTcpSock,
   ChannelTypeMaxNumType
};
