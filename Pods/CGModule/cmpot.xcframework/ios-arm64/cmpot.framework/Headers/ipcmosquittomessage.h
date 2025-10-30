/**
 *
 * @file ipcmosquittomessage.h
 *
 *
 * Copyright (C) 2017 Comelit Group SpA
 *
 */

#ifndef IPCMOSQUITTOMESSAGE_H
#define IPCMOSQUITTOMESSAGE_H

#include <mosquittopp.h>

namespace Comelit  {
namespace MPot     {
namespace IPC	   {

/**
 * The IPCMosquittoMessage class is an Internal class to manage a mosquitto broker message
 */
class IPCMosquittoMessage
{
public:
	IPCMosquittoMessage(const mosquitto_message *msg);
	~IPCMosquittoMessage();
	const mosquitto_message *getMessage(void);
private:
	mosquitto_message message;
};

} // namespace IPC
} // namespace MPot
} // namespace Comelit

#endif // IPCMOSQUITTOMESSAGE_H
