// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Sockets/Public/Sockets.h"
#include "Networking/Public/Networking.h"
#include "MySocketClient.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_DELEGATE_OneParam(FOnReceiveSurrogateModelDataDelegate, FString, Data);

UCLASS(BlueprintType)
class UE2ROS_API UMySocketClient : public UObject
{
	GENERATED_BODY()
public:
	UMySocketClient();
	~UMySocketClient();
	bool CreateSocketClient(FString IP, int32 Port);
	bool SendData(FString message);
	bool ReceiveData();
	bool ThreadEnd();
public:
	FSocket* Server;
	FIPv4Address ip;
	FRunnableThread* m_ReceiveThread;
	FOnReceiveSurrogateModelDataDelegate OnReceiveSurrogateModelData;
};
