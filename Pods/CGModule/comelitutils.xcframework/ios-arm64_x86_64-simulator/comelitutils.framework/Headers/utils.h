#ifndef LIBCOMELIT_UTILS_H
#define LIBCOMELIT_UTILS_H

// Typedefs ===============================================================

#include <stdint.h> //or <inttypes.h> (in the global namespace)
#include <iostream>
#include <string>
#include <functional>
#include <vector>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <csignal>
#include <ctime>
#include <zlib.h>

using std::string;

typedef uint64_t u64;
typedef  int64_t s64;
typedef uint32_t u32;
typedef  int32_t s32;
typedef uint16_t u16;
typedef  int16_t s16;
typedef uint8_t  u8;
typedef  int8_t  s8;

typedef  uint8_t u8_t;
typedef uint16_t u16_t;
typedef uint32_t u32_t;
typedef uint64_t u64_t;

namespace Comelit {
namespace Utils   {

// AeS128 class ========================================================

void aes128_encrypt(uint8_t *input, const uint8_t *key, uint8_t *output);
void aes128_decrypt(uint8_t *input, const uint8_t *key, uint8_t *output);

// AeS128 class ========================================================

typedef unsigned char u8;

typedef struct
{
	u8 key[32];
	u8 enckey[32];
	u8 deckey[32];
} aes256_context;


void aes256_init(aes256_context *, u8 * /* key */);
void aes256_done(aes256_context *);
void aes256_encrypt_ecb(aes256_context *, u8 * /* plaintext */);
void aes256_decrypt_ecb(aes256_context *, u8 * /* cipertext */);

// AlawUtils class ========================================================

class AlawUtils
{
public:
	AlawUtils();

	/**
	 * Funzione che calcola l'esponente del campione audio per il companding A-law.
	 * L'esponente Ë espresso in soli 3bit (valore da 0 a 7), codifica la posizione
	 * del primo bit a 1 dopo il bit di segno.
	 *
	 * @param value campione audio non compresso di cui calcolare l'esponente A-law
	 */
	static u16 g711_find_exponent(u16 value);

	/**
	 * Funzione che applica la decompressione A-law ad una serie di campioni audio
	 * compressi. Necessita che i bit pari vengano invertiti prima di iniziare
	 * la decompressione.
	 *
	 * @param out_buf buffer dove vengono riportati i campioni audio decompressi
	 * @param in_buf  buffer dove sono presenti i campioni audio compressi.
	 * @param count   numero di campioni audio da decomprimere.
	 */
	static void g711_alaw2linear(s16 *out_buf, u8 *in_buf, u16 count);

	/**
	 * Funzione che applica la compressione A-law ad una serie di campioni audio
	 * lineari. La compressione A-law prevede che il campione audio sia a 13 bit
	 * per cui non vengono considerati i 3 LSb del campione audio ricevuto.
	 * Il bit di segno viene riportato nel bit 7 del campione compresso.
	 * Secondo specifica, a fine compressione prima di trasmettere i bit compressi
	 * vengono invertiti i bit pari.
	 *
	 * @param out_buf buffer dove vengono riportati i campioni audio compressi
	 * @param in_buf  buffer dove vengono presenti i campioni non compressi.
	 * @param count   numero di campioni audio da comprimere.
	 */
	static void g711_linear2alaw(u8 *out_buf, s16 *in_buf, u16 count);
};


string base64_encode(unsigned char const *, unsigned int len);
unsigned int base64_decode(string const &s, uint8_t *decoded_bytes);
string base64_decode(string const &encoded_string);

// H264Utils class ========================================================

class H264Utils
{
public:
	H264Utils(char *pStart, unsigned short nLen);
	/**
	 * Get the H264 profile of the video stream
	 *
	 * @return stream's profile
	 */

	unsigned int getProfile() {return profile_idc;}
	/**
	 * Get the width retrieved from SPS
	 *
	 * @return picture's width
	 */

	int getwidth() {return width;}
	/**
	 * Get the height retrieved from SPS
	 *
	 * @return picture's height
	 */
	int getHeight() {return height;}

private:
	inline unsigned int readBit();
	unsigned int readBits(int n);
	unsigned int readExpGolomb();
	unsigned int readSE();
	void parse();
public:

private:
	char *m_pStart;
	unsigned short m_nLength;
	unsigned int nIndex, nOffset;
	unsigned int profile_idc;
	int width;
	int height;
};

// Crc32 class ============================================================

class Crc32
{
public:

	/**
	 * Compute the 32-bit CRC
	 * This CRC can be specified as:
	 *
	 * Width : 32, Poly : 0x04c11db7, RefIn : false, RefOut : false, XorOut : 0.
	 *
	 * This differs from the "standard" CRC-32 algorithm in that the values are not reflected,
	 * and there is no final XOR value.
	 * These differences make it easy to compose the values of multiple blocks.
	 *
	 * @param buf buffer
	 * @param len length of buffer
	 * @param init starting value
	 *
	 * @return the crc32
	 */
	static unsigned int xcrc32(const unsigned char *buf, int len, unsigned int init);

};

// Crc16 class ============================================================

/// Seme con il quale iniziare la generazione del CRC16 e del CRC32 in base al sistema
#if defined(SYS_ELITE)
#define CRC16_SEED					0x0001
#define CRC32_SEED					0x00000001
#else
#define CRC16_SEED					0xFFFF
#define CRC32_SEED					0xFFFFFFFF
#endif


class Crc16
{
public:

	/*
	 * Derived from CRC algorithm for JTAG ICE mkII, published in Atmel
	 * Appnote AVR067.
	 */

	static unsigned short crc16_sum(const unsigned char *message,
	                                unsigned long length,
	                                unsigned short crc);
	/*
	 * Verify that the last two bytes is a (LSB first) valid CRC of the
	 * message.
	 */
	static int crc16_verify(const unsigned char *message,
	                        unsigned long length);
	/*
	 * Append a two byte CRC (LSB first) to message.  length is size of
	 * message excluding crc.  Space for the CRC bytes must be allocated
	 * in advance!
	 */
	static void crc16_append(unsigned char *message,
	                         unsigned long length);
	/*
	 * Define the crc16 from parameters Seed and Polinomial, so it is
	 * indipendent to ::crc_table.
	 */
	static unsigned short crc16_sum_pol(const unsigned char *message, unsigned long length,
	                                    unsigned short crc, unsigned short polinomial);

};

// ---------------------------------------------------------------------------

/* MD5
 converted to C++ class by Frank Thilo (thilo@unix-ag.org)
 for bzflag (http://www.bzflag.org)

   based on:

   md5.h and md5.c
   reference implementation of RFC 1321

   Copyright (C) 1991-2, RSA Data Security, Inc. Created 1991. All
rights reserved.

License to copy and use this software is granted provided that it
is identified as the "RSA Data Security, Inc. MD5 Message-Digest
Algorithm" in all material mentioning or referencing this software
or this function.

License is also granted to make and use derivative works provided
that such works are identified as "derived from the RSA Data
Security, Inc. MD5 Message-Digest Algorithm" in all material
mentioning or referencing the derived work.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.

*/

// a small class for calculating MD5 hashes of strings or byte arrays
// it is not meant to be fast or secure
//
// usage: 1) feed it blocks of uchars with update()
//      2) finalize()
//      3) get hexdigest() string
//      or
//      MD5(std::string).hexdigest()
//
// assumes that char is 8 bit and int is 32 bit
class MD5
{
public:
	typedef unsigned int size_type; // must be 32bit

	MD5();
	MD5(const std::string &text);
	void update(const unsigned char *buf, size_type length);
	void update(const char *buf, size_type length);
	MD5 &finalize();
	std::string hexdigest() const;
	friend std::ostream &operator<<(std::ostream &, MD5 md5);

private:
	void init();
	typedef unsigned char uint1; //  8bit
	typedef unsigned int uint4;  // 32bit
	enum {blocksize = 64}; // VC6 won't eat a const static int here

	void transform(const uint1 block[blocksize]);
	static void decode(uint4 output[], const uint1 input[], size_type len);
	static void encode(uint1 output[], const uint4 input[], size_type len);

	bool finalized;
	uint1 buffer[blocksize]; // bytes that didn't fit in last 64 byte chunk
	uint4 count[2];   // 64bit counter for number of bits (lo, hi)
	uint4 state[4];   // digest so far
	uint1 digest[16]; // the result

