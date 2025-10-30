/**
 *
 * @file common.h
 * Copyright (C) 2017 Comelit Group SpA
 *
 */

#ifndef COMMON_H
#define COMMON_H

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iterator>
#include <list>
#include <sstream>
#include <thread>
#include <unordered_map>
#include <vector>

#ifndef __WIN32__ // LINUX, MACOSX, ...
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <signal.h>
#else // WINDOWS
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x0600
#include <winsock2.h>
#include <ws2tcpip.h>
#endif

#include <fcntl.h>
#include <sys/file.h>
#ifndef __WIN32__
#include <sys/mman.h>
#ifndef __MACH__
#include <sys/prctl.h>
#endif
#endif
#include <sys/stat.h>
#include <cstring>
#include <ctime>
#include <unistd.h>

#include <cmpot/log.h>

namespace Comelit {
namespace MPot    {

#define LOG_INDENT "                               "
#define MAX_WAIT_TICKS  2
#define NON_ESCAPE_CHARS " #$%&()*+,-./:;<>?@[]^_{|}~"
#define COPYRIGHT_NOTICE "(c) 2017 by Comelit R&D S.r.l. All rights reserved."
#define LOGADDR_NDIGITS 8  ///< Comelit IP System has 8 digits!

#define SET_THREAD_NAME(x) Common::setThreadName(x)
#define GET_THREAD_NAME    Common::getThreadName().c_str()

using std::back_inserter;
using std::basic_string;
using std::exception;
using std::function;
using std::getline;
using std::hash;
using std::hex;
using std::ifstream;
using std::istream;
using std::istream_iterator;
using std::istringstream;
using std::list;
using std::ostringstream;
using std::string;
using std::stringstream;
using std::tolower;
using std::thread;
using std::uppercase;
using std::vector;
using std::ws;

typedef uint8_t logaddr_main_t[LOGADDR_NDIGITS];
typedef vector<string> Tokens;

/**
 * @brief The Iface struct represents an active network interface
 */
struct Iface
{
public:
	Iface()
	{
		ip.s_addr      = 0;
		netmask.s_addr = 0;
		bcast.s_addr   = 0;
		id             = 0;
		name           = "";
		ifa_flags      = 0;
	}
	
	Iface(in_addr newIP, in_addr newNM, in_addr newBC, int newID, string newName, unsigned int ifaFlags)
	{
		ip        = newIP;
		netmask   = newNM;
		bcast     = newBC;
		id        = newID;
		name      = newName;
		ifa_flags = ifaFlags;
	}
	
	/**
	 * @brief operator == comparison function
	 * @param[in] toCompare Iface object to compare this one with
	 * @return true if the two interface are actually the same; false otherwise
	 */
	bool operator==(const Iface &toCompare)
	{
		return ip.s_addr      == toCompare.ip.s_addr      &&
		       netmask.s_addr == toCompare.netmask.s_addr &&
		       bcast.s_addr   == toCompare.bcast.s_addr   &&
		       id             == toCompare.id             &&
		       name           == toCompare.name;
	}
	
public:

	in_addr ip;
	in_addr netmask;
	in_addr bcast;
	int     id;      // numeric id used to distinguish among interfaces (assigned by getIfacesInfo() from if.h linux ifindex), can be set to TUNNEL_IFACE (-1)
	string  name;    // mnemonic name associated to the interface (e.g., "eth0")
	unsigned int ifa_flags; // standard network flags
};

typedef std::map<int, Iface> Ifaces;

/**
 * This namespace contains some platform-independent common utility functions
 */
namespace Common {

/// Retry times for any cloud request
static const std::vector<int> CLOUD_RETRY_TIMES = {1, 5, 10, 30, 60, 120, 240, 480, 600 }; // default retry interval [s]

/**
 * @brief executeProgram execute a command and return its output (if any)
 * @param[in] cmdLine is the command line to execute (program + parameters)
 * @param[out] results is a reference to a string object that will be filled with the output of the command
 * @return true if the operation succedeed, false otherwise
 * @note this function is "blocking" that is it blocks the caller until the job executed finishes
 */
bool executeProgram(const string &cmdLine, string &results);

/**
 * @brief executeProgram execute a command and return its output (if any)
 * @param[in] cmdLine is the command line to execute (program + parameters)
 * @return the output of the command as a string; if the operation failed, return an error message (string: "[ERROR]")
 * @note this function is "blocking" that is it blocks the caller until
 * the job executed finishes
 */
string executeProgram(const string &cmdLine);

/**
 * @brief getFileContents return the contents of a file as a string (string)
 * @return the contents of a file on success; string "[ERROR]" otherwise
 */
string getFileContents(const string &filePath);

/**
 * @brief intToBin convert an integer to the binary equivalent (expressed as string)
 * @param[in] toConvert is the number to convert to binary format
 * @return a string representation for the binary equivalent of the input number
 */
string intToBin(const int &toConvert);

/**
 * @brief isInteger Check if a string contains a valid number (integer-type)
 * @return true if the string contains a number (and can be converted into);
 *         false otherwise
 */
bool isInteger(string &toTest);

/**
 * @brief isValidIpAddress check if a string contains a valid IP address
 * @param[in] toCheck is the string to look into for a valid IP address
 * @return true if the input string contains a valid IP address; false otherwise
 */
bool isValidIpAddress(const string &toCheck);

/**
 * @brief split split a string in substring
 * @param[in] input input string to be splitted in substrings
 * @param[in] delim character used as delimiter for splitting
 * @return a vector with substrings found
 */
vector<string> split(const string &input, char delim);

/**
 * @brief split
 * @param[in] input input string to be splitted in substrings
 * @param[in] delim character used as delimiter for splitting
 * @param[out] elems output vector of found substrings
 * @return elems vector (reference to the same elems parameter)
 */
vector<string> &split(const string &input, char delim, vector<string> &elems);

/**
 * @brief findSubstring look for the first occurrence of a substring in a vector of strings
 * @param[in] container vector of strings to look for the input substring in
 * @param[in] toFind input substring to look for in the container
 * @return the position of the substring in the input container; -1 if the
 *         string is not found
 */
int findSubstring(const vector<string> &container, const string &toFind);

/**
 * @brief getFileLines get all lines of a file as a vector of strings
 * @param[in] filePath file path to be read
 * @return a vector of strings (lines of the given file)
 */
vector<string> getFileLines(const string filePath);

/**
 * @brief fileExists check if a file exists
 * @param[in] filePath file path to be checked for existence
 * @return true if file exists; false otherwise
 */
bool fileExists(const string &filePath);

/**
 * @brief dirExists check if a directory exists
 * @param dirPath  directory path to be checked for existence
 * @return true if the directory exists; false otherwise
 */
bool dirExists(const string &dirPath);

/**
 * @brief makeDir create a directory
 * @param dirPath directory path to be created
 * @return true if the directory was successfully created; false otherwise
 */
bool makeDir(const string &dirPath);

/**
 * @brief replace replace all occurrences of substring in a string with another substring
 * @param[in] input input string
 * @param[in] toFind substring to be searched into input string
 * @param[in] replacement replacement substring
 * @return a new string with all occurrences of the toFind replaced
 */
string replace(string input, const string &toFind, const string &replacement);

/**
 * @brief trim trim the input string (removed chars: space,
 *             '\n', '\r' and '\t')
 * @param[in] toTrim string to be trimmed
 * @return the trimmed string
 */
string trim(const string &toTrim);

/**
 * @brief serialize serialize a string, encoding special symbols
 * @param[in] input string to be serialized
 * @return the input string serialized (new string)
 */
string serialize(const string &input);

/**
 * @brief deserialize deserialize a string, decoding special symbols
 * @param[in] input string to be deserialized
 * @return the input string deserialized (new string)
 */
string deserialize(const string &input);

/**
 * @brief toLower convert all characters in string to lower
 * @param[in] input input string
 * @return the input string in lower format (new string)
 */
string toLower(string input);

/**
 * @brief logaddrMainToString
 * @param[in] toConvert input logaddr_main_t to be converted to string
 * @return a C++ std::string representation for the given logaddr_main_t
 */
string logaddrMainToString(const logaddr_main_t &toConvert);

/**
 * @brief logaddrMainCpy copy a logaddr to anotherone
 * @param[out] dest destination address
 * @param[in] src source address
 */
void logaddrMainCpy(logaddr_main_t dest, const logaddr_main_t src);

/**
 * @brief logaddrMainEq
 * @param[in] first first address to be compared
 * @param[in] second second address to be compared
 * @return true if the addresses are the same/equal; false otherwise
 */
bool logaddrMainEq(const logaddr_main_t first, const logaddr_main_t second);

bool logaddrMainSet(logaddr_main_t output, const string input);

/**
 * @brief getIfacesInfo get the information of network adapters with a configured
 * ip address, loopback interface is not included
 * @param[in] flags standard network interface flags
 * @return a map with information for network interfaces
 */
const Ifaces getIfacesInfo(unsigned int flags = IFF_BROADCAST);

bool getCorrectIface(const in_addr &matchAddr, Iface &matchIface,
                     unsigned int flags = IFF_BROADCAST);
                     
/**
 * @brief detectIface detect the presence of a network interface called \p ifName
 * @param ifName interface name
 * @return true if the \p ifName is detected on the system
 */
bool detectIface(const string ifName);

/**
 * Return the default IPv4 gateway for the specified interface.
 * @param[in]  ifName  Interface name (e.g. "eth0")
 * @param[out] gateway gateway address if it is found
 * @retval true returned gateway is valid
 * @retval false returned gateway is invalid
 */
bool getDefaultIPv4Gateway(const string &ifName, in_addr &gwAddr);

/**
* Return Iface with default gateway configured
*
* @return Iface, empty iface if no valid iface found
*/
Iface getGatewayIface();

/**
 * Return Iface with given name
 * @param[in] ifName
 * @param[out] iface
 * @param[in] flags standard network inteface flags
 * @return true on interface found
 */
bool getIface(const string ifName, Iface &iface, unsigned int flags = IFF_BROADCAST);

/**
 * Return Iface with given name
 * @param[in] ifId interface identifier
 * @param[in] flags standard network inteface flags
 * @param[out] iface
 * @return true on interface found
 */
bool getIface(const int ifId, Iface &iface, unsigned int flags = IFF_BROADCAST);

/**
 * @brief macToString convert a MAC address to a string
 * @param[in] macaddress input MAC address
 * @return the hex string represnetation for the input MAC address
 */
string macToString(const uint8_t *macaddress);

/**
 * @brief getMacAddress get the MAC address for the input network adapter
 * @param[in] ifName input interface name (default: "eth0")
 * @return the MAC address for the input network adapter; an empty string on failure
 */
string getMacAddress(const string &ifName = "eth0");

/**
 * @brief getMacAddress get the MAC address for the input network adapter
 * @param[out] ret output MAC Address structure (an array of uint8_t, length 6)
 * @param[in] ifName input interface name (default: "eth0")
 * @return true if the MAC address of the input network adapter was
 *         successfully obtained; false otherwise
 */
bool getMacAddress(uint8_t *ret, const string &ifName = "eth0");

/**
 * @brief ntop convert a sockaddr_in structure to a string
 * @param[in] input sockaddr_in structure to be converted to string
 * @return a string representation for the input sockaddr_in structure
 */
string ntop(const sockaddr_in *input);

/**
 * @brief ntop convert a in_addr sockaddr_in to a string
 * @param[in] input in_addr structure to be converted to string
 * @return a string representation for the input in_addr structure
 */
string ntop(const in_addr *input);

/**
 * @brief ntop convert a sockaddr_in structure to a string
 * @param[in] input sockaddr_in structure to be converted to string
 * @return a string representation for the input sockaddr_in structure
 */
string ntop(sockaddr_in *input);

/**
 * @brief ntop convert a in_addr sockaddr_in to a string
 * @param[in] input in_addr structure to be converted to string
 * @return a string representation for the input in_addr structure
 */
string ntop(in_addr *input);

/**
 * @brief toNumber templated function converting a string to a number.
 * The string is always interpreted as base 10 numbers.
 * @param[in] toConvert input string to be converted to number
 * @return the string converted to a number (of given type T)
 */
template <typename T>
typename std::enable_if<std::is_unsigned<T>::value, T>::type
toNumber(const string &toConvert)
{
	return static_cast<T>(std::strtoul(toConvert.c_str(), nullptr, 10));
}

template <typename T>
typename std::enable_if<std::is_signed<T>::value, T>::type
toNumber(const string &toConvert)
{
	return static_cast<T>(std::strtol(toConvert.c_str(), nullptr, 10));
}

template<>
long long toNumber<long long>(const string &toConvert);

template<>
unsigned long long toNumber<unsigned long long>(const string &toConvert);

/**
 * @brief toString convert an instance of type T to a string
 * @param[in] toConvert input instance of type T to be converted to string
 * @return the converted input as a string
 */
template <typename T>
inline string toString(T toConvert)
{
	return std::to_string(toConvert);
}

/**
 * toString specialization for float (backward compatibility)
 */
template <>
inline string toString<float>(float toConvert)
{
	ostringstream ss;
	ss << toConvert;
	return ss.str();
}

/**
 * toString specialization for double (backward compatibility)
 */
template <>
inline string toString<double>(double toConvert)
{
	ostringstream ss;
	ss << toConvert;
	return ss.str();
}


/**
 * @brief toString convert a string to string (actually a no-op)
 * @param[in] toConvert string to be simply returned (no need for conversion)
 * @return the input string
 * @note template specialization
 */
template <>
inline string toString<string>(string toConvert) { return toConvert; }

/**
 * @brief toString convert a C string to a std::string
 * @param[in] toConvert C string to be converted to std::string
 * @return the input string encapsulating the input C string
 * @note template specialization
 */
template <>
inline string toString<const char *>(const char *toConvert) { return toConvert; }

/**
 * @brief flatten flatten a vector of type T to a string
 * @param[in] input vector of type T to be flattened to a string
 * @return the flattened input vector as a string
 */
template<typename T>
string flatten(const vector<T> &input)
{
	string ret = "";
	
	for (size_t i = 0; i < input.size(); i++)
		ret += "." + Common::toString(input[i]);
		
	return ret;
}

/**
 * @brief timeToString convert a time_t structure to a string
 * @param[in] tm input time_t structure
 * @return a string representation for the input time_t structure (output format: YYYY-MM-DD HH:mm:SS)
 */
const string timeToString(const time_t &tm);

/**
 * @brief get_elem_map
 * @param key
 * @param map_template
 * @return
 */
template <typename K, typename T>
T *get_elem_map(K key, const std::map<K, T *> &map_template)
{
	auto elem_map = map_template.find(key);
	
	if (elem_map != map_template.end())
	{
		return elem_map->second;
	}
	
	return NULL;
}

typedef enum
{
	DATE_ONLY = 0,   ///< Consider only date, strftime format "%F"
	DATE_SECOND,     ///< Add time to date, strftime format "%F.%X"
	DATE_MILLISECOND ///< Add milliseconds to date and time, strftime format "%F.%X.%03d"
} DateTimeResolution;

/**
 * Format current time info as requested by @a dr and @a forceUTC parameters
 *
 * @param  dr time resolution
 * @param  forceUTC force time in UTC mode instead of localtime
 * @return the formatted string
 */
const string now(DateTimeResolution dr = DATE_SECOND, bool forceUTC = false);

/**
 * @brief contains check if a container include a particular value
 * @param[in] container input container to be inspected for the presence of the input value
 * @param[in] toCheck to be searched into the input container
 * return true if the container includes the input value; false otherwise
 */
template <typename T>
bool contains(T &container, typename T::value_type toCheck)
{
	return find(container.begin(), container.end(), toCheck) != container.end();
}

string getThreadName();
void   setThreadName(const char *);

#if !defined(__WIN32__) && !defined(OS_ANDROID)// LINUX, MACOSX, ...

/**
 * @brief initShMem open a POSIX shared memory area.
 * If the area does not exist, it is created and initialized it to all zeros.
 * @param[in]  fname  name of the POSIX shared memory object to open.
 * @param[in]  size   shared memory area in bytes.
 * @param[out] fd     location where the shared memory file descriptor will
 *                    be stored. Not NULL.
 * @return A pointer to the shared memory area, or NULL on error.
 */
void *initShMem(const string &fname, size_t size, int &fd);

/**
 * @brief lockShMem put a lock on shared memory segment among processes
 * @param[in] fd file descriptor for the shared memory segment to put the lock on
 */
void lockShMem(int fd);

/**
 * @brief unlockShMem unlock a locked shared memory
 * @param[in] fd file descriptor for the shared memory segment to unlock
 */
void unlockShMem(int fd);
#endif

typedef function<void (void)> DelayedCallback;

class DelayedExecution
{
private:

	DelayedCallback  callback;
	bool             thRunning;
	long             ticks;
	long             ticksUpd;
	string           thName;
	
public:

	DelayedExecution();
	
	DelayedExecution(long ticksBeforeUpdate);
	
	void setThreadName(string thName);
	
	void setCallback(DelayedCallback cb);
	
	void setTicksBeforeUpdate(long ticks);
	
	void onChange();
	
	void workerCode();
	
	void close();
};

typedef std::function<bool (unsigned int)> PeriodicExecutionCb;

struct PeriodicExecutionEntry
{
	unsigned int        periodMs;
	unsigned int        timeRef;
	PeriodicExecutionCb callback;
	
	PeriodicExecutionEntry(unsigned int period, PeriodicExecutionCb cb):
		periodMs(period), timeRef(0), callback(cb) { }
};

typedef std::vector<PeriodicExecutionEntry> PeriodicExecutions;

/**
 * @brief The PeriodicExecution class implements a timer with
 *        multiple attached callbacks. Each callback execution time
 *        is computed in order to maintain a regular execution for all
 *        callbacks. There is a callback parameter, ticks, which informs
 *        the caller on how many periods expired since last call.
 */
class PeriodicExecution
{
public:

	PeriodicExecution(): started(false) { }
	
	/**
	 * @brief addPeriodicCallback add a new callback to be periodically called
	 * @param periodMs period for the callback [ms]
	 * @param periodicCall callback function
	 *
	 * @note minimum period must be 20+ ms (suggested: 100 ms)
	 */
	void addPeriodicCallback(unsigned int periodMs, PeriodicExecutionCb &&periodicCall)
	{
		periodicCbs.emplace_back(PeriodicExecutionEntry(periodMs, periodicCall));
	}
	
