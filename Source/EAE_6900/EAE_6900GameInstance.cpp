// Fill out your copyright notice in the Description page of Project Settings.

#include "EAE_6900GameInstance.h"

// engine includes
#include "Kismet/GameplayStatics.h"
#include "Json.h"
#include "JsonUtilities.h"

// game includes

// static member initialization
UEAE_6900GameInstance* UEAE_6900GameInstance::Instance = nullptr;
const FString UEAE_6900GameInstance::ManifestSlotName(TEXT("EAE_6900_MAN"));
const FString UEAE_6900GameInstance::LevelSlotPrefix(TEXT("EAE_6900_LVL_"));

UEAE_6900GameInstance::UEAE_6900GameInstance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Http = &FHttpModule::Get();
}

void UEAE_6900GameInstance::Init()
{
	UE_LOG(LogGame, Log, TEXT("UEAE_6900GameInstance::Init!"));
	UEAE_6900GameInstance::Instance = this;

	LoadManifest();
}

void UEAE_6900GameInstance::Shutdown()
{
	UEAE_6900GameInstance::Instance = nullptr;
	UE_LOG(LogGame, Log, TEXT("UEAE_6900GameInstance::Shutdown!"));
}

bool UEAE_6900GameInstance::BeginPlay()
{
	SaveManifest();
	return true;
}

bool UEAE_6900GameInstance::EndPlay(EEndPlayReason::Type EndPlayReasonIn)
{
	SaveableObjectList.Empty();

	CurrentlyLoadedLevelData.LevelName = NAME_None;
	CurrentlyLoadedLevelData.PlayerSaveData = FPlayerSaveData();
	CurrentlyLoadedLevelData.Destructibles.Empty();
	CurrentlyLoadedLevelData.Collectibles.Empty();

	return true;
}

void UEAE_6900GameInstance::LoadManifest()
{
#if ENABLE_REMOTE_STORAGE
	Request_GetManifest();
#else
	if (UGameplayStatics::DoesSaveGameExist(ManifestSlotName, 0) == false)
	{
		SaveManifest();
		return;
	}

	if (UEAE_6900ManifestSave* LoadManifestInstance = Cast<UEAE_6900ManifestSave>(UGameplayStatics::LoadGameFromSlot(ManifestSlotName, 0)))
	{
		ManifestData.LevelTimestampList = LoadManifestInstance->ManifestData.LevelTimestampList;
		UE_LOG(LogGame, Log, TEXT("Manifest loaded successfully!"));
	}
	else
	{
		UE_LOG(LogGame, Error, TEXT("Couldn't load manifest!"));
	}
#endif // ENABLE_REMOTE_STORAGE
}

void UEAE_6900GameInstance::SaveManifest()
{
#if ENABLE_REMOTE_STORAGE
	{
		// get the manifest data stringified to json
		FString ManifestJsonString;
		FJsonObjectConverter::UStructToJsonObjectString<FManifestData>(ManifestData, ManifestJsonString);
		Request_PostManifest(ManifestJsonString);
		//UE_LOG(LogGame, Log, TEXT("ManifestJsonString:\n%s"), *ManifestJsonString);
	}
#else
	if (UEAE_6900ManifestSave* ManifestSaveInstance = Cast<UEAE_6900ManifestSave>(UGameplayStatics::CreateSaveGameObject(UEAE_6900ManifestSave::StaticClass())))
	{
		ManifestSaveInstance->ManifestData.LevelTimestampList = ManifestData.LevelTimestampList;

		UGameplayStatics::SaveGameToSlot(ManifestSaveInstance, ManifestSlotName, 0);
		UE_LOG(LogGame, Log, TEXT("Manifest saved successfully!"));
	}
	else
	{
		UE_LOG(LogGame, Error, TEXT("Couldn't save manifest!"));
	}
#endif // ENABLE_REMOTE_STORAGE

}

void UEAE_6900GameInstance::LoadLevel(int32 Index)
{
	ensure(Index >= 0 && Index < ManifestData.LevelTimestampList.Num());

	FString LevelSaveSlotName;
	GetLevelSaveSlotName(LevelSaveSlotName, Index);

	if (UGameplayStatics::DoesSaveGameExist(LevelSaveSlotName, 0) == false)
	{
		UE_LOG(LogGame, Error, TEXT("Couldn't find level save file for index:%d"), Index);
		return;
	}

	if (UEAE_6900LevelSave* LoadLevelSaveInstance = Cast<UEAE_6900LevelSave>(UGameplayStatics::LoadGameFromSlot(LevelSaveSlotName, 0)))
	{
		bCurrentLevelDataExists = true;
		CurrentlyLoadedLevelData.LevelName = LoadLevelSaveInstance->LevelSaveData.LevelName;
		CurrentlyLoadedLevelData.PlayerSaveData.PlayerHealth = LoadLevelSaveInstance->LevelSaveData.PlayerSaveData.PlayerHealth;
		CurrentlyLoadedLevelData.PlayerSaveData.PlayerAmmo = LoadLevelSaveInstance->LevelSaveData.PlayerSaveData.PlayerAmmo;
		CurrentlyLoadedLevelData.PlayerSaveData.PlayerTransform = LoadLevelSaveInstance->LevelSaveData.PlayerSaveData.PlayerTransform;

		for (const auto& DestructibleSaveData : LoadLevelSaveInstance->LevelSaveData.Destructibles)
		{
			CurrentlyLoadedLevelData.Destructibles.Add(DestructibleSaveData.Key, DestructibleSaveData.Value);
		}

		for (const auto& CollectibleSaveData : LoadLevelSaveInstance->LevelSaveData.Collectibles)
		{
			CurrentlyLoadedLevelData.Collectibles.Add(CollectibleSaveData.Key, CollectibleSaveData.Value);
		}

		UE_LOG(LogGame, Log, TEXT("Level for index:%d loaded successfully!"), Index);
	}
	else
	{
		UE_LOG(LogGame, Error, TEXT("Couldn't load level save file for index:%d"), Index);
	}
}

