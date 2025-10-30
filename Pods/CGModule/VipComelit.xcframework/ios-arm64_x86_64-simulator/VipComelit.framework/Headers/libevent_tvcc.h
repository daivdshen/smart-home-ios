#ifndef LIBEVENT_TVCC_H
#define LIBEVENT_TVCC_H

///Typedef for definition of message type (msg_type_id)
typedef enum {
	getDeviceReply = 0,
	getChannelReply,
	getChannelListReply,
	getStreamReply,
	getStreamListReply,
	setStreamResolutionReply,
	setStreamSourceReply,
	setStreamBitRateTypeReply,
	setStreamBitRateReply,
	setStreamFrameRateReply,
	setStreamQualityReply,
	setStreamEncoderReply,
	setStreamProfileReply,
	setStreamIFrameReply,
	setChannelBrightnessReply,
	setChannelContrastReply,
	setChannelSaturationReply,
	setChannelHueReply,
	changeSpeedPTZReply,
	movePTZStartReply,
	movePTZStopReply,
	addPresetPointPTZReply,
	gotoPresetPointPTZReply,
	getCruisePointInfoPTZReply,
	searchRecordByDayReply,
	startLiveStreamReply,
	stopLiveStreamReply,
	startPlaybackReply,
	stopPlaybackReply
}eMsgTypeTvcc;

///Typedef for answer result
typedef enum {
	TvccOk,
	TvccErrSet,
	TvccErrConn,
	TvccErrGetAddrInfo,
	TvccErrSocket,
	TvccErrSend,
	TvccErrSendBuf,
	TvccErrMem,
	TvccErrGet,
	TvccErrJson,
	TvccNotValid,
	TvccNotSupported,
	TvccErrNoJson,
	TvccErrPtz,
	TvccErrSearch,
	TvccNoCapability,
	TvccErrPlayback,
}eReplyTvcc;








#endif // LIBEVENT_TVCC_H
