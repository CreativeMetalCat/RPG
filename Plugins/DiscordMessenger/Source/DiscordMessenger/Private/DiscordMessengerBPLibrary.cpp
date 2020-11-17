// Copyright © 2020 David ten Kate - All Rights Reserved.

#include "DiscordMessengerBPLibrary.h"

#include "HttpModule.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Misc/FileHelper.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

void UDiscordMessengerBPLibrary::SendDiscordMessage(
	const FString& Webhook,
	const FString& Content,
	const FDiscordEmbed& Embed,
	const TArray<FString>& Files,
	bool IncludeEmbed
	)
{
	TSharedPtr<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	
	Request->SetVerb("POST");
	Request->SetURL(Webhook);
	Request->SetHeader(TEXT("Content-Type"), TEXT("application/json"));

	if (Files.Num() > 0)
	{
		Request->SetHeader(TEXT("Content-Type"), TEXT("multipart/form-data;boundary=AaB03x"));
	}
	
	// Set author
	TSharedPtr<FJsonObject> JsonAuthor = MakeShareable(new FJsonObject);
	JsonAuthor->SetStringField("name", Embed.Author.Name);
	JsonAuthor->SetStringField("url", Embed.Author.Url);
	JsonAuthor->SetStringField("icon_url", Embed.Author.IconUrl);

	// Set embed fields
	TArray<TSharedPtr<FJsonValue>> JsonFieldArray;
	for (int i = 0; i < Embed.Fields.Num(); i++)
	{
		TSharedPtr<FJsonObject> JsonField = MakeShareable(new FJsonObject);
		JsonField->SetStringField("name", Embed.Fields[i].Name);
		JsonField->SetStringField("value", Embed.Fields[i].Value);
		JsonField->SetBoolField("inline", Embed.Fields[i].IsInline);

		TSharedRef<FJsonValueObject> JsonValueField = MakeShareable(new FJsonValueObject(JsonField));
		JsonFieldArray.Add(JsonValueField);
	}
	
	// Set footer
	TSharedPtr<FJsonObject> JsonFooter = MakeShareable(new FJsonObject);
	JsonFooter->SetStringField("text", Embed.Footer);
	
	// Set embed
	TSharedPtr<FJsonObject> JsonEmbeds = MakeShareable(new FJsonObject);
	JsonEmbeds->SetStringField("title", Embed.Title);
	JsonEmbeds->SetStringField("description", Embed.Description);
	JsonEmbeds->SetNumberField("color", Embed.Color);
	JsonEmbeds->SetStringField("url", Embed.Url);
	JsonEmbeds->SetObjectField("author", JsonAuthor);
	JsonEmbeds->SetArrayField("fields", JsonFieldArray);
	JsonEmbeds->SetObjectField("footer", JsonFooter);
	
	// Set embed image
	if (Files.Num() > 0 && !Embed.Image.IsEmpty())
	{
		TSharedPtr<FJsonObject> JsonImage = MakeShareable(new FJsonObject);
		FString GetImage = "attachment://";
		JsonImage->SetStringField("url", GetImage.Append(Embed.Image));

		JsonEmbeds->SetObjectField("image", JsonImage);
	}

	TSharedRef<FJsonValueObject> JsonValueEmbeds = MakeShareable(new FJsonValueObject(JsonEmbeds));
	TArray<TSharedPtr<FJsonValue>> JsonArray;
	JsonArray.Add(JsonValueEmbeds);

	// Set main fields
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);
	JsonObject->SetStringField("content", Content);

	if (IncludeEmbed)
	{
		JsonObject->SetArrayField("embeds", JsonArray);
	}
	
	FString Payload = "";
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(&Payload);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	if (Files.Num() > 0)
	{
		TArray<uint8> Data;
		FString StartBorder = "--AaB03x\r\n";
		Data.Append(reinterpret_cast<uint8*>(TCHAR_TO_UTF8(*StartBorder)), StartBorder.Len());
		
		FString NewBorder = "\r\n--AaB03x\r\n";
		int32 FileCounter = 0;
		
		// Add file(s) to Payload
		for (int i = 0; i < Files.Num(); i++)
		{
			TArray<uint8> FileData;
			FFileHelper::LoadFileToArray(FileData, *Files[i]);

			int StartFileNamePos = Files[i].Find("/", ESearchCase::IgnoreCase, ESearchDir::FromEnd);
			FString FileName = Files[i].RightChop(StartFileNamePos + 1);
			
			FString Layout = "Content-Disposition: form-data; name=\"file";
			Layout.Append(FString::FromInt(FileCounter++));
			Layout.Append("\"; filename=\"");			
			Layout.Append(FileName);
			
			if (FileName.Contains(".png", ESearchCase::IgnoreCase, ESearchDir::FromEnd))
			{
				Layout.Append("\"\r\nContent-Type: image/png\r\n\r\n");
			}
			else if (FileName.Contains(".jpg", ESearchCase::IgnoreCase, ESearchDir::FromEnd))
			{
				Layout.Append("\"\r\nContent-Type: image/jpeg\r\n\r\n");
			}
			else if (FileName.Contains(".gif", ESearchCase::IgnoreCase, ESearchDir::FromEnd))
			{
				Layout.Append("\"\r\nContent-Type: image/gif\r\n\r\n");
			}
			else if (FileName.Contains(".txt", ESearchCase::IgnoreCase, ESearchDir::FromEnd))
			{
				Layout.Append("\"\r\nContent-Type: text/plain\r\n\r\n");
			}
			else if (FileName.Contains(".mp4", ESearchCase::IgnoreCase, ESearchDir::FromEnd))
			{
				Layout.Append("\"\r\nContent-Type: video/mp4\r\n\r\n");
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[DiscordMessenger] File type not supported!") );
			}

			Data.Append(reinterpret_cast<uint8*>(TCHAR_TO_UTF8(*Layout)), Layout.Len());
			Data.Append(FileData);
			Data.Append(reinterpret_cast<uint8*>(TCHAR_TO_UTF8(*NewBorder)), NewBorder.Len());
		}

		// Add JSON Payload
		FString Layout = "Content-Disposition: form-data; name=\"payload_json\"\r\n\r\n";
		Data.Append(reinterpret_cast<uint8*>(TCHAR_TO_UTF8(*Layout)), Layout.Len());
		Data.Append(reinterpret_cast<uint8*>(TCHAR_TO_UTF8(*Payload)), Payload.Len());

		FString EndBorder = "\r\n--AaB03x--";
		Data.Append(reinterpret_cast<uint8*>(TCHAR_TO_UTF8(*EndBorder)), EndBorder.Len());
		
		Request->SetContent(Data);
	}
	else
	{
		Request->SetContentAsString(Payload);
	}
	
	Request->ProcessRequest();
}