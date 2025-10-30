/**
 *
 * @file cloudlogmanager.h
 *
 * Manage Cloud Logging Data
 * throught an ipcmanager instance and nebulad
 * daemon running in the system
 *
 * Copyright (C) 2016 Comelit Group SpA
 *
 */

#ifndef CLOUDLOGMANAGER_H
#define CLOUDLOGMANAGER_H

#include <cmpot/common.h>
#include <cmpot/ipcmanager.h>
#include <cmpot/powermanager.h>
#include <mutex>

namespace Comelit  {
namespace MPot     {
namespace IPC      {

class CloudLogManager : public PowerManagerImpl
{
public:
	CloudLogManager(IPCManager *ipcServer);

	/**
	 * Timer function to be called periodically
	 */
	void    timer();

	bool    addMessage(const std::string &msg); ///< add message to the pending queue

private:
	/**
	 * Try to resend the first pending message to the cloud
	 * @retval -1 trylock failed
	 * @retval  N number of pending messages still on queue
	 */
	int     processFirstPendingMessage();
	int     getQueueSize();

private:
	IPCManager  *ipcServer;
	bool         suspended; ///< true in suspension mode (no actions can be performed)
	list<string> pendingMessages; ///< Queue of pending messages to be sent to the cloud
	std::mutex   pendingMsgMutex; ///< Protect access on pending messages list
	std::mutex   sendingMsgMutex; ///< Protect access on message sending
	time_t       updateTime; ///< update time flag, time update once this date is reached
	int          cloudErrors; ///< number of errors since the last successfull request

	// PowerManagerImpl interface
public:
	bool suspend(PowerManager *pm);
	void resume(PowerManager *pm);
};

}
}
}

#endif // CLOUDLOGMANAGER_H
