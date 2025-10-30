/**
 *
 * @file tabledef.h
 * Copyright (C) 2013 Comelit Group SpA
 *
 */

#ifndef TABLEDEF_H
#define TABLEDEF_H

#include <cassert>
#include <cstring>
#include <map>
#include <thread>
#include <vector>

#ifndef aSN_UNIVERSAL
// These are defined in snmp++'s snmp_pp/integer.h, but it is an optional
// dependency, so we cannot include it here. Since we only use a bunch of
// defines, let's just redefine them.
#define aSN_UNIVERSAL         (0x00)
#define aSN_APPLICATION       (0x40)
#define aSN_PRIMITIVE         (0x00)
#define sNMP_SYNTAX_INT       (aSN_UNIVERSAL | aSN_PRIMITIVE | 0x02)
#define sNMP_SYNTAX_OCTETS    (aSN_UNIVERSAL | aSN_PRIMITIVE | 0x04)
#define sNMP_SYNTAX_INT32     sNMP_SYNTAX_INT
#define sNMP_SYNTAX_IPADDR    (aSN_APPLICATION | aSN_PRIMITIVE | 0x00)
#define sNMP_SYNTAX_TIMETICKS (aSN_APPLICATION | aSN_PRIMITIVE | 0x03)
#endif

#include <cmpot/timeticksvalue.h>
#include <cmpot/common.h>

#define MAX_INDEXES		8
#define NO_RANGE_LIMIT     0xDEADBEEF
#define CMPOT_SYNTAX_LOGADDRMAIN 0x98
#define CMPOT_SYNTAX_TABLE       0x99
#define UNLIMITED_IDXS		 -1
#define UNLIMITED_ROWS		 -1

#define EMPTY_IPADDR      string("0.0.0.0")
#define EMPTY_STRING      string("")
#define EMPTY_TABLE       Table(NULL)

namespace Comelit  {
namespace MPot     {

using std::bind;
using std::make_pair;
using std::thread;
using std::map;
using std::vector;

using namespace Comelit::MPot::Common;

/// Forward declaration
class Table;
class ExtraInfo;

enum class RowEvent
{
	UNKNOWN,
	ROW_CREATED,
	ROW_DELETED,
	ROW_ACTIVATED,
	ROW_DEACTIVATED,
	DATA_CHANGED
};

/// Available SNMP types (remapped from SNMP++/AGENT++ ones plus an addition for tables)
enum ValueType
{
	VT_INTEGER     = sNMP_SYNTAX_INT32,
	VT_TIMETICKS   = sNMP_SYNTAX_TIMETICKS,
	VT_STRING      = sNMP_SYNTAX_OCTETS,
	VT_IPADDR      = sNMP_SYNTAX_IPADDR,
	VT_LOGADDRMAIN = CMPOT_SYNTAX_LOGADDRMAIN,
	VT_TABLE       = CMPOT_SYNTAX_TABLE
};

class Param;
class Variant;

typedef function<bool (const Variant &value,
                       const ExtraInfo *extra)>  ParamValidator;
typedef function<bool (Variant &value,
                       const ExtraInfo *extra)>  ParamFiller;
typedef function<void (const Variant   &value,
                       const ExtraInfo *extra)>  ParamPostSet;
typedef vector<ParamPostSet>                     ParamPostSets;

/// This class represent the type of a table column ("field")
class FieldType
{
public:

	ValueType      type;
	unsigned int   min;
	unsigned int   max;
	ParamValidator validator;
	
public:

	/**
	 * @brief FieldType empty constructor (needed by unorderd_map internals)
	 */
	FieldType();
	
	/**
	 * @brief FieldType standard constructor
	 * @param[in] snmpType define the type for this field
	 * @param[in] minValue min value for the field (for numeric types)
	 * @param[in] maxValue max value for the field (for numeric types)
	 */
	FieldType(ValueType snmpType,
	          unsigned int minValue = NO_RANGE_LIMIT,
	          unsigned int maxValue = NO_RANGE_LIMIT);
	          
	/**
		 * @brief FieldType standard constructor
		 * @param[in] snmpType define the type for this field
		 * @param[in] newValidator assign a validation function (validator) for this field
		 * @param[in] minValue min value for the field (for numeric types)
		 * @param[in] maxValue max value for the field (for numeric types)
		 */
	FieldType(ValueType snmpType,
	          ParamValidator &&newValidator,
	          unsigned int minValue = NO_RANGE_LIMIT,
	          unsigned int maxValue = NO_RANGE_LIMIT);
	          
