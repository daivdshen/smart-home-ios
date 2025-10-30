/**
 *
 * @file param.h
 * Copyright (C) 2013 Comelit Group SpA
 *
 */

#ifndef PARAM_H
#define PARAM_H

#include <fstream>
#include <sstream>

// Forward declaration: we cannot include cmpot/mibtypes.h (it is not installed
// if SNMP is disabled), but luckily here we use only SNMPInfo pointers.
namespace Comelit  {
namespace MPot     {
namespace SNMP     {
class SNMPInfo;
}
}
}

#include <cmpot/tabledef.h>
#include <cmpot/common.h>

#define OUTPUT_FORMAT(name, type, value) \
	(name) << ".0 = 0:" << (type) << ":" << (value);

#define OUTPUT_FORMAT_CELL(oid, type, value) \
	(oid) + ":" + (type) + ":" + (value) + " "

#define THREAD_SLEEP   1
#define NO_NAME		   "__NO_NAME__"
#define DO_SAVE        ParamSavePolicy::SAVE
#define DO_NOT_SAVE    ParamSavePolicy::DONT_SAVE
#define EMPTY_RANGE    Range(NO_RANGE_LIMIT, NO_RANGE_LIMIT)

namespace Comelit  {
namespace MPot     {

using namespace SNMP;

using std::pair;
using std::ofstream;
using std::stringstream;
using std::thread;

class Param;
class ParamGroup;

typedef pair<unsigned long, unsigned long> Range;

enum class ParamSavePolicy
{
	SAVE, DONT_SAVE
};

class Param: public IChangeListener
{
private:

	string            name;        /// name (key) of this Param
	ParamValidator    validator;
	ParamFiller       filler;
	ParamPostSets     callbacks;   /// vector of callback invoked when something happens
	ParamGroup       *group;       /// pointer to the group this Param belongs to
	SNMPInfo         *snmpInfo;    /// SNMP object info associated to this Param (can be NULL)
	Variant           startValue;/// start value for the param
	Variant		      value;       /// actual value for this Param
	ValueType         type;        /// actual type for this Param
	ParamSavePolicy   savePolicy;  /// save or not save on file
	Range             numRange;    /// numeric range

private:

	/**
	 * @brief fillWithDefaultValue initialize attribute 'value' with the proper default value.
	 * It assigns that value by looking at the given 'type' attribute.
	 */
	void fillWithDefaultValue();

	/**
	 * @brief fillValue assign the input value to the 'value' attribute
	 * @param[in] initValue value to copy from
	 */
	void fillValue(Variant &initValue);

	/**
	 * @brief fillSnmpInfo initialize SNMP Agent++ structure (if this param
	 * already contains a non-null snmpInfo object).
	 * If SNMP is not implemented, does nothing.
	 */
	void fillSnmpInfo();

	string serializeScalar();

	/**
	 * @brief saveScalar save the scalar value contained in this param on disk
	 * @param[in] outFile output stream to write to
	 */
	void saveScalar(FILE *outFile);

	/**
	 * @brief isSpecialColumn check if the specified table column is 'special' or not
	 * @param[in] tDef table definion containing columns info
	 * @param[in] colId id of the column to be checked
	 * @return true if the column is a index column or the row status column; false otherwise (that is is a field column)
	 */
	bool isSpecialColumn(TableDef *tDef, unsigned int colId);

	/**
	 * @brief flattenTableRow return a whole table row as a string
	 * @param[in] tDef table definition used to get values from the row
	 * @param[in] row input row to extract values from
	 * @return a string representation for the input row
	 */
	string flattenTableRow(TableDef *tDef, TableRow &row);

	string serializeTable();

	/**
	 * @brief saveTable save the table contained in this param on disk
	 * @param[in] outFile output stream to write to
	 */
	void saveTable(FILE *outFile);

	/**
	 * @brief notifyAll send a message to all registered listeners
	 * @param[in] message message to send to all registered listeners (default: empty string)
	 * @param[in] info extra information object (if available)
	 * @note currently the ExtraInfo object is only used to provide some details about a table row change
	 */
	void notifyAll(const string &message = "", const ExtraInfo *info = NULL);

