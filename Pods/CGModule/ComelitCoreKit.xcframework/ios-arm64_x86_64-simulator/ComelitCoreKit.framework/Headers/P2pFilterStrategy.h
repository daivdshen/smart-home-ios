//
//  P2pFilterStrategy.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 15/07/2019.
//  Copyright © 2019 Comelit Group Spa. All rights reserved.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, P2pFilterStrategy) {
    /// Use all the remote candidates
    P2pFilterStrategyNone = 0,
    /// Use only remote peer candidates
    P2pFilterStrategyOnlyPeer,
    /// Use only remote relayed candidates
    P2pFilterStrategyOnlyRelayed
};
