// Copyright © 2020 David ten Kate - All Rights Reserved.

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "DiscordMessengerBPLibrary.generated.h"

USTRUCT(BlueprintType, Category="DiscordMessenger")
struct FDiscordAuthor
{
	GENERATED_BODY()

	FDiscordAuthor()
        : Name(TEXT("")), Url(TEXT("")), IconUrl(TEXT(""))
	{}

	FDiscordAuthor(const FString& AName, const FString& AUrl, const FString& AIconUrl)
		: Name(AName), Url(AUrl), IconUrl(AIconUrl)
	{}
	
	UPROPERTY(BlueprintReadWrite, Category="Discord")
	FString Name;

	UPROPERTY(BlueprintReadWrite, Category="Discord")
	FString Url;
	
	UPROPERTY(BlueprintReadWrite, Category="Discord")
	FString IconUrl;
};

USTRUCT(BlueprintType, Category="DiscordMessenger")
struct FDiscordField
{
	GENERATED_BODY()

	FDiscordField()
		: Name(TEXT("")), Value(TEXT("")), IsInline(false)
	{}

	FDiscordField(const FString& AName, const FString& AValue, bool AIsInline)
		: Name(AName), Value(AValue), IsInline(AIsInline)
	{}
	
	UPROPERTY(BlueprintReadWrite, Category="Discord")
	FString Name;

	UPROPERTY(BlueprintReadWrite, Category="Discord")
	FString Value;
	
	UPROPERTY(BlueprintReadWrite, Category="Discord")
	bool IsInline;
};

USTRUCT(BlueprintType, Category="DiscordMessenger")
struct FDiscordEmbed
{
	GENERATED_BODY()
	
	UPROPERTY(BlueprintReadWrite, Category="Discord")
	FString Title;

	UPROPERTY(BlueprintReadWrite, Category="Discord")
	FString Description;

	UPROPERTY(BlueprintReadWrite, Category="Discord")
	int32 Color;
	
	UPROPERTY(BlueprintReadWrite, Category="Discord")
	FString Url;

	UPROPERTY(BlueprintReadWrite, Category="Discord")
	FDiscordAuthor Author;

	UPROPERTY(BlueprintReadWrite, Category="Discord")
	TArray<FDiscordField> Fields;

	UPROPERTY(BlueprintReadWrite, Category="Discord")
	FString Image;

	UPROPERTY(BlueprintReadWrite, Category="Discord")
	FString Footer;
};

UCLASS()
class UDiscordMessengerBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	UFUNCTION(BlueprintCallable, Category="DiscordMessenger", meta = (AutoCreateRefTerm = "Embed, Files"))
	static void SendDiscordMessage(
		const FString& Webhook,
		const FString& Content,
		const FDiscordEmbed& Embed,
		const TArray<FString>& Files,
		bool IncludeEmbed = true
		);
};