void UEAE_6900GameInstance::SaveLevel()
{
	FLevelSaveData LevelDataToSave;
	for (const auto& SaveableObject : SaveableObjectList)
	{
		SaveableObject->SubmitDataToBeSaved(LevelDataToSave);
	}

#if ENABLE_REMOTE_STORAGE
	{
		// get the level save data stringified to json
		FString LevelSaveJsonString;
		FJsonObjectConverter::UStructToJsonObjectString<FLevelSaveData>(LevelDataToSave, LevelSaveJsonString);
		UE_LOG(LogGame, Log, TEXT("LevelSaveJsonString:\n%s"), *LevelSaveJsonString);

		// must save the manifest after adding a new level
		ManifestData.LevelTimestampList.Add(FDateTime::Now());
		SaveManifest();
	}
#else
	if (UEAE_6900LevelSave* LevelSaveInstance = Cast<UEAE_6900LevelSave>(UGameplayStatics::CreateSaveGameObject(UEAE_6900LevelSave::StaticClass())))
	{
		// copy all properties into the save game object
		{
			LevelSaveInstance->LevelSaveData.LevelName = LevelDataToSave.LevelName;
			LevelSaveInstance->LevelSaveData.PlayerSaveData.PlayerHealth = LevelDataToSave.PlayerSaveData.PlayerHealth;
			LevelSaveInstance->LevelSaveData.PlayerSaveData.PlayerAmmo = LevelDataToSave.PlayerSaveData.PlayerAmmo;
			LevelSaveInstance->LevelSaveData.PlayerSaveData.PlayerTransform = LevelDataToSave.PlayerSaveData.PlayerTransform;

			for (const auto& DestructibleSaveData : LevelDataToSave.Destructibles)
			{
				LevelSaveInstance->LevelSaveData.Destructibles.Add(DestructibleSaveData.Key, DestructibleSaveData.Value);
			}

			for (const auto& CollectibleSaveData : LevelDataToSave.Collectibles)
			{
				LevelSaveInstance->LevelSaveData.Collectibles.Add(CollectibleSaveData.Key, CollectibleSaveData.Value);
			}
		}

		// get a new slot name for this save file
		FString LevelSaveSlotName;
		GetLevelSaveSlotName(LevelSaveSlotName, ManifestData.LevelTimestampList.Num());

		UGameplayStatics::SaveGameToSlot(LevelSaveInstance, LevelSaveSlotName, 0);
		UE_LOG(LogGame, Log, TEXT("Level saved successfully!"));

		// must save the manifest after adding a new level
		ManifestData.LevelTimestampList.Add(FDateTime::Now());
		SaveManifest();
	}
	else
	{
		UE_LOG(LogGame, Error, TEXT("Couldn't save level!"));
	}
#endif // ENABLE_REMOTE_STORAGE

}

void UEAE_6900GameInstance::GetLevelSaveSlotName(FString& OutLevelSaveSlotName, const int32 Index) const
{
	OutLevelSaveSlotName = FString::Printf(TEXT("%s%02d"), *LevelSlotPrefix, Index);
}

//~==============================================================================
// HTTP

void UEAE_6900GameInstance::MakeRequest()
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UEAE_6900GameInstance::OnResponseReceived);
	Request->SetURL("http://eae-6900.getsandbox.com/hello");
	Request->SetVerb("GET");
	Request->SetHeader("User-Agent", "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", "application/json");
	Request->ProcessRequest();
}

void UEAE_6900GameInstance::OnResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	TSharedPtr<FJsonObject> JsonObject;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

	if (FJsonSerializer::Deserialize(Reader, JsonObject))
	{
		FString Name = JsonObject->GetStringField("name");
		UE_LOG(LogGame, Log, TEXT("Received name:%s from server!"), *Name);
	}
}

void UEAE_6900GameInstance::Request_PostManifest(const FString& JsonString)
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UEAE_6900GameInstance::Response_PostManifest);
	Request->SetURL("http://eae-6900.getsandbox.com/manifest");
	Request->SetVerb("POST");
	Request->SetHeader("User-Agent", "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(JsonString);
	Request->ProcessRequest();
}

void UEAE_6900GameInstance::Response_PostManifest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UE_LOG(LogGame, Log, TEXT("POST manifest succeeded!"));
	}
	else
	{
		UE_LOG(LogGame, Error, TEXT("POST manifest failed!"));
	}
}

void UEAE_6900GameInstance::Request_GetManifest()
{
	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UEAE_6900GameInstance::Response_GetManifest);
	Request->SetURL("http://eae-6900.getsandbox.com/manifest");
	Request->SetVerb("GET");
	Request->SetHeader("User-Agent", "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", "application/json");
	Request->ProcessRequest();
}

void UEAE_6900GameInstance::Response_GetManifest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		FManifestData ManifestData;
		FJsonObjectConverter::JsonObjectStringToUStruct<FManifestData>(Response->GetContentAsString(), &ManifestData, 0, 0);
		UE_LOG(LogGame, Log, TEXT("GET manifest succeeded!"));
	}
	else
	{
		UE_LOG(LogGame, Error, TEXT("GET manifest failed!"));
	}
}

void UEAE_6900GameInstance::Request_PostLevel(const FString& JsonString)
{}

void UEAE_6900GameInstance::Response_PostLevel(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{}

void UEAE_6900GameInstance::Request_GetLevel(const FString& JsonString)
{}

void UEAE_6900GameInstance::Response_GetLevel(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{}