	/**
	 * @brief setContinousCallback add a unique special callback to be called
	 *        as soon as possible (at least 10 ms between calls)
	 * @param continousCall callback function
	 */
	void setContinousCallback(PeriodicExecutionCb &&continousCall)
	{
		continousCb = continousCall;
	}
	
	/**
	 * @brief start start the timer (blocking call)
	 * @return false, if already started; otherwise true
	 */
	bool start();
	
	/**
	 * @brief stop stop the timer in execution
	 * @return false, if already stopped; otherwise true
	 */
	bool stop();
	
private:

	bool                started;
	PeriodicExecutions  periodicCbs;
	PeriodicExecutionCb continousCb;
};

#if !defined(__WIN32__) && !defined(__MACH__) // LINUX
typedef std::function<bool (void)> TimedAction; // Callback invoked when the timer expires

class Timer
{
public:

	Timer(TimedAction &&cb, time_t s, suseconds_t ns = 0);
	
	~Timer() { stop(); }
	
	bool start();
	
	void stop();
	
	// The signal handler function with extended signature
	static void alarmFunction(int sigNumb, siginfo_t *si, void *uc)
	{
		Timer *pTimer = reinterpret_cast<Timer *>(si->si_value.sival_ptr);
		
		if (pTimer->callback == NULL)
			return;
			
		// If the callbacks return false, stop the timer!
		if (!pTimer->callback())
			pTimer->stop();
	}
	
private:

	bool              started;
	timer_t           timerID;
	struct itimerspec timerSpecs;
	sigset_t          sigBlockSet;
	struct sigevent   signalEvent;
	struct sigaction  signalAction;
	TimedAction       callback;
};
#endif

/**
 * @brief The ByteArray class implements a small type-safe byte array
 *        structure. Actually it's a wrapper around a std::vector<uint8_t>.
 *
 * @see http://stackoverflow.com/q/259297
 */
class ByteArray
{
public:

	ByteArray() { }
	
	ByteArray(const char *cstr): buf(strlen(cstr))
	{
		memcpy(&buf[0], cstr, buf.size());
	}
	
	ByteArray(const char *cstr, size_t len): buf(len)
	{
		memcpy(&buf[0], cstr, buf.size());
	}
	
	ByteArray(const unsigned char *cstr): buf(strlen((const char *)cstr))
	{
		memcpy(&buf[0], cstr, buf.size());
	}
	
	ByteArray(const unsigned char *cstr, size_t len): buf(len)
	{
		memcpy(&buf[0], cstr, buf.size());
	}
	
	ByteArray(const string &str): buf(str.size())
	{
		memcpy(&buf[0], str.c_str(), buf.size());
	}
	
	ByteArray(vector<uint8_t> &&vec)
		: buf(std::move(vec)) { }
	ByteArray(size_t len)
		: buf(len) { }
	bool operator==(const ByteArray &other) const
	{
		if (buf.size() != other.buf.size())
			return false;
			
		return std::equal(buf.begin(), buf.begin() + buf.size(), other.buf.begin());
	}
	
