/**
 *
 * @file support.h
 *
 * Ancillary functions (utilities)
 *
 * Copyright (C) 2016 Comelit Group SpA
 *
 */

#ifndef SUPPORT_H_
#define SUPPORT_H_

#include <rapidjson/document.h>

#include <string>

#define NEBULA_FIXED_EPS_FILE_PATH "/tmp/comelit-cloud-eps"

#define NEBULA_FIXED_EPS_URL       "https://eps.cloud.comelitgroup.com/servicerest"
#define NEBULA_FIXED_MQTT_URL      "ssl://mep.cloud.comelitgroup.com:443"
#define NEBULA_FIXED_MQTT_CLUSTER  "ceps"
#define NEBULA_FIXED_MQTT_BROKER   "meps"


namespace Comelit {
namespace Nebula  {

using std::string;

/**
 * This namespace contains some static ancillary/mundane functions
 * used throughout the project
 */
namespace Support {
string getIPAddress();

/**
 * @brief getCloudRetryTime
 * get the cloud retry time for exponential backoff strategy
 * given the number of retries executed
 * @param retry number of retries
 * @return wait time in seconds
 */
int  getCloudRetryTime(uint retry);
}

}
}

#endif // SUPPORT_H_
