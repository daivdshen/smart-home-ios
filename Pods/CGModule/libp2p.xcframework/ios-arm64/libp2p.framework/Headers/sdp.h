/**
 * @file   sdp.h
 *
 * Handles SDP management
 *
 * @author Dario Corna
 */

#ifndef SDP_H
#define SDP_H

#include <string>
#include <list>
#include <memory>

/**
 *  The SdpAttribute struct
 * https://tools.ietf.org/html/rfc4566#section-5.13
 */
struct SdpAttribute
{
	/**  Attribute name */
	std::string name;
	/**  Attribute value */
	std::string value;
	/**  Attribute direction (e.g., for extmap) */
	std::string  direction;

	/**
	 *  parseGlobal parse a global attribute line
	 * @param[in] input
	 * @return sdp attribute struct
	 */
	static SdpAttribute parseGlobal(char *input);
};

/**
 *  The SdpMLine struct
 * https://tools.ietf.org/html/rfc4566#section-5.14
 *
 */
struct SdpMLine
{
	/**  SDP m-line representation */
	/**  Media type (string) */
	std::string type;
	/**  Media port */
	int port;
	/**  Media protocol */
	std::string proto;
	/**  List of formats */
	std::list<std::string> fmts;
	/**  List of payload types */
	std::list<int> ptypes;
	/**  Media c= protocol */
	bool c_ipv4;
	/**  Media c= address */
	std::string c_addr;
	/**  Media b= type */
	std::string b_name;
	/**  Media b= value */
	uint32_t b_value;
	/**  Media direction */
	std::string  direction;
	/**  List of m-line attributes */
	std::list<SdpAttribute> attributes;

	static std::shared_ptr<SdpMLine> parse(char *input);

	/**
	 * parseAttribute parse mline attribute line to add to attributes list
	 * @param[in] input
	 */
	void parseAttribute(char *input);

	/**
	 * getComputedMLine
	 * appends to given stringstream the computed string to
	 * send as sdp answer, considering video and audio
	 * codec supported.
	 * @param[in,out] ss
	 * @param[in] port
	 * @param[in] ip4
	 * @return true on success
	 */
	bool getComputedMLine(std::stringstream &ss, int port, char *ip4);
	int getVideoCodecPT(); ///< get payload type
	int getAudioCodecPT(); ///< get payload type
	uint32_t getSsrc(); ///< get rtp SSRC
};

/**
 * The Sdp class
 * handles an sdp offer and compute answer
 *
 * Usage:
 * - Constructor with char* to input sdp
 * - parseSdp
 * - getComputedAttributes get computed global attributes
 * - access to public members to get attributes and mlines
 */
class Sdp
{
public:
	Sdp(const char *inputRemote);
	~Sdp();
	bool parseSdp(); ///< parse sdp and generate attributes and mlines
	bool getComputedAttributes(std::stringstream &ss); ///< get computed global attributes

public:
	char *inputRemote;
	std::list<SdpAttribute> attributes; ///< global attributes
	std::list<std::shared_ptr<SdpMLine>> mLines;
};

#endif // SDP_H
