/**
 * @file servicelookupmanager.h
 *
 * Contains the class EpsServiceLookupManager.
 *
 * Containts a collection of EpsServiceLookup.
 *
 * In order to keep valid the EPS, just invoke timer() periodically (e.g. by a thread).
 * Otherwise synchronization must be performed by the caller.
 */

#ifndef SERVICELOOKUPMANAGER_H
#define SERVICELOOKUPMANAGER_H

#include <string>
#include <map>
#include <chrono>

#include <cloud/cloudrequest.h>

namespace Comelit {
namespace Nebula  {

using std::pair;
using std::string;
using std::map;

using Comelit::Nebula::CloudAnswer;
using Comelit::Nebula::CloudRequest;

typedef std::function<void (string service, string version, string eps, time_t expireTime)>
OnEpsChangeEvent;

class EpsServiceLookup : public ICloudRequestHandler
{
public:

	typedef enum
	{
		EPS_OK,
		EPS_EXPIRED,
		EPS_NOTFOUND
	} EpsStatus;
	
public:

	EpsServiceLookup(string name, string version);
	
	string getSrvName();
	string getSrvVersion();
	string getEps() { return eps; }
	EpsStatus getEpsStatus();
	
	/**
	 * Get updated eps from cloud
	 * @param[in] token valid token to be used for cloud authentication
	 * @return true if eps correctly update
	 */
	bool   refreshEps(string token);
	
	/**
	 * Run function
	 * Require a valid token to optionally perform an
	 * eps refresh
	 * @param[in] token
	 */
	void   run(string token);
	
	/**
	 * Set valid eps data
	 * @param eps
	 * @param epsExpireTime
	 */
	void   setEpsData(string eps, time_t epsExpireTime);
	
	void   setOnEpsChangeEvent(OnEpsChangeEvent cbk) { onEpsChangeEvent = cbk;}
		
	/**
	 * Process cloud "eps/slk" response
	 * Process cloud "eps/slk" response and act accordingly
	 * @param[in] cresponse cloud response
	 */
	void   processSLKResponse(CloudAnswer *response);
	
	/// Get the ongoing cloud request.
	void  *getCurrentCloudRequest() { return curCloudRequest; }
	
	/// Enable eps update on ttl expiration
	void setAutoUpdate(bool enable) { update = enable; }
	
private:

	typedef enum
	{
		IDLE,
		EPS_REFRESH,
		EPS_REFRESHING
	} Status;
	
	string srvName;
	string srvVersion;
	string eps;
	int	   epsTtl;
	time_t epsExpireTime; ///< expiration time (datetime utc)
	OnEpsChangeEvent onEpsChangeEvent;
	Status              status;
	uint                retry;
	void               *curCloudRequest;
	bool   update; ///< true if eps sync on expiration is enabled
	
private:

	bool getEpsFromSLKService(string token, string &error);
	/**
	 * Get remaining time in seconds before token expiration
	 * @return 0 if token is already expired
	 */
	double getRemainingTime();
};

typedef map<pair<string, string>, EpsServiceLookup> EpsServiceLookupMap;
typedef std::function<void (EpsServiceLookup &svc)> OnServiceAdd;

class EpsServiceLookupManager
{
public:

	EpsServiceLookupManager();
	
	/**
	 * returns the eps retrieved via slk service
	 * if eps is not registered an EpsServiceLookup is created
	 * and requested to server
	 *
	 * @param[in]  name
	 * @param[in]  version
	 * @param[out] err error code (0 ok, 1 expired, 2 fixed eps)
	 *
	 * @return resolved eps
	 */
	string resolveEps(string name, string version, int &err);
	
	/**
	 * @brief removeService remove service lookup from map
	 * @param name
	 * @param version
	 * @return true if found, false if not found
	 */
	bool removeService(string name, string version);
	
	/**
	 * Invalidate all services, forcing a new eps request on the next run call
	 */
	void invalidateAllServices();
	
	/**
	 * @brief invalidateService
	 * Invalidate a single service, forcing a new eps request on the next run call
	 * @param name service name
	 * @param version service version, in case version is left empty,
	 * all services with the same name will be invalidated
	 */
	void invalidateService(const string &name, const string &version);

	/**
	 * Run main cycle, this function must be continously called to let eps engine work
	 * maintaining the eps services updated on Comelit Cloud and periodically
	 * obtaining a new eps to be used for Cloud service operations
	 */
	void run(string token);
	
	static string getFixedEps();
	
	/**
	 * Add an EpsService to the list
	 * optionally providing initialization data.
	 * Note that this function doesn't call a refresh eps, even if not available or expired.
	 * @param name
	 * @param version
	 * @param update
	 * @param epsValue
	 * @param expireTime
	 */
	void   addService(string name, string version, bool update = true, string epsValue = "",
	                  time_t expireTime = 0);
	                  
	void   setOnServiceAdd(OnServiceAdd cbk) { onServiceAdd = cbk;}
	
private:

	EpsServiceLookupMap services;
	OnServiceAdd        onServiceAdd; ///< to be called on service add
};

}
}
#endif // SERVICELOOKUPMANAGER_H