	/**
	 * @brief Return a the internal SNMPInfo* entry as a void*.
	 * @return If ENABLE_SNMP is defined, returns
	 *         snmpInfo->entryAs<void *>(). otherwise, NULL.
	 * This is needed in @a snmpEntryAs() to avoid including
	 * cmpot/mibtypes.h (it is not installed if ENABLE_SNMP is not
	 * defined).
	 */
	void *getSnmpEntryAsVoidPtr();

public:

	/**
	 * @brief Param basic constructor (without an initial value)
	 * @param[in] entryGroup group name which this param will belong to
	 * @param[in] entryType type of this Param
	 * @param[in] sInfo pointer to a SNMP info object (default: NULL)
	 * @param[in] saveOnFile enable/disable saving on disk (default: true)
	 * @param[in] range defines range limits for the param (default: no limits)
	 */
	Param(ParamGroup      &entryGroup,
	      ValueType        entryType,
	      SNMPInfo        *sInfo      = NULL,
	      ParamSavePolicy  saveOnFile = ParamSavePolicy::SAVE,
	      Range            range      = EMPTY_RANGE);

	/**
	 * @brief Param extended constructor allowing to specify an initial
	 * value for this Param
	 * @param[in] entryGroup group name which this param will belong to
	 * @param[in] entryType type of this Param
	 * @param[in] initValue initial value for this Param (must be of/consistent with
	 * the entryType argument)
	 * @param[in] sInfo pointer to a SNMP info object (default: NULL)
	 * @param[in] saveOnFile enable/disable saving on disk (default: true)
	 * @param[in] range defines range limits for the param (default: no limits)
	 */
	Param(ParamGroup      &entryGroup,
	      ValueType        entryType,
	      Variant          initValue,
	      SNMPInfo        *sInfo      = NULL,
	      ParamSavePolicy  saveOnFile = ParamSavePolicy::SAVE,
	      Range            range      = EMPTY_RANGE);

	/**
	 * @brief resetToStartValue return to the initial value for this param
	 */
	void resetToStartValue();

	/**
	 * @brief getGroup return the group this Param belongs to
	 * @return a pointer to the group this Param belongs to
	 */
	ParamGroup *getGroup();

	/**
	 * @brief setName set or change the internal pointer to the real name
	 * (string) allocated elsewhere (that is in the ParamManager). This way Param
	 * does not waste space replicating its name but it knows how to obtain its name.
	 * @param[in] newName pointer to the string with the name (key) of this param
	 */
	void setName(const string &newName);

	/**
	 * @brief getName return the name (key) of this param
	 * @return the name (key) associated to this Param
	 */
	string getName();

	/**
	 * @brief getType return the type of this Param
	 * @return the type of this Param
	 */
	ValueType getType();

	/**
	 * @brief getSnmpInfo return the SNMP object associated to this Param (can be NULL)
	 * @return the SNMP object inside this Param (or NULL if there isn't)
	 */
	SNMPInfo *getSnmpInfo();

	/**
	 * @brief getValue return the value of this Param
	 * @return a constant reference of the value of this Param
	 */
	const Variant &getValue() const;

	/**
	 * @brief getValue return the value of this Param
	 * @return a reference to the the value of this Param
	 */
	Variant &getValue();

	/// Quick getters
	const string         &getIPAddress()    { return value.getIPAddress();    }
	const logaddr_main_t &getLogaddrMain()  { return value.getLogaddrMain();  }
	const long           &getLong()         { return value.getLong();         }
	const bool            getBool()         { return value.getLong() != 0;    } // standard C-style comparison
	const bool            getSnmpBool()     { return value.getSnmpBool();     }
	const string         &getString()       { return value.getString();       }
	const Table          &getTable()        { return value.getTable();        }
	Table                *getMutableTable() { return value.getMutableTable(); }
	const TimeTicksValue  getTimeTicks()    { return value.getTimeTicks();    }

	/**
	 * @brief setValue set an integer value for this Param
	 * @param[in] newValue integer value to be assigned to the 'value' attribute of this Param
	 */
	void setValue(const int &newValue);

	/**
	 * @brief setValue set a long value for this Param
	 * @param[in] newValue long value to be assigned to the 'value' attribute of this Param
	 */
	void setValue(const long &newValue);

	/**
	 * @brief setValue set a TimeTicks value for this Param
	 * @param[in] newValue TimeTicks value to be assigned to the 'value' attribute of this Param
	 */
	void setValue(TimeTicksValue &newValue);

