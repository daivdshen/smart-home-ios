/**
 * @file cmpserviceprovider.h
 *
 * CmpServiceProvider class. Class that implement cmp services
 *
 * The CMPService class allows to perform a CMP scan and get the result in form
 * of a list of comelit devices found in a specific network.
 * To note that in order to retrieve the result a callback has to be registered.
 *
 * @author Fabio Bigoni, Roberto Muzzì
 */

#ifndef CMPSERVICEPROVIDER_H
#define CMPSERVICEPROVIDER_H

#include <sys/types.h>
#ifndef __WIN32__
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#else
#include <winsock2.h>
#include <ws2tcpip.h>
#endif
#include <unistd.h>

#include <string.h>
#include <functional>
#include <iostream>
#include <list>

//for Mac OS X
#include <stdlib.h>

using namespace std;

#define PACKED                  __attribute__ ((packed))

#define IP_SIZE                 4
#define FOURCC_SIZE             4
#define MAC_ADDR_SIZE           6
#define LOGADDR_NDIGITS         8                                                       ///< Comelit IP System has 8 digits!

#ifndef __WIN32__
typedef int skt_t;
#else
typedef SOCKET skt_t;
#endif

typedef struct PACKED
{
	char          opcode[FOURCC_SIZE];
	unsigned int  devId;
	unsigned char macAddr[MAC_ADDR_SIZE];
	unsigned char flags;
} CMPHere;

typedef struct PACKED
{
	unsigned char  opcode[FOURCC_SIZE];
	unsigned char  vipAddress[LOGADDR_NDIGITS];
	unsigned short subAddress;
	unsigned char  macAddr[MAC_ADDR_SIZE];
	unsigned char  hwID[FOURCC_SIZE];
	unsigned char  appID[FOURCC_SIZE];
	unsigned int   appVerCode;
	unsigned char  appVerString[80];
	unsigned char  sysID[FOURCC_SIZE];
	unsigned char  description[36];
	unsigned char  flags;
	unsigned char  opStatus;
	unsigned char  macStatus;
	unsigned char  reserved;
	unsigned char  modelID[FOURCC_SIZE];
	unsigned char  videoEnc;
	unsigned char  videoDec;
	unsigned char  audioEnc;
	unsigned char  audioDec;
	unsigned short extenderFlags;
	unsigned int   JSONConfProtVersion;
	unsigned int   JSONConfDescVersion;
	unsigned char  JSONConfDecType;
} CMPInfo;

typedef struct PACKED
{
	unsigned char  opcode[FOURCC_SIZE];
	unsigned int   minDevSid;
	unsigned int   maxDevSid;
	unsigned char  filterLogAddr[LOGADDR_NDIGITS];
	unsigned char  filterLogAddrMask[LOGADDR_NDIGITS];
	unsigned short subAddressMin;
	unsigned short subAddressMax;
	union {
		uint32_t       ipAddr;
		unsigned char  ipAddrByte[IP_SIZE];
	};
	union {
		uint32_t       ipAddrMask;
		unsigned char  ipAddrMaskByte[IP_SIZE];
	};
	unsigned char  filterMacAddr[MAC_ADDR_SIZE];
	unsigned char  filterMacAddrMask[MAC_ADDR_SIZE];
	unsigned char  hwID[FOURCC_SIZE];
	unsigned char  appID[FOURCC_SIZE];
	unsigned char  sysID[FOURCC_SIZE];
	unsigned char  modelID[FOURCC_SIZE];
} CMPscan;

typedef struct
{
	string ip;
	uint16_t port;
	CMPHere here;
	CMPInfo info;
	int cmpSize;
} CmpDevice;


class CmpServiceProvider
{

public:
	typedef std::function<void(CmpDevice *, void *)> CallbackFunction;

	typedef std::function<void(unsigned int lvl, const char *emitter, const char *params)>
	LogCbkFunction;

	CmpServiceProvider();
	~CmpServiceProvider();

	/**
	 * @brief start allows to start the CMP scan
	 * @param broadcast the broadcast ip to use
	 * @return 0 if everything is ok, -1 otherwise
	 */
	int scan(const char *broadcast) __attribute((deprecated));

	/**
	 * @brief start allows to start the CMP scan
	 * @note no callback is called
	 * @param[in] broadcast the broadcast ip to use
	 * @param[out] devList devices found
	 * @return 0 if everything is ok, -1 otherwise
	 */
	int scan(const char *broadcast, list<CmpDevice> &devList);

	/**
	 * Function to set filter for log address (for scan request)
	 * @param vipAddress  a Main Logical Address to be matched.
	 * @param mask        bit mask that tells which parts of the Main Logical Address are to be compared
	 */
	void setLogAddrFilter(char vipAddress[LOGADDR_NDIGITS], char mask[LOGADDR_NDIGITS]);

