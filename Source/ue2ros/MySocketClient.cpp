// Fill out your copyright notice in the Description page of Project Settings.


#include "MySocketClient.h"

#include "MyReceiveThread.h"

UMySocketClient::UMySocketClient()
{
	Server = nullptr;
	m_ReceiveThread = nullptr;
}

UMySocketClient::~UMySocketClient()
{
	ThreadEnd();
}

bool UMySocketClient::CreateSocketClient(FString IP, int32 Port)
{
	FIPv4Address::Parse(IP, ip);
	TSharedRef<FInternetAddr> addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateInternetAddr();
	addr->SetIp(ip.Value);
	addr->SetPort(Port);
	Server = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->CreateSocket(NAME_Stream, TEXT("default"), false);
	if (Server)
	{
		if (Server->Connect(*addr))
		{
			UE_LOG(LogTemp, Warning, TEXT("Connect to server successfully!"));
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Connect to server failed!"));
			return false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Create socket failed!"));
		return false;
	}
}

bool UMySocketClient::SendData(FString message)
{
	if (Server)
	{
		TCHAR* serializedChar = message.GetCharArray().GetData();
		int32 size = FCString::Strlen(serializedChar);
		int32 sent = 0;
		Server->Send((uint8*)TCHAR_TO_UTF8(serializedChar), size, sent);
		return true;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Send data failed!"));
		return false;
	}
}

bool UMySocketClient::ReceiveData()
{
	m_ReceiveThread = FRunnableThread::Create(new MyReceiveThread(this), TEXT("MyReceiveThread"));
	return false;
}

bool UMySocketClient::ThreadEnd()
{
	if (m_ReceiveThread)
	{
		m_ReceiveThread->Kill(false);
		delete m_ReceiveThread;
		// m_ReceiveThread = nullptr;
	}
	// if (Server)
	// {
		// Server->Close();
		// ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->DestroySocket(Server);
		// Server = nullptr;
	// }
	return false;
}