	// low level logic operations
	static inline uint4 F(uint4 x, uint4 y, uint4 z);
	static inline uint4 G(uint4 x, uint4 y, uint4 z);
	static inline uint4 H(uint4 x, uint4 y, uint4 z);
	static inline uint4 I(uint4 x, uint4 y, uint4 z);
	static inline uint4 rotate_left(uint4 x, int n);
	static inline void FF(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void GG(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void HH(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
	static inline void II(uint4 &a, uint4 b, uint4 c, uint4 d, uint4 x, uint4 s, uint4 ac);
};

std::string md5(const std::string &str);


class SHA1Hasher
{
public:

	static const size_t HASH_LENGTH  = 20; ///< Hash size in bytes (= 160 bit)

public:

	SHA1Hasher();

	void           writeByte(uint8_t data);
	void           write(const void *data, size_t len);
	const uint8_t *result();

	static const int sha1sum(const std::string &fileName);
	static const std::string sha1hex(const std::string str);

private:

	void           initHmac(const uint8_t *key, int keyLength);
	const uint8_t *resultHmac();

	void init();
	void hashBlock();
	void addUncounted(uint8_t data);
	void pad();

private:

	static const size_t BLOCK_LENGTH = 64;

private:

	uint32_t buffer[BLOCK_LENGTH/4];
	uint32_t state[HASH_LENGTH/4];
	uint32_t byteCount;
	uint8_t  bufferOffset;
	uint8_t  keyBuffer[BLOCK_LENGTH];
	uint8_t  innerHash[HASH_LENGTH];
};

// PeriodicExecution class ================================================

#if defined(IOS)
#include "timing_utils.h"
#else
typedef std::function<bool (unsigned int)> PeriodicExecutionCb;

class PeriodicExecution;

struct PeriodicExecutionEntry
{
	PeriodicExecutionEntry(unsigned int period, PeriodicExecutionCb cb);
	unsigned int             periodMs;
	unsigned int             timeRef;
	timespec                 timeStart;
	timer_t                  timerId;
	struct itimerspec        timePeriod;
	struct sigevent          sigEv;
	PeriodicExecutionCb      callback;
	PeriodicExecution        *object;
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

	PeriodicExecution();

	~PeriodicExecution();

	/**
	 * @brief addPeriodicCallback add a new callback to be periodically called
	 * @param periodMs period for the callback [ms]
	 * @param periodicCall callback function
	 *
	 * @note minimum period must be 20+ ms (suggested: 100 ms)
	 */
	void addPeriodicCallback(unsigned int periodMs, PeriodicExecutionCb &&periodicCall);

	/**
	 * @brief setContinousCallback add a unique special callback to be called
	 *        as soon as possible (at least 10 ms between calls)
	 * @param continousCall callback function
	 */
	void setContinousCallback(PeriodicExecutionCb &&continousCall);

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

	/**
	 * Notify function of all timers created by PeriodicExecution class
	 * @param input data notification
	 */
	static void timerCallback(union sigval sigVal);

private:

	bool                    started;
	PeriodicExecutions      periodicCbs;
	static std::mutex       timerMutex;
	std::mutex              condMutex;
	std::condition_variable condition;

	/**
	 * Function to calculate the difference of two struct timespec
	 * @param start time
	 * @param stop time
	 * @param (stop - start) time
	 */
	static void timespec_diff(timespec *start, timespec *stop, timespec *result);

	/**
	 * Disable and delete timers in a given PeriodicExecution object
	 * @param pointer to PeriodicExecution
	 * @return true if stop is performed, false if already stopped
	 */
	static bool deleteTimers(PeriodicExecution *obj);
};

#endif //IOS

class TimeRef
{
public:
	TimeRef()
	{
		timeRef     = std::chrono::steady_clock::now();
		running = false;
	}

	void start()
	{
		running = true;
		timeRef     = std::chrono::steady_clock::now();
	}

	void stop()
	{
		running = false;
	}

	std::chrono::steady_clock::time_point timeRef; /// used for internal timer

	double elapsed()
	{
		std::chrono::duration<double, std::ratio<1, 1>> elapsedReq = std::chrono::steady_clock::now() -
		                                                             timeRef;
		return elapsedReq.count();
	}

	double elapsedMs()
	{
		std::chrono::duration<double, std::milli> elapsedReq = std::chrono::steady_clock::now() - timeRef;
		return elapsedReq.count();
	}

	bool isRunning() { return running; }
private:
	bool running;
};

// --------------------------------------

/**
 * @brief The steady_condition_variable class
 * Is a re-implementation of std::condition variable class, which
 * uses monotonic clock instead of system clock.
 * Most of the implementation is copied as it is from the std version
 * in order to be full compatible.
 * only relevant changes are:
 * - constructor, where pthread_condattr_setclock is used to set CLOCK_MONOTONIC option
 * - definition of __clock_t, which is set to steady_clock instead of system_clock
 *
 */
class steady_condition_variable
{
	typedef std::chrono::steady_clock	__clock_t;
	typedef pthread_cond_t	__native_type;

	pthread_condattr_t _M_attr;
#ifdef __GTHREAD_COND_INIT
	__native_type	_M_cond = __GTHREAD_COND_INIT;
#else
	__native_type	_M_cond = PTHREAD_COND_INITIALIZER;
#endif

public:
	typedef __native_type	*native_handle_type;

	steady_condition_variable() noexcept;
	~steady_condition_variable();

	steady_condition_variable(const steady_condition_variable &) = delete;
	steady_condition_variable &operator=(const steady_condition_variable &) = delete;

	void
	notify_one() noexcept;

	void
	notify_all() noexcept;

	void
	wait(std::unique_lock<std::mutex> &__lock);


	template<typename _Predicate>
	void
	wait(std::unique_lock<std::mutex> &__lock, _Predicate __p)
	{
		while (!__p())
			wait(__lock);
	}

	template<typename _Duration>
	std::cv_status
	wait_until(std::unique_lock<std::mutex> &__lock,
	           const std::chrono::time_point<__clock_t, _Duration> &__atime)
	{ return __wait_until_impl(__lock, __atime); }

	template<typename _Clock, typename _Duration>
	std::cv_status
	wait_until(std::unique_lock<std::mutex> &__lock,
	           const std::chrono::time_point<_Clock, _Duration> &__atime)
	{
		const typename _Clock::time_point __c_entry = _Clock::now();
		const __clock_t::time_point __s_entry = __clock_t::now();
		const auto __delta = __atime - __c_entry;
		const auto __s_atime = __s_entry + __delta;

		return __wait_until_impl(__lock, __s_atime);
	}

	template<typename _Clock, typename _Duration, typename _Predicate>
	bool
	wait_until(std::unique_lock<std::mutex> &__lock,
	           const std::chrono::time_point<_Clock, _Duration> &__atime,
	           _Predicate __p)
	{
		while (!__p())
			if (wait_until(__lock, __atime) == std::cv_status::timeout)
				return __p();

		return true;
	}

	template<typename _Rep, typename _Period>
	std::cv_status
	wait_for(std::unique_lock<std::mutex> &__lock,
	         const std::chrono::duration<_Rep, _Period> &__rtime)
	{ return wait_until(__lock, __clock_t::now() + __rtime); }

	template<typename _Rep, typename _Period, typename _Predicate>
	bool
	wait_for(std::unique_lock<std::mutex> &__lock,
	         const std::chrono::duration<_Rep, _Period> &__rtime,
	         _Predicate __p)
	{ return wait_until(__lock, __clock_t::now() + __rtime, std::move(__p)); }

	native_handle_type
	native_handle()
	{ return &_M_cond; }

private:
	template<typename _Dur>
	std::cv_status
	__wait_until_impl(std::unique_lock<std::mutex> &__lock,
	                  const std::chrono::time_point<__clock_t, _Dur> &__atime)
	{
		auto __s = std::chrono::time_point_cast<std::chrono::seconds>(__atime);
		auto __ns = std::chrono::duration_cast<std::chrono::nanoseconds>(__atime - __s);

		struct timespec  __ts =
		{
			static_cast<std::time_t>(__s.time_since_epoch().count()),
			static_cast<long>(__ns.count())
		};

		pthread_cond_timedwait(&_M_cond, __lock.mutex()->native_handle(),
		                       &__ts);

		return (__clock_t::now() < __atime
		        ? std::cv_status::no_timeout : std::cv_status::timeout);
	}
};

} // Utils NS
} // Comelit  NS

#endif // LIBCOMELIT_UTILS_H
