#pragma once

#include "NetworkProtocolInterface.h"
#include "NetworkProtocolColor.generated.h"

USTRUCT(BlueprintType)
struct FColorData
{
	GENERATED_USTRUCT_BODY()

public:
	friend FArchive& operator<< (FArchive& Ar, FColorData& Data)
	{
		auto& ReturnValue = Ar << Data.Color << Data.TimeStamp;

		return ReturnValue;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network|ColorData")
	FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Network|ColorData")
	float TimeStamp;
};

UCLASS()
class UDPCOMMUNICATION_API UNetworkProtocolColor : public UObject, public INetworkProtocolInterface
{
	GENERATED_BODY()

public:
	UNetworkProtocolColor(const FObjectInitializer& ObjectInitializer);

public:
	
	virtual bool Parse(const FArrayReaderPtr& Reader) override;

	
	UFUNCTION(BlueprintCallable, Category = "Network")
	bool GetLatest(FColorData& Data);

	
	UFUNCTION(BlueprintCallable, Category = "Network")
	void Clear();

private:
	
	TArray<FColorData> Stack;
};
