// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "NetworkProtocolTransform.h"
#include "NetworkProtocolColor.h"
#include "UDPCommunicationGameMode.generated.h"


UCLASS()
class UDPCOMMUNICATION_API AUDPCommunicationGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AUDPCommunicationGameMode();
	AUDPCommunicationGameMode(const FObjectInitializer& ObjectInitializer);

public:
	virtual void BeginPlay();
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason);
	
	virtual void Init();

	
	virtual void Shutdown();


	UFUNCTION(BlueprintPure, Category = "Network")
		UNetworkProtocolTransform* GetTransformProtocol() const { return TransformProtocol; }

	
	UFUNCTION(BlueprintPure, Category = "Network")
		UNetworkProtocolColor* GetColorProtocol() const { return ColorProtocol; }
private:

	bool InitTransformProtocol();

	
	void ShutdownTransformProtocol();


	bool InitColorProtocol();

	
	void ShutdownColorProtocol();
	UFUNCTION(BlueprintCallable)
	void Send(FTransformData Transdata);

private:
	
	UPROPERTY()
	class UNetworkProtocolTransform* TransformProtocol;

	
	UPROPERTY()
	class UNetworkProtocolColor* ColorProtocol;

	
	UPROPERTY()
	class UUdpSocket* TransformUdpSocket;


	UPROPERTY()
	class UUdpSocket* ColorUdpSocket;
	
	
};
