#pragma once

#include "NetworkProtocolInterface.h"
#include "NetworkProtocolTransform.generated.h"

USTRUCT(BlueprintType)
struct FTransformData
{
	GENERATED_USTRUCT_BODY()

public:
	friend FArchive& operator<< (FArchive& Ar, FTransformData& Data)
	{
		auto& ReturnValue = Ar << Data.Location << Data.Rotation << Data.Scale << Data.TimeStamp;

		return ReturnValue;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network|TransformData")
	FVector Location;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network|TransformData")
	FRotator Rotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network|TransformData")
	FVector Scale;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network|TransformData")
	float TimeStamp;
};

/**
 *
 */
UCLASS()
class UDPCOMMUNICATION_API UNetworkProtocolTransform : public UObject, public INetworkProtocolInterface
{
	GENERATED_BODY()

public:
	UNetworkProtocolTransform(const FObjectInitializer& ObjectInitializer);

public:
	
	
	virtual bool Parse(const FArrayReaderPtr& Reader) override;

	
	UFUNCTION(BlueprintCallable, Category = "Network")
	bool GetLatest(FTransformData& Data);

	
	UFUNCTION(BlueprintCallable, Category = "Network")
	void Clear();

private:
	
	TArray<FTransformData> Stack;
};