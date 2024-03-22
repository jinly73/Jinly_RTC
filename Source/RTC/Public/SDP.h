// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Dom/JsonObject.h"
#include "SocketIOClientComponent.h"
#include "SDP.generated.h"


// 미디어 설명을 위한 구조체
USTRUCT(BlueprintType)
struct FMediaDescription {

	GENERATED_BODY()

	FString type; // 예: "audio", "video"
	int32 port;
	FString protocol; // 예: "RTP/AVP"
	TArray<int32> format; // 타입들
};

// 미디어 속성을 위한 구조체
USTRUCT(BlueprintType)
struct FAttributeDescription {

	GENERATED_BODY()

	FString a;
};

// SDP 전체를 나타내는 구조체
USTRUCT(BlueprintType)
struct FSessionDescription {

	GENERATED_BODY()
	int32 v; // 프로토콜 버전, 보통 0
	FString o; // originator
	FString s; // sessionName
	FString c; // connectionInfo
	FString t; // timing
	TArray<FMediaDescription> m; // mediaDescriptions 미디어 설명들
	TArray<FAttributeDescription> a; // attribute 미디어 속성들
	//참고:https://brunch.co.kr/@linecard/141
};

// ICE Candidate를 나타내는 구조체
USTRUCT(BlueprintType)
struct FIceCandidate
{

	GENERATED_BODY()

	FString Candidate;
	FString SdpMid;
	int32 SdpMLineIndex;
	FString Foundation;
	int32 Component;
	FString Protocol;
	uint32 Priority;
	FString Address;
	int32 Port;
	FString Type;
};



UCLASS()
class RTC_API ASDP: public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASDP();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTC")
	USocketIOClientComponent* siocc;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RTC")
	FSIOConnectParams SIOConnectParams;

public:
	FString RoomName;
	TSharedPtr<FJsonObject> SelfInfo;
public:

	UFUNCTION(BlueprintCallable, Category = "RTC")
	void JoinRoom(FString roomName, FString email);

	void Offer(TSharedPtr<FJsonObject> JsonObject);
	
	void Answer(TSharedPtr<FJsonObject> JsonObject);

	UFUNCTION(BlueprintCallable, Category = "RTC")
	void Candidate(FString candidateSendID, FIceCandidate candidate, FString candidateReceiveID);

	UFUNCTION(BlueprintCallable, Category = "RTC")
	void PrintOnNativeEvent();

	UFUNCTION(BlueprintCallable, Category = "RTC")
	void GetOffer();

	UFUNCTION(BlueprintCallable, Category = "RTC")
	void GetAnswer();

	UFUNCTION(BlueprintCallable, Category = "RTC")
	void GetCandidate();
public:
	USIOJsonValue* ConvertSessionDescriptionToSIOJsonValue(const FSessionDescription& SessionDesc);
	USIOJsonValue* IceCandidateToSIOJsonValue(const FIceCandidate& Candidate);
	TSharedPtr<FJsonObject> ConvertDataToJsonObject(FString roomName, FString email);
	TSharedPtr<FJsonObject> ConvertOfferDataToJsonObject(FString offerSendID, FString offerSendEmail, FSessionDescription sdp, FString offerReceiveID);
	TSharedPtr<FJsonObject> ConvertAnswerDataToJsonObject(FString answerSendID, FSessionDescription sdp, FString answerReceiveID);
	TSharedPtr<FJsonObject> ConvertCandidateDataToJsonObject(FString candidateSendID, FIceCandidate candidate, FString candidateReceiveID);
	TSharedPtr<FJsonObject> ConvertSessionDescriptionToJsonObject(FSessionDescription sdp);
	TSharedPtr<FJsonObject> ConvertCandidateToJsonObject(FIceCandidate& Candidate);

public:
	void OnConnectionFail();

};
