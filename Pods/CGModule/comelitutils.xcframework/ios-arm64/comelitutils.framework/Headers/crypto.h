/**
 *
 * @file crypto.h
 * Copyright (C) 2017 Comelit Group SpA
 *
 */

#ifndef CMPOT_CRYPTO_H
#define CMPOT_CRYPTO_H

#include <string>
#include <zlib.h>
#include <stdexcept>

#include "utils.h"

namespace Comelit {
namespace Utils    {

using std::string;

namespace Crypto {
/**
 * @brief aes128encrypt encrypt the input string by the input key in AES128
 * @param[in] encKey encoding key (must be 32-chars-long!)
 * @param[in] input input string to be encrypted in AES128
 * @return the input string encrypted in AES128
 */
string aes128encrypt(const string &encKey, const string &input);

/**
 * @brief aes256encrypt encrypt the input string by the input key in AES256
 * @param[in] encKey encoding key (must be 32-chars-long!)
 * @param[in] input input string to be encrypted in AES256
 * @return the input string encrypted in AES256
 */
string aes256encrypt(const string &encKey, const string &input);

/**
 * @brief aes256decrypt decrypt the input AES256-encrypted string by the input key
 * @param[in] encKey decoding key (must be 32-chars-long!)
 * @param[in] input input string to be decrypted from AES256
 * @return the input string decrypted from AES256
 */
string aes256decrypt(const string &encKey, const string &input);

/**
 * @brief b64encode encode the input string in base64
 * @param[in] input input string to be encoded in base64
 * @return the input string encoded in base64
 */
string b64encode(const string &input);

/**
 * @brief b64decode decode the input string from base64
 * @param[in] input input string to be decoded from base64
 * @return the input string decoded from base64
 */
string b64decode(const string &input);

/**
 * @brief gzipCompressString compress a string using GZIP compression format
 * @param[in] str input string
 * @param[in] compressionlevel
 * @param[in] useBaseAlgorithm use base DEFLATE algorithm, not the extended used in Gzip format
 * @return the compressed string on success; an empty string on failure
 */
string gzipCompressString(const string &str, int compressionlevel = Z_NO_COMPRESSION,
                          bool useBaseAlgorithm = false);

/**
 * @brief gzipDecompressString
 * @param[in] str input string
 * @return the decompressed string on success; an empty string on failure
 */
string gzipDecompressString(const string &str);

/**
 * @brief computeMD5 compute the MD5 for the inout string
 * @param[in] str input string to compute MD5 for
 * @return the computed MD5 string
 */
string computeMD5(const string &str);

/**
 * @brief computeSHA1 compute the SHA1 for the input string
 * @param[in] str: input string to compute SHA1 for
 * @return the computed SHA1 string
 */
string computeSHA1(const string &str);
}

} // MPot    NS
} // Comelit NS

#endif // CMPOT_CRYPTO_H
