// Fill out your copyright notice in the Description page of Project Settings.

#include "UDPCommunication.h"
#include "UdpSocket.h"

UUdpSocket::UUdpSocket()
{
}

UUdpSocket::UUdpSocket(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

bool UUdpSocket::Init(const FString& InIp, int32 InPort, INetworkProtocolInterface* InProtocol)
{
	Ip = InIp;
	Port = InPort;
	Protocol = InProtocol;

	FIPv4Address::Parse(Ip, IpAddress);

	if (!CreateSocket())
	{
		Shutdown();
		return false;
	}

	if (!CreateReceiver())
	{
		Shutdown();
		return false;
	}
	if (!CreateSender())
	{
		Shutdown();
		return false;
	}

	UE_LOG(LogTemp, Log, TEXT("Init UdpSocket."));
	return true;
}

void UUdpSocket::Shutdown()
{
	DeleteReceiver();
	DeleteSender();
	DeleteSocket();

	
	UE_LOG(LogTemp, Log, TEXT("Shutdown UdpSocket."));
}

bool UUdpSocket::CreateSocket()
{
	if (IpAddress.IsMulticastAddress())
	{
		Socket = FUdpSocketBuilder(TEXT("Multicast"))
			.WithMulticastLoopback()
			.WithMulticastTtl(1)
			.JoinedToGroup(IpAddress)
			.BoundToPort(Port)
			.Build();
	}
	else
	{
		Socket = FUdpSocketBuilder(TEXT("Unicast"))
			.BoundToAddress(IpAddress)
			.BoundToPort(Port)
			.Build();
	}

	if (!Socket)
	{
		UE_LOG(LogTemp, Error, TEXT("Build Socket failed."));
		return false;
	}

	return true;
}

void UUdpSocket::DeleteSocket()
{
	if (!Socket) return;

	Socket->Close();
	delete Socket;
	Socket = nullptr;
}

bool UUdpSocket::CreateReceiver()
{
	FString threaddesc = "zlReciver";
	threaddesc.AppendInt(FAsyncThreadIndex::GetNext());
	const TCHAR* tdc = *threaddesc;
	Receiver = new FUdpSocketReceiver(Socket, FTimespan::FromMilliseconds(1), tdc);

	if (!Receiver)
	{
		UE_LOG(LogTemp, Error, TEXT("Construct Receiver failed."));
		return false;
	}

	Receiver->OnDataReceived().BindUObject(this, &UUdpSocket::OnDataReceived);

	Receiver->Start();

	return true;
}

void UUdpSocket::DeleteReceiver()
{
	if (!Receiver) return;

	Receiver->Stop();
	delete Receiver;
	Receiver = nullptr;
}

void UUdpSocket::OnDataReceived(const FArrayReaderPtr& Reader, const FIPv4Endpoint& Sender)
{
	if (GEngine)
	{
		FString DataStr = FString::FromHexBlob(Reader->GetData(), Reader->Num());
		UE_LOG(LogTemp, Log, TEXT("%s"), *DataStr);
	}

	if (!Protocol->Parse(Reader))
	{
		UE_LOG(LogTemp, Error, TEXT("UUdpSocket::OnDataReceived failed."));
	}
}


bool UUdpSocket::CreateSender()
{
	FString threaddesc = "zlsender";
	threaddesc.AppendInt(FAsyncThreadIndex::GetNext());
	const TCHAR* tdc = *threaddesc;
	Sender = new FUdpSocketSender(Socket,tdc);

	if (!Sender)
	{
		UE_LOG(LogTemp, Error, TEXT("Construct sender failed."));
		return false;
	}

	return true;

}

void UUdpSocket::DeleteSender()
{
	if (!Sender) return;

	Sender->Stop();
	delete Sender;
	Sender = nullptr;
}
void UUdpSocket::Sendto(FTransformData Transdata, FIPv4Endpoint recipient)
{

	FArrayWriter writer;
	writer << Transdata;
	TSharedRef<TArray<uint8>, ESPMode::ThreadSafe> *Data = new TSharedRef<TArray<uint8>, ESPMode::ThreadSafe>(&writer);
	
	Sender->Send(*Data,recipient);
}

void  UUdpSocket::SetRate(uint32 rate)
{
	Sender->SetSendRate(rate);

}