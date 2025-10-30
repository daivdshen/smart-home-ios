/**
 * @file   rtputils.h
 *
 * Wrapper for rtp/rtcp utility functions
 *
 * @author Dario Corna
 */
#ifndef RTPUTILS_H
#define RTPUTILS_H

class RtpUtils
{
public:
	static const int AUDIO_PT_PCMU;
	static const int AUDIO_PT_PCMA;

	static bool packetIsRtp(const char *buf, int len);
	static bool packetIsRtcp(const char *buf, int len);
};
#endif // RTPUTILS_H