	/**
	 * @brief hasRange return if this field has a range or not
	 * @return true if the field has an associated range; false otherwise
	 */
	bool hasRange();
	
	/**
	 * @brief getType return the type associated to this field
	 * @return return the type associated to this field
	 */
	const ValueType &getType();
};

/// Defines the available types for the Variant class
enum class VariantType
{
	UNDEFINED, LONG, TIMETICKS, LOGADDRMAIN, STRING, TABLEDEF
};

/// Defines and interface for objects wanting to be informed when something
/// happens to a a Variant object ("listeners")
struct IChangeListener
{
	virtual bool fill(Variant &value, const ExtraInfo *change = NULL) = 0;
	virtual bool validate(const Variant &val, const ExtraInfo *change = NULL) = 0;
	virtual void onChange(const string &message = "", const ExtraInfo *change = NULL) = 0;
};

/// Variant class allowing to store different types (one per time)
class Variant
{
public:

	/**
	 * @brief Variant empty constructor
	 */
	Variant();
	
	/**
	 * @brief Variant constructor for integer values
	 * @param[in] value integer value to be assigned to this Variant object
	 */
	Variant(int value);
	
	/**
	 * @brief Variant constructor for long values
	 * @param[in] value long value to be assigned to this Variant object
	 */
	Variant(long value);
	
	/**
	 * @brief Variant constructor for TimeTicksValue values
	 * @param[in] value TimeTicksValue value to be assigned to this Variant object
	 */
	Variant(TimeTicksValue value); ///< Crea un valore TimeTicks
	
	/**
	 * @brief Variant constructor for CSPStack's logaddr_main_t values
	 * @param[in] value CSPStack's logaddr_main_t value to be assigned to this Variant object
	 */
	Variant(const logaddr_main_t value); ///< Crea un logaddr_main

	
	/**
	 * @brief Variant constructor for string values
	 * @param[in] value string value to be assigned to this Variant object
	 */
	Variant(const string &value);

	/**
	 * @brief Variant constructor for string rvalues
	 * @param value[in] value string value to be moved to this Variant object
	 */
	Variant(string &&value);

	/**
	 * @brief Variant constructor for C string values
	 * @param[in] value C string value to be assigned to this Variant object
	 */
	Variant(const char *value);
	
	/**
	 * @brief Variant constructor for Table values
	 * @param[in] value Table value to be assigned to this Variant object
	 */
	Variant(const Table &value);
	
	/**
	 * @brief Variant copy constructor for Variant values
	 * @param[in] value Variant value to be copied/assigned to this Variant object
	 */
	Variant(const Variant &value);

	/**
	 * @brief Variant move constructor
	 * @param other
	 */
	Variant(Variant &&other);

	
	/**
	 * Destructor
	 */
	~Variant();
	
	/**
	 * @brief operator= assignment operator
	 * @param[in] other Variant value to be copied to this Variant object
	 * @return a reference to this object
	 */
	Variant &operator=(const Variant &other);

	Variant &operator=(Variant &&other);

	
	/**
	 * @brief operator== comparison operator
	 * @param[in] other Variant value this object will be compared to
	 * @return true if the two objects are the same/contain the same value; false otherwise
	 */
	bool operator==(const Variant &other);
	
	/**
	 * @brief getLong return the long value actually contained in this object
	 * @return the long value contained in this object
	 */
	const long &getLong() const;
	
	/**
	 * @brief getSnmpBool return an SNMP-encoded boolean stored in a long
	 * @return true if the value is 1, false if it is 2, unspecified otherwise
	 */
	const bool getSnmpBool() const;
	
	/**
	 * @brief getTimeTicks return the TimeTicksValue value actually contained in this object
	 * @return the TimeTicksValue value contained in this object in its long integer form
	 */
	const TimeTicksValue getTimeTicks() const;
	
	const logaddr_main_t &getLogaddrMain() const;
	
	/**
	 * @brief toLogaddrMain return the CSPStack's logaddr_main_t value actually contained in this object
	 * @return the logaddr_main_t value contained in this object
	 */
	void toLogaddrMain(logaddr_main_t logMain) const;
	
	/**
	 * @brief getIPAddress return the IP address value actually contained in this object
	 * @return the IP address (string) value contained in this object
	 */
	const string &getIPAddress() const;
	
	/**
	 * @brief getString return the string value actually contained in this object
	 * @return the IP address (string) value contained in this object
	 */
	const string &getString() const;
	
