#pragma once

#include "CoreMinimal.h"
#include "Core/Public/HAL/Runnable.h"
#include "Sockets/Public/Sockets.h"
#include "MySocketClient.h"

class UE2ROS_API MyReceiveThread : public FRunnable
{
public:
	MyReceiveThread(UMySocketClient* Client):m_Client(Client),m_bStop(false) {}
	~MyReceiveThread();
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;
private:
	UMySocketClient* m_Client;
	bool m_bStop;
	FThreadSafeCounter m_StopTaskCounter;
};
