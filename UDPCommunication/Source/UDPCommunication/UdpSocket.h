// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "NetworkProtocolInterface.h"
#include "NetworkProtocolTransform.h"
#include "UdpSocket.generated.h"

/**
 * 
 */
UCLASS()
class UDPCOMMUNICATION_API UUdpSocket : public UObject
{
	GENERATED_BODY()
	
public:
	UUdpSocket();
	UUdpSocket(const FObjectInitializer& ObjectInitializer);

public:

	bool Init(const FString& Ip, int32 Port, INetworkProtocolInterface* Protocol);

	
	void Shutdown();
	bool CreateSender();
	void DeleteSender();
	void Sendto(FTransformData Transdata, FIPv4Endpoint recipient);
	void SetRate(uint32 rate);
private:

	bool CreateSocket();


	void DeleteSocket();


	bool CreateReceiver();

	
	void DeleteReceiver();
	
	
	void OnDataReceived(const FArrayReaderPtr& Reader, const FIPv4Endpoint& Sender);



private:
	FString Ip;
	FIPv4Address IpAddress;
	int32 Port;
	INetworkProtocolInterface* Protocol;

	FSocket* Socket;
	FUdpSocketReceiver* Receiver;
	FUdpSocketSender* Sender;
};