	/**
	 * @brief getTable return the table value actually contained in this object
	 * @return the table value contained in this object
	 */
	const Table &getTable() const;
	
	/**
	 * @brief getTable return a pointer to the table value actually contained in this object
	 * @return a pointer to the table value contained in this object
	 */
	Table *getMutableTable();
	
	/**
	 * @brief toString return a string representation for the the value actually contained
	 * @return a string representation for the the value actually contained
	 */
	string toString() const;
	
	/**
	 * @brief setListener define/save the listener to be informed when something happens to this object
	 * @param[in] interested pointer to the listener to be saved
	 */
	void setListener(IChangeListener *interested);
	
	bool callValidator(const Variant &value, const ExtraInfo *info = NULL);
	
	bool callFiller(const ExtraInfo *info = NULL);
	
	/**
	 * @brief notifyListener inform the listener that something happended
	 * @param[in] message description/message associated to the event (default: empty string)
	 * @param[in] info extra information object (if available)
	 * @note currently the ExtraInfo object is only used to provide some details about a table row change
	 */
	void notifyListener(string message = "", ExtraInfo *info = NULL);
	
	const VariantType getType() const { return type; }
	
private:

	VariantType       type;
	IChangeListener  *listener;
	
	union /// union contained the allowed types
	{
		long		    num;
		logaddr_main_t  lam;
		string		 *str;
		Table		  *tbl;
	} val;
	
private:

	/**
	 * @brief deleteInternalStorage invoked to clean up memory for complex/pointer-based
	 * object (string and Table)
	 */
	void deleteInternalStorage();
};

// Some practical typedefs
typedef vector<unsigned long>        IndexOids;
typedef map<unsigned int, FieldType> FieldsMap;
typedef map<unsigned int, Variant>   TableRow;
typedef map<IndexOids, TableRow>     TableRows;

typedef function<void (const IndexOids &idx, TableRow &row)> RowAction;

/// This class repsent the structure ("definition") for a table
class TableDef
{
private:

	IndexOids    indexOids;
	unsigned int rowStatusCol;
	unsigned int totalCols;
	FieldsMap    fields;
	int          numOfIdxPerRow;
	long         maxRows;
	
public:

	/**
	 * @brief TableDef empty constructor
	 */
	TableDef();
	
	/**
	 * @brief TableDef constructor
	 * @param[in] tableIndexes vector of index associated to this table
	 * @param[in] rowStatus id of the column representing the row status
	 * @param[in] idxPerRow number of indexes required for each row (default: UNLIMITED_IDXS)
	 * @param[in] maxTableRows maximum number of rows (default: UNLIMITED_ROWS)
	 * different and unlimited number of indexes)
	 */
	TableDef(IndexOids tableIndexes, unsigned int rowStatus, int idxPerRow = UNLIMITED_IDXS,
	         long maxTableRows = UNLIMITED_ROWS);
	         
	/**
	 * @brief TableDef constructor
	 * @param[in] tableIndexes vector of index associated to this table
	 * @param[in] rowStatus id of the column representing the row status
	 * @param[in] fieldsMap map of field structures representing and defining the columns
	 * @param[in] idxPerRow number of indexes required for each row (default: UNLIMITED_IDXS)
	 * @param[in] maxTableRows maximum number of rows (default: UNLIMITED_ROWS)
	 * different and unlimited number of indexes)
	 */
	TableDef(IndexOids tableIndexes, unsigned int rowStatus, FieldsMap &fieldsMap,
	         int idxPerRow = UNLIMITED_IDXS, long maxTableRows = UNLIMITED_ROWS);
	         
	/**
	 * @brief getIndexOids return the index Oids for this table
	 * @return a vector of unsigned longs representing the Oids identifying this table
	 */
	const IndexOids &getIndexOids() const { return indexOids; }
	
	/**
	 * @brief getRowStatusCol return the id of the column used to maintain the row status
	 * @return the id of the rows status column
	 */
	const unsigned int &getRowStatusCol() const { return rowStatusCol; }
	
	/**
	 * @brief getTotalCols return the total number of columns for the row (including special ones)
	 * @return the number of columns
	 */
	const unsigned int &getTotalCols() const { return totalCols; }
	
	/**
	 * @brief getFields return the map of fields (that is columns)
	 * @return the map of the fields (columns) for this table
	 */
	const FieldsMap &getFields() const { return fields; }
	
