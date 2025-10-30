/**
*
* @file parammanager.h
* Copyright (C) 2013 Comelit Group SpA
*
*/

#ifndef PARAMMANAGER_H
#define PARAMMANAGER_H

#include <fstream>
#include <set>

#include <regex.h>

#include <cmpot/param.h>
#include <cmpot/common.h>

#define CFG_REGEX_PATTERN \
	"^[[:space:]]*"                        /* start of line, optional spaces */ \
	"([[:alnum:].]*\\.[[:digit:].]*)"      /* key */ \
	"[[:space:]]*=[[:space:]]*"            /* "=" (with optional spaces around) */ \
	"([^=[:space:]](.*[^[:space:]])?)"     /* value */ \
	"[[:space:]]*$"                        /* optional spaces, end of line */

#define CFG_REGEX_OPTIONS REG_EXTENDED | REG_NEWLINE

namespace Comelit  {
namespace MPot     {

using std::pair;
using std::set;
using std::string;
using std::unordered_map;

typedef set<ParamGroup *>              ParamGroups;
typedef unordered_map<string, Param *> ParamMap;

/// This class allows to quickly handle and operate on Param objects
class ParamManager
{
private:

	ParamGroups groups;
	ParamMap    params;
	regex_t     cfgRe;

private:

	/**
	 * @brief splitKey split a string containg Param data into identifier (stem) and indexes
	 * @param[in] key input string to be splitted
	 * @param[out] stem identifier of the Param object
	 * @param[out] indexes indexes for the Param object
	 * @return true if splititng succeeded; false otherwise
	 */
	bool splitKey(const string key, string &stem, IndexOids &indexes);

	/**
	 * @brief setScalar try to fill a scalar Param object with value from the input string
	 * @param[in] raw string containing the data for a scalar Param object
	 * @param[out] target Param to be filled with data from the input string
	 * @return true if all operations succeeded; false otherwise
	 */
	bool setScalar(string &raw, Param *target);

	/**
	 * @brief setTable try to fill a table Param object with value from the input string
	 * @param[in] raw string containing the data for a table Param object
	 * @param[in] indexes identify the Table row we are operating on
	 * @param[out] target Param to be filled with data from the input string
	 * @return true if all operations succeeded; false otherwise
	 */
	bool setTableRow(const IndexOids &indexes, string &raw, Param *target);

	/**
	 * @brief splitTriplet split an SNMP triple value in each part
	 * @param[in] input input string to be splitted
	 * @param[in] offset position to start looking for the triplet from
	 * @param[in] colId column identifier
	 * @param[out] type type of value
	 * @param[out] payload actual content
	 * @return true if splitting succeeded; fals eotherwise
	 */
	bool splitTriplet(const string &input, size_t &offset, unsigned int &colId, ValueType &type,
	                  string &payload);


	/**
	 * @brief checkForDigit
	 * helper function, find a number inside a string starting at a given position
	 * and terminating wih a specific delimiter
	 * @param[in] input string to be parsed
	 * @param[in] terminator character which indicates the end of the digit string
	 * @param[in/out] i starting index, this will be updated to the index after the terminator char
	 * @param[out] out found number
	 * @return true if the number is found, false otherwise
	 */
	static inline bool checkForDigit(const string &input, char terminator, size_t &i,
	                                 unsigned int &out);

	/**
	 * @brief matchToString extract a value from a string by looking at the given regmatch_t range
	 * @param[in] input input string
	 * @param[in] match input string contating the range for substring extraction
	 * @return the extracted substring
	 */
	string matchToString(const string &input, const regmatch_t &match);

	/**
	 * @brief loadFromFile load a file and imports Param data
	 * @param[in] filePath file path to read from
	 */
	void loadFromFile(string filePath);

public:

	/**
	 * @brief ParamManager empty constructor
	 */
	ParamManager();

	/**
	 * @brief getGroups return the Param Groups holding Param objects
	 * @return all the groups holding Param objects
	 */
	ParamGroups &getGroups() { return groups; }

	/**
	 * @brief getParams return all Param object handled by this ParamManager object
	 * @return all Params handled by this ParamManager object
	 */
	ParamMap &getParams() { return params; }

	/**
	 * @brief loadGroup load all data from the given group (and its file)
	 * @param[in] group input group for reading its params from its file
	 */
	void loadGroup(ParamGroup &group);

	/**
	 * @brief loadAllGroups load all data from all groups (and their respective files)
	 */
	void loadAllGroups();

	/**
	 * @brief saveAllGroups save all data for all groups (on their respective files)
	 */
	void saveAllGroups();

	/**
	 * @brief operator[] return a pointer to Param associated to the input string
	 * @param[in] key Param identifier
	 * @return a pointer to the wanted Param; NULL value otherwise
	 * @note This method cannot be used to add a new Param! Use add() instead.
	 */
	Param *&operator[](string key);

	/**
	 * @brief add add a new Param to the ParamManager
	 * @param[in] key Param identifier (key)
	 * @param[in] param Param object
	 * @return true if no error occurred while adding the new entry; false otherwise
	 */
	bool add(string key, Param *param);
};

} // MPot     NS
} // Comelit  NS

#endif // PARAMMANAGER_H
