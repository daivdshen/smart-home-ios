//
//  AVBufferParameter.h
//  ComelitCoreKit
//
//  Created by Cornelli Fabio on 22/12/21.
//

#import <Foundation/Foundation.h>

typedef NS_ENUM(NSInteger, AVBufferParameter) {
    AVBufferParameterAudioRxPacketBufferLength = 0, ///< Circular Buffer Lengtgh for Audio RX pcks (default 3)
    AVBufferParameterVideoRxPacketBufferLength,     ///< Circular Buffer Lengtgh for Video RX pcks (default 12)
    AVBufferParameterAudioRxPacketPreBuffering,     ///< Audio RX packet buffering before beginning the removal from the queue (default 0)
    AVBufferParameterVideoRxPacketPreBuffering,     ///< Video RX packet buffering before beginning the removal from the queue (default 0)
    AVBufferParameterRtpRxPacketBufferLength,       ///< Circular Buffer Lengtgh for RTP raw RX pcks (default 0 disabled)
    AVBufferParameterRtpRxPacketPreBuffering        ///< Video RX packet buffering before beginning the removal from the queue (default 0)
};
