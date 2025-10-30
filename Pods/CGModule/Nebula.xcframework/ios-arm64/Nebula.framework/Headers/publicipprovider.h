#ifndef PUBLICIPPROVIDER_H
#define PUBLICIPPROVIDER_H

#include <cloud/cloudasyncrequestmanager.h>
#include <comelitutils/exponentialbackoff.h>

namespace Comelit {
namespace Nebula  {

/**
 * @brief The PublicIpProvider class
 * is responsable to Retrieve device Public Ip via HTTP request
 * the public Ip is only retieved once, at startup
 * once IP is available, it does not need to be refreshed
 */
class PublicIpProvider
{
public:
	PublicIpProvider();
	~PublicIpProvider();

	/**
	 * @brief run
	 * this method must be called periodically, in order to
	 * allow the PublicIpProvider to work.
	 */
	void run();

	/**
	 * @brief getpublicIp
	 * retrieve the Device Public IP retrieved via
	 * http://checkip.comelitdns.com
	 * @return
	 * The Public IP if a valid IP was retrieved
	 * empty string otherwise
	 */
	const std::string &getpublicIp();
private:

	/**
	 * @brief requestPublicIp
	 * make the HTTP request to retrieve device Public IP
	 * @return
	 */
	bool requestPublicIp();

	/**
	 * @brief processPublicIpResponse
	 * handle the HTTP answer to Public IP request.
	 * @param answer
	 */
	void processPublicIpResponse(HTTPAnswer *answer);

private:
	Comelit::Utils::ExponentialBackoff expBackoff;
	std::mutex requestLock;
	bool requestInProgress;

	std::string publicIp;
};

/**
 * @brief publicIpProvider
 * the global instance of PublicIpProvider
 */
extern PublicIpProvider *publicIpProvider;

} //Comelit
} //Nebula

#endif //PUBLICIPPROVIDER_H