	/**
	 * @brief setValue set a logaddr_main_t (CSPStack's logical address) value for this Param
	 * @param[in] newValue logaddr_main_t value to be assigned to the 'value' attribute of this Param
	 */
	void setValue(const logaddr_main_t &newValue);

	/**
	 * @brief setValue set a Variant value for this Param
	 * @param[in] newValue Variant value to be assigned to the 'value' attribute of this Param
	 */
	void setValue(const Variant &newValue);

	/**
	 * @brief setValue set a string value for this Param
	 * @param[in] newValue string value to be assigned to the 'value' attribute of this Param
	 */
	void setValue(string &newValue);

	/**
	 * @brief setValue set a string value for this Param
	 * @param[in] newValue string value to be assigned to the 'value' attribute of this Param
	 */
	void setValue(const char *newValue);

	/**
	 * @brief setValue set a Table value for this Param
	 * @param[in] newValue Table value to be assigned to the 'value' attribute of this Param
	 */
	void setValue(const Table &newValue);

	/**
	 * @brief setValidator set validation function to process data before setting the value
	 * @param[in] newValidator validation function for validating each new value
	 */
	void setValidator(const ParamValidator &&newValidator);

	/**
	 * @brief setFiller set filler function to process data for setting the value
	 * @param[in] newFiller filler function for setting new value
	 */
	void setFiller(const ParamFiller &&newFiller);

	/**
	 * @brief addPostSet add a post-set callback (that's a new listener) to the callbacks list
	 * @param[in] callback new callback to be added to the list of listeners that will be
	 * informed when something particular happens
	 */
	void addPostSet(const ParamPostSet &&callback);

	/**
	 * @brief save save this Param on disk
	 * @param[in] outFile output stream to write to
	 */
	string serialize();

	/**
	 * @brief save save this Param on disk
	 * @param[in] outFile output stream to write to
	 */
	void save(FILE *outFile);

	/**
	 * @brief isSnmpEntry check if this Param contains a valid SNMP object
	 * @return true if the Param contains a non-null SNMP object; false otherwise.
	 *         If SNMP is not implemented, always returns false.
	 */
	bool isSnmpEntry();

	/**
	 * @brief fill allow to fill an input variant value
	 * @param[in] value fillable (that is "modifiable") value
	 * @param[in] extra information associated to the value (used for tables)
	 * @return true to signal that filling suceeded; false otherwise
	 */
	bool fill(Variant &value, const ExtraInfo *info = NULL);

	/**
	 * @brief validate validate a value
	 * @param[in] value value to be validated
	 * @param[in] extra information associated to the value (used for tables)
	 * @return true if values is valid; false otherwise
	 */
	bool validate(const Variant &value, const ExtraInfo *info = NULL);

	/**
	 * @brief onChange callback called when something happened elsewhere
	 * @param[in] message description of the event happened/message associated to this event
	 * @param[in] info extra information object (if available)
	 * @note: IChangeListener override
	 * @note currently the ExtraInfo object is only used to provide some details about a table row change
	 */
	void onChange(const string &message = "", const ExtraInfo *info = NULL);

	/**
	 * @brief hasRange check if the Param value has an associated range
	 * @return
	 */
	bool hasRange()
	{
		return (numRange.first  != NO_RANGE_LIMIT &&
		        numRange.second != NO_RANGE_LIMIT);
	}

	template<typename T>
	T *snmpEntryAs()
	{
		if (!isSnmpEntry())
			return NULL;

		// We cannot call snmpInfo->entryAs when !ENABLE_SNMP, so we trick
		// the compiler by extracting a void* and then converting to T*.
		return (T *) getSnmpEntryAsVoidPtr();
	}

	bool setSnmpEntryVisibility(bool isVisible);

	bool setSnmpEntryReadOnly(bool forceReadOnly);

	/**
	 * @brief setTableCellValue quick shortcut function to update the value of a table cell
	 * @param[in] rowIds ids of the table row containing the cell
	 * @param[in] colId column identifier
	 * @param[in] newValue new value for the cell
	 * @return true if operation succeedes; false othrwise
	 *
	 * @note this function also update the SNMP part inside this Param (if any)
	 *
	 * @note this function performs a lot of checks to ensure the update is proper and
	 *       successfully performed
	 */
	bool setTableCellValue(const IndexOids &rowIds, const size_t &colId, const Variant &newValue);