	/**
	 * @brief getNumOfIdxPerRow return the number of indexes required for each new row
	 * @return the number of indexes required for each new row
	 */
	const int  &getNumOfIdxPerRow()  const { return numOfIdxPerRow; }
	
	/**
	 * @brief getField return the type associated to the input field id
	 * @param[in] colId id of the field
	 * @return field type associated to the input field
	 */
	FieldType &getField(unsigned int colId) { return fields[colId]; }
	
	/**
	 * @brief setMaxRows set the maximum number of rows in this table
	 * @param[in] newMaxRows the new maximum number of rows in this table
	 */
	void setMaxRows(long newMaxRows) { maxRows = newMaxRows; }
	
	/**
	 * @brief getMaxRows return the maximum number of rows in this table
	 * @return the maximum number of rows in this table
	 */
	long getMaxRows() { return maxRows; }
};

enum class FieldChangePolicy
{
	ALWAYS_EDITABLE,
	READONLY_WHEN_ROWACTIVE
};

/// This class represent a full table, made of a TableDef object and Tablerow objects
class Table
{
public:

	FieldChangePolicy fePolicy;
	TableDef          *def;
	TableRows          rows;
	
public:

	/**
	 * @brief Table empty constructor
	 */
	Table();
	
	/**
	 * @brief Table constructor
	 * @param[in] tableDef table definition (structure) used as a base for creating and operating on table rows
	 */
	Table(TableDef *tableDef);
	
	/**
	 * @brief addOrUpdateRow add or update a row and return it
	 * @param[in] idxs indexes used to identify the row we are operating on
	 * @return a reference to a row, newly created or already existent
	 *
	 * @note if the inner TableDef has a valid maxRows set (default: not, unlimited),
	 *       this function will return INVALID_TABLE_ROW
	 */
	TableRow &addOrUpdateRow(const IndexOids &idx);

	/**
	 * @brief addRow add a row and return it
	 * @param[in] idxs indexes used to identify the row we are operating on
	 * @return a reference to a row, newly created
	 *
	 * @note if the inner TableDef has a valid maxRows set (default: not, unlimited),
	 *       this function will return INVALID_TABLE_ROW
	 */
	TableRow &addRow(const IndexOids &idxs);
	
	/**
	 * @brief addOrUpdateRow add or update a row and return it
	 * @param[in] idxs indexes used to identify the row we are operating on
	 * @param[out] if non-null, true on success; false on error
	 * @return a reference to a row, newly created or already existent
	 *
	 * @note if the inner TableDef has a valid maxRows set (default: not, unlimited),
	 *       this function will return INVALID_TABLE_ROW
	 */
	TableRow &addOrUpdateRow(const IndexOids &idxs, bool *success);
	
	/**
	 * @brief addOrUpdateRow add or update a row and return it
	 * @param[in] idxs indexes used to identify the row we are operating on
	 * @param[out] output reference to fill on success
	 * @return true on success; false othrwise
	 */
	bool addOrUpdateRow(const IndexOids &idxs, TableRow &output);
	
	/**
	 * @brief deleteRow delete a row
	 * @param[in] idxs  indexes used to identify the row we want to delete
	 * @return true if deletion successfully happened; false otherwise
	 */
	bool deleteRow(const IndexOids &idxs);
	
	/**
	 * @brief operator[] subscript operator used to directly access a row by its indexes
	 * @param[in] idxs indexes used to identify the row
	 * @return the row associated to the input indexes
	 */
	TableRow &operator[](IndexOids idxs);
	
	const TableRow &operator[](IndexOids idxs) const;
	
	/**
	 * @brief setCellValue set the value for a cell
	 * @param[in] rowIds identifies the row containing the cell
	 * @param[in] colId identifies the column for the cell
	 * @param[in] newValue the new long value to write into the cell
	 * @return true if all operation succeeded; false otherwise
	 */
	bool setCellValue(const IndexOids &rowIds, const size_t &colId, const long &newValue);
	
	/**
	 * @brief setCellValue set the value for a cell
	 * @param[in] rowIds identifies the row containing the cell
	 * @param[in] colId identifies the column for the cell
	 * @param[in] newValue the new string value to write into the cell
	 * @return true if all operation succeeded; false otherwise
	 */
	bool setCellValue(const IndexOids &rowIds, const size_t &colId, const string &newValue);
	
