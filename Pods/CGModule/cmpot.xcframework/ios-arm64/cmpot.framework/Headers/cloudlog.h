/**
 *
 * @file cloudlog.h
 *
 * Manage Cloud Logging Events
 * create cloud log struct to be sent
 * as a string to a CloudLogManager instance
 *
 * Copyright (C) 2016 Comelit Group SpA
 *
 */

#ifndef CLOUDLOG_H
#define CLOUDLOG_H

#include <cmpot/common.h>
#include <cmpot/jsonutil.h>
#include <mutex>

namespace Comelit  {
namespace MPot     {
namespace IPC      {

class CloudLog
{
public:
	struct DeviceInfo
	{
		DeviceInfo()
		{
			this->fwVers      = "";
			this->modelId     = "";
			this->description = "";
			this->serial        = "";
		}

		DeviceInfo(const string &fwVers, const string &modelId, const string &description, const string &sn)
		{
			this->fwVers      = fwVers;
			this->modelId     = modelId;
			this->description = description;
			this->serial         = sn;
		}

		string fwVers;      ///< Logger firmware version
		string modelId;     ///< modelId
		string description; ///< description
		string serial;         ///< unique identifier
	};

	struct LogEvent
	{
		LogEvent() = delete;

		LogEvent(const string &time, const string &action, const string &message):
			time(time), action(action), message(message), params( {})
		{
		}

		LogEvent(const string &time, const string &action, const string &message,
		         const map<string, string> &params):
			time(time), action(action), message(message), params(params)
		{
		}

		string time; ///< event time of day in milliseconds
		string action;
		string message;
		map<string, string> params;
	};

	typedef enum
	{
		CDEBUG,
		CINFO,
		CWARN,
		CERROR
	} LogLevel;

public:
	CloudLog();
	CloudLog(const CloudLog &);

	void    setDeviceInfo(const DeviceInfo &info) { deviceInfo = info; }
	void    setLogVersion(const string &version) { logVersion = version; }

	/**
	 * Reset current message
	 */
	void    reset();

	/**
	 * @brief Add a telemetry event to the queue
	 * @param evt
	 */
	void     addEvent(const LogEvent &evt);

	/**
	 * Add json element to log message
	 * For example device identification or various stats
	 * @param key name of the json element
	 * @param obj object, can be an array or an object
	 */
	void     addElement(const string &key, const rapidjson::Value &obj);

	/**
	 * Add json element to log message
	 * Add a simple key-value element to the json log
	 * @param name name of the json element
	 * @param value string value
	 */
	void     addElement(const string &key, const string &value);

	/**
	 * Send collected log to the cloud
	 * After send reset is called automatically to reset message
	 * @param[in] tag log tag for the cloud
	 * @param[in] log level for the cloud
	 * @param[in] event id for cloud CCAPI
	 * @return    string to be sent to cloud ipc service
	 */
	string     getLogString(const string &tag, LogLevel lvl = CDEBUG, const string &eventID = "");

private:
	std::mutex          logMtx;       ///< Protect access

	map<string, LogEvent>      logQueue;
	rapidjson::Document message; ///< Message to send to the log cloud service
	rapidjson::Document body;    ///< Main Body of request
	DeviceInfo          deviceInfo;
	string              logVersion; ///< internal log versioning
};

}
}
}

#endif // CLOUDLOG_H