	bool operator!=(const ByteArray &other) const
	{
		return !(*this == other);
	}
	
	bool operator<(const ByteArray &other) const
	{
		return this->buf < other.buf;
	}
	
	ByteArray &operator+=(const ByteArray &other)
	{
		buf.insert(buf.end(), other.buf.begin(), other.buf.end());
		return *this;
	}
	
	ByteArray operator+(const ByteArray &other) const
	{
		ByteArray tmp(*this);
		tmp += other;
		return tmp;
	}
	
	uint8_t *operator[](size_t pos)
	{
		if (pos >= buf.size())
			return NULL;
			
		return &buf[pos];
	}
	
	const uint8_t *operator[](size_t pos) const
	{
		if (pos >= buf.size())
			return NULL;
			
		return &buf[pos];
	}
	
	ByteArray &assign(const char *buffer, const size_t buflen)
	{
		buf.assign(buffer, buffer + buflen);
		return *this;
	}
	
	ByteArray substr(size_t from, size_t num) const
	{
		return ByteArray(vector<uint8_t>(buf.begin() + from, buf.begin() + from + num));
	}
	
	ByteArray substr(size_t from) const
	{
		return ByteArray(vector<uint8_t>(buf.begin() + from, buf.end()));
	}
	
	const uint8_t *data() const
	{
		return buf.data();
	}
	
	uint8_t *data()
	{
		return buf.data();
	}
	
	size_t size() const
	{
		return buf.size();
	}
	
	bool empty() const
	{
		return buf.size() == 0;
	}

	std::string toString() const
	{
		return string((const char *)buf.data(), buf.size());
	}

	static ByteArray encode8(const uint8_t param)
	{
		return ByteArray((const char *)&param, sizeof(param));
	}
	
	static ByteArray encode16LE(const uint16_t param)
	{
		return ByteArray((const char *)&param, sizeof(param));
	}
	
	static ByteArray encode32LE(const uint32_t param)
	{
		return ByteArray((const char *)&param, sizeof(param));
	}
	
	static ByteArray encode16BE(const uint16_t param)
	{
		uint16_t beParam = htons(param);
		return ByteArray((const char *)&beParam, sizeof(beParam));
	}
	
	static ByteArray encode32BE(const uint32_t param)
	{
		uint32_t beParam = htonl(param);
		return ByteArray((const char *)&beParam, sizeof(beParam));
	}
	
private:

	vector<uint8_t> buf;
};
}

} // MPot    NS
} // Comelit NS

#endif // COMMON_H
