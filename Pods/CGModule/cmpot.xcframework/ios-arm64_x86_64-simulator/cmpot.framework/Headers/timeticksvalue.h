/**
 *
 * @file timeticksvalue.h
 * Copyright (C) 2013 Comelit Group SpA
 *
 */

#ifndef TIMETICKS_VALUE_H
#define TIMETICKS_VALUE_H

#include <cmpot/common.h>

namespace Comelit  {
namespace MPot     {

/// This class models/mimicks SNMP timeticks
class TimeTicksValue
{
private:

	long cs;

public:

	/**
	 * @brief TimeTicksValue empty constructor
	 */
	TimeTicksValue() : cs(0) { }

	/**
	 * @brief TimeTicksValue constructor
	 * @param[in] ticks long value representing the ticks
	 */
	TimeTicksValue(long ticks) : cs(ticks)  { }

	/**
	 * @brief operator= assignment operator
	 * @param[in] other assigns another TimeTicksValue to this one
	 * @return a reference to the current re-assigned object
	 */
	TimeTicksValue &operator=(const TimeTicksValue &other)
	{
		cs = other.cs;
		return (*this);
	}

	bool operator==(const TimeTicksValue &other) { return (cs == other.cs); }
	bool operator!=(const TimeTicksValue &other) { return (cs != other.cs); }
	bool operator< (const TimeTicksValue &other) { return (cs < other.cs);  }
	bool operator> (const TimeTicksValue &other) { return (cs > other.cs);  }
	bool operator<=(const TimeTicksValue &other) { return (cs <= other.cs); }
	bool operator>=(const TimeTicksValue &other) { return (cs >= other.cs); }

	/**
	 * @brief get return the number of ticks holded by this TimeTicksValue object
	 * @return a long representation of the ticks holded by this TimeTicksValue
	 */
	const long get() const { return cs; }

	/**
	 * @brief centisecs get return the number of ticks holded by this TimeTicksValue object
	 * @return a long representation of the ticks holded by this TimeTicksValue (actually centisecs)
	 */
	const long centisecs() const { return cs; };

	/**
	 * @brief seconds get return the number of seconds holded by this TimeTicksValue object
	 * @return a long representation of the ticks holded by this TimeTicksValue (converted to seconds)
	 */
	const long seconds() const { return cs / 100; };


	/**
	 * @brief set change the ticks value holded by this TimeTicksValue object
	 * @param[in] ticks number of ticks (new value)
	 */
	void set(long ticks) { cs = ticks; }
};

} // MPot     NS
} // Comelit  NS

#endif // TIMETICKS_VALUE_H