	/**
	 * @brief setCellValue set the value for a cell
	 * @param[in] rowIds identifies the row containing the cell
	 * @param[in] colId identifies the column for the cell
	 * @param[in] newValue the new logaddr_main_t value to write into the cell
	 * @return true if all operation succeeded; false otherwise
	 */
	bool setCellValue(const IndexOids &rowIds, const size_t &colId, const logaddr_main_t &newValue);
	
	/**
	 * @brief getDef return a pointer to the table definition
	 * @return a pointer to the table definition
	 */
	TableDef *getDef() { return def; }
	
	/**
	 * @brief getNextAvailableRowIdx return a valid array of indexes for a new row
	 * @return an array of indexes for a new row
	 */
	IndexOids getNextAvailableRowIdx();
	
	IndexOids getNextAvailableRowIdx(const IndexOids &input);
	
	/**
	 * @brief getRow look for a row and if it exists, fill the output parameter
	 * @param[in] idxs indexeas
	 * @param[out] outputRow output parameters (if the TableRow exists)
	 * @return true if the TableRow exists; false otherwise
	 */
	TableRow getRow(IndexOids idxs) const;
	
	/**
	 * @brief getRow look for a row and if it exists, fill the output parameter
	 * @param[in] idxs indexeas
	 * @param[out] outputRow output parameters (if the TableRow exists)
	 * @return true if the TableRow exists; false otherwise
	 */
	bool getRow(IndexOids idxs, TableRow &outputRow);
	
	/**
	 * @brief findRowWithMatch try to find a row containing the given value in a given field
	 * @param[in] fieldId row field column to be inspected for finding the input value
	 * @param[in] val input value to be looked for
	 * @param[out] outputRow reference used to assign the output (if any)
	 * @return true if a row with the given value in the given field was found; false otherwise
	 *
	 * @note found row is a copy!
	 */
	bool findRowWithMatch(const unsigned int &fieldId, const Variant &val, TableRow &outputRow);
	
	/**
	 * @brief findRowWithMatch try to find a row containing the given value in a given field
	 * @param[in] fieldId row field column to be inspected for finding the input value
	 * @param[in] val input value to be looked for
	 * @param[out] outputIds reference used to assign the oid of the first matching row
	 * @return true if a row with the given value in the given field was found; false otherwise
	 */
	bool findRowWithMatch(const unsigned int &fieldId, const Variant &val, IndexOids &outputIds);
	
	/**
	 * @brief setFieldChangePolicy define the field-change policy for the table
	 * @param[in] newPolicy set the field-change policy
	 */
	void setFieldChangePolicy(FieldChangePolicy newPolicy) { fePolicy = newPolicy; }
	
	/**
	 * @brief getFieldChangePolicy get the field change policy
	 * @return the current field change policy for this table
	 */
	FieldChangePolicy getFieldChangePolicy() { return fePolicy; }
	
	/**
	 * @brief forEachRow iterate on all available rows and perform an action on each of them
	 * @param[in] action RowAction function to be performed on each row
	 */
	void forEachRow(const RowAction &&action);
	
	/**
	 * @brief clear clear the table by safely removing all rows
	 */
	void clear();
	
private:

	/**
	 * @brief addRow add a new row to the table
	 * @param[in] idxs indexes used to identify the row we are operating on
	 * @param[out] if non-null, true on success; false on error
	 * @return a reference to a row, newly created or already existent
	 *
	 * @note if the inner TableDef has a valid maxRows set (default: not, unlimited),
	 *       this function will return INVALID_TABLE_ROW
	 */
	TableRow &addRow(const IndexOids &idxs, bool *success);
};

/// Extra Information class: used to provide more/additional info in PostSetCallbacks
/// @note: currently used only to provide extended info for Table objects
class ExtraInfo
{
public:

	IndexOids     rowIds;
	RowEvent      rowEvent;
	unsigned long colChangedId;
	
public:

	// Constructor meant to provide info about rowStatus-related events
	ExtraInfo(IndexOids ids, RowEvent newRowEvent = RowEvent::UNKNOWN)
		: rowIds(ids), rowEvent(newRowEvent), colChangedId(-1)
	{
		assert(newRowEvent != RowEvent::DATA_CHANGED); // use the other ctor!
	}
	
	// Constructor meant to provide info about data-related events
	ExtraInfo(IndexOids ids, unsigned long colId)
		: rowIds(ids), rowEvent(RowEvent::DATA_CHANGED), colChangedId(colId)
	{ }
};

} // MPot     NS
} // Comelit  NS

#endif // TABLEDEF_H
