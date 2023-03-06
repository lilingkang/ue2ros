#include "MyReceiveThread.h"

MyReceiveThread::~MyReceiveThread()
{
	m_bStop = true;
}

bool MyReceiveThread::Init()
{
	m_bStop = false;
	return true;
}

uint32 MyReceiveThread::Run()
{
	if (!m_Client->Server)
    {
       	return 0;
    }
	TArray<uint8> m_Data;
	//接收数据包
	while (!m_bStop && m_Client->Server->GetConnectionState() == ESocketConnectionState::SCS_Connected)   //线程计数器控制
	{
		m_Data.Init(0, 500*1024u);
		int32 read = 0;
		m_Client->Server->Recv(m_Data.GetData(), m_Data.Num(), read);
		if (read > 0)
		{
			m_Data.SetNum(read);
			FString ReceivedUE4String = FString(UTF8_TO_TCHAR(m_Data.GetData()));
			
			// UE_LOG(LogTemp, Warning, TEXT("ReceivedUE4String*** %s"), *ReceivedUE4String);
		
			m_Client->OnReceiveSurrogateModelData.ExecuteIfBound(ReceivedUE4String);
			
			// UE_LOG(LogTemp, Warning, TEXT("ReceivedUE4String*** %d"), read);
		}
		m_Data.Empty();
		FPlatformProcess::Sleep(0.01f);
	}

	return 1;
}

void MyReceiveThread::Stop()
{
	m_bStop = true;
}