	/**
	 * Function to set filter for mac address (for scan request)
	 * @param macAddress  a MAC address to be matched.
	 * @param mask        bit mask that tells which parts of the MAC address are to be compared
	 */
	void setMacAddrFilter(char macAddress[MAC_ADDR_SIZE], char mask[MAC_ADDR_SIZE]);

	/**
	 * Function to set filter for ip address (for scan request)
	 * @param[in] ip a ip address to be matched.
	 * @param[in] netmask a netmask address to be matched.
	 */
	void setIpFilter(const in_addr &ip, const in_addr &netmask);

	/**
	 * @brief Select in how many interval divide scan operation
	 * @param[in] intervalSize must be >= 1 (default:1)
	 * @retval false \p intervalSize == 0. Old value is manteined.
	 * @retval true new interval is applied
	 */
	bool setDevSidInterval(unsigned int intervalSize);

	/**
	 * Function to set filter for hwID (for scan request)
	 * @param hwID a hardware-id to be matched.
	 */
	void setHwIDFilter(unsigned char hwID[FOURCC_SIZE]);

	/**
	 * Function to set filter for appID (for scan request)
	 * @param appID a applicaion-id to be matched.
	 */
	void setAppIDFilter(unsigned char appID[FOURCC_SIZE]);

	/**
	 * Function to set filter for sysID (for scan request)
	 * @param sysID a system-id to be matched.
	 */
	void setSysIDFilter(unsigned char sysID[FOURCC_SIZE]);

	/**
	 * Function to set filter for modelID (for scan request)
	 * @param modelID a model-id to be matched.
	 */
	void setModelIDFilter(unsigned char modelID[FOURCC_SIZE]);

	/**
	 * Enable restrict mode, from result are removed devices that don't match
	 * with selected filter. Some devices infact always answer to cmp (ex: rcmp enabled devices)
	 * @param[in] enable turn on/off restrict mode (default is off)
	 */
	void setRestrictFilter(bool enable);

	/**
	 * @brief setResultCallback allows to setup the callback used to return the result of the scan
	 * @param cb callback function
	 * @param arg callback arg
	 */
	void setResultCallback(CallbackFunction cb, void *arg) __attribute((deprecated));

private:
	CMPscan scanSt;
	skt_t sd;
	int rc, i;
	socklen_t otherLen;
	struct sockaddr_in myAddr, remoteServAddr;
	struct sockaddr_in otherAddr;
	struct hostent *host;
	int broadcast = 1;
	list<CmpDevice *> devices;
	CallbackFunction
	resultCallback;                ///< callback internal representation of the callback registered by a third-party
	void *resultCallbackArg;
	unsigned int devIdIntervalSize;
	bool restricted;

	/**
	 * @brief start allows to start the CMP scan
	 * @param[in] broadcast the broadcast ip to use
	 * @param[out] devList if null callback is call otherwise list is
	 *             filled with found devices
	 * @return 0 if everything is ok, -1 otherwise
	 */
	int scan(const char *broadcast, list<CmpDevice> *devList);

	/**
	 * @brief startScan send the message 'SCAN'
	 * @param broadcast address
	 * @return 0 if everything is ok, -1 otherwise
	 */
	int startScan(string addr);

	/**
	 * @brief startInfo send the message 'INFO'
	 * @param device pointer to device to get info
	 * @return 0 if everything is ok, -1 otherwise
	 */
	int startInfo(CmpDevice *device, std::list<CmpDevice> *list = nullptr);

	/**
	 * @brief setupSocket allows to setup the socket connection
	 * @param isBroadcast specify if the socket can send broadcast messages
	 * @return 0 if everything is ok, -1 otherwise
	 */
	int setupSocket(bool isBroadcast);

	/**
	 * @brief setupRemoteAddress allows to setup the ip:port to instaurate the communication
	 * @param address of the destination
	 * @param port of the destination
	 * @return 0 if everything is ok, -1 otherwise
	 */
	int setupRemoteAddress(string address, uint16_t port);

	/**
	 * Search if devId passed is into devices list
	 * @param devId
	 * @return 1 if found, 0 if not
	 */
	int searchDevId(unsigned int devId, const string &devIp);

	/**
	 * Check if \p cmpDev match with selected filters
	 * @note usefull only if #setRestrictFilter is active
	 * @param cmpDev cmp device answer
	 * @return 0 if everything is ok, -1 otherwise
	 */
	int getRestrict(const CmpDevice &cmpDev);

	/**
	 * Function to close socket (shutdown + close)
	 * @param fd
	 */
	void closeSocket(skt_t fd);

	static bool maskMatch(unsigned int len,
	                      const void *a,
	                      const void *b,
	                      const void *mask);
};
#endif // CMPSERVICEPROVIDER_H