	/**
	 * @brief getTableCellValue quick shortcut function to obtain the value of a table cell
	 * @param[in] rowIds ids of the table row containing the cell
	 * @param[in] colId colId column identifier
	 * @param[out] output output value filled if the cell is corretly found
	 * @return true if the value was successfully obtained (and placed into output variable); false otherwise
	 */
	bool getTableCellValue(const IndexOids &rowIds, const size_t &colId, Variant &output);
};

class ParamGroup
{
private:

	string		     name;      /// group name
	string		     filePath;  /// file patch this group will use for saving data
	vector<Param *>  owned;     /// vector of Param (pointers) owned by this group
	DelayedExecution worker;
	bool             activeMode;/// set if the group is active or not (i.e., for saving purposes)
	int              compressLevel;  ///< Compress files when saving

private:

	/**
	 * @brief workerCode thread code: this thread contains a timer and checks if there is activity on this
	 * Param Group. Each new activity increments a ticks counter that's periodically decremented by the thread.
	 * When the counter reaches 0, the thread saves whole group (and it Parameters) on disk.
	 */
	void workerCode();

public:

	/**
	 * @brief ParamGroup empty constructor
	 * @note attribute 'name' will be set to NO_NAME, making this object directly unusable
	 * (assign another ParamGroup to this one or use setParams to change it)
	 */
	ParamGroup();

	/**
	 * @brief ParamGroup constructor
	 * @param[in] groupName name of this group
	 * @param[in] outputFile associated file to write on (if set to NO_NAME, the file name will be automatically
	 * chosen by looking at the group name)
	 */
	ParamGroup(const string &groupName, const string &outputFile = NO_NAME);

	/**
	 * @brief ParamGroup copy constructor
	 * @param[in] other ParamGroup to copy from
	 */
	ParamGroup(const ParamGroup &other);

	/**
	 * @brief operator= assignment operator
	 * @param[in] other other ParamGroup to assign data from
	 * @return a reference to this object
	 */
	ParamGroup &operator=(const ParamGroup &other);

	/**
	 * @brief setParams change name and/or output file for this group
	 * @param[in] groupName name of this group
	 * @param[in] outputFile associated file to write on (if set to NO_NAME, the file name will be automatically
	 * chosen by looking at the group name)
	 */
	void setParams(const string &groupName, const string &outputFile = NO_NAME);

	/**
	 * @brief setActiveMode set a flag to signal if the group is active or not.
	 * @param[in] enabled activate or disactivate this group
	 * @note used by loadFromFile method in ParamManager to signal Params it
	 * is reading data from file and there is no need to immediately re-save
	 * them on file. In other words this flag is used to mark the data reading
	 * phase is in progress.
	 */
	void setActiveMode(bool enabled);

	/**
	 * @brief isActive
	 * @return true if the group is active; false otherwise
	 */
	bool isActive();

	/**
	 * @brief addOwnedParam add a Param to the list of owned parameters
	 * @param[in] toAdd pointer to an owned Param
	 */
	void addOwnedParam(Param *toAdd);

	/**
	 * @brief getName return the name of this Group
	 * @return the name of this Group
	 */
	const string &getName();

	/**
	 * @brief getFilePath return the file path where this Group saves to
	 * @return the file path where this Group saves to
	 */
	const string &getFilePath();

	/**
	 * @brief onChange called when something happened to a Param owned by this Group
	 * @param[in] param Param for which the event was fired
	 * @param[in] message event description/message associate to the event
	 * @param[in] info used to forward more information about the change
	 */
	void onChange(Param *param, const string &message = "", const ExtraInfo *info = NULL);

	/**
	 * @brief save save all owned Params on disk
	 * @return true if saving succeeded; false otherwise
	 */
	bool save();

	/**
	 * @brief setTicksBeforeUpdate set the new ticks waited before committing an update
	 * @param[in] ticks set the new ticks waited before committing an update
	 */
	void setTicksBeforeUpdate(long ticks);

	void setParamSnmpVisibility(bool makeVisible);

	void setParamsSnmpReadOnly(bool forceReadOnly);

	/**
	 * Change compression setting.
	 * Compression is disabled by default.
	 *
	 * @param newLevel The desired compression level. Allowed values:
	 *                 - 0 disables compression
	 *                 - 1-9 enables compression (1 = fastest, 9 = best)
	 */
	void setCompression(int newLevel);
};

} // MPot     NS
} // Comelit  NS

#endif // PARAM_H
