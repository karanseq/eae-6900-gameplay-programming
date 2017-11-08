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
	{
		Request_GetManifest();
	}
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

	FString LevelSaveTime(ManifestData.LevelTimestampList[Index].ToString());

#if ENABLE_REMOTE_STORAGE
	{
		Request_GetLevel(LevelSaveTime);
	}
#else
	if (UGameplayStatics::DoesSaveGameExist(LevelSaveTime, 0) == false)
	{
		UE_LOG(LogGame, Error, TEXT("Couldn't find level save file for index:%d"), Index);
		return;
	}

	if (UEAE_6900LevelSave* LoadLevelSaveInstance = Cast<UEAE_6900LevelSave>(UGameplayStatics::LoadGameFromSlot(LevelSaveTime, 0)))
	{
		bCurrentLevelDataExists = true;
		CurrentlyLoadedLevelData.LevelSaveTime = LoadLevelSaveInstance->LevelSaveData.LevelSaveTime;
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
#endif // ENABLE_REMOTE_STORAGE
}

void UEAE_6900GameInstance::SaveLevel()
{
	const FDateTime Now(FDateTime::Now());

	FLevelSaveData LevelDataToSave;
	LevelDataToSave.LevelSaveTime = Now;

	// get data from all "saveables"
	for (const auto& SaveableObject : SaveableObjectList)
	{
		SaveableObject->SubmitDataToBeSaved(LevelDataToSave);
	}

#if ENABLE_REMOTE_STORAGE
	{
		// get the level save data stringified to json
		FString LevelSaveJsonString;
		FJsonObjectConverter::UStructToJsonObjectString<FLevelSaveData>(LevelDataToSave, LevelSaveJsonString);
		Request_PostLevel(LevelSaveJsonString);
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
		UGameplayStatics::SaveGameToSlot(LevelSaveInstance, Now.ToString(), 0);
		UE_LOG(LogGame, Log, TEXT("Level saved successfully!"));
	}
	else
	{
		UE_LOG(LogGame, Error, TEXT("Couldn't save level!"));
	}
#endif // ENABLE_REMOTE_STORAGE

	// must save the manifest after adding a new level
	ManifestData.LevelTimestampList.Add(Now);
	SaveManifest();
}

void UEAE_6900GameInstance::DeleteLevel(int32 Index)
{
	ensure(Index >= 0 && Index < ManifestData.LevelTimestampList.Num());

	FString LevelSaveTime(ManifestData.LevelTimestampList[Index].ToString());

#if ENABLE_REMOTE_STORAGE
	{
		Request_DeleteLevel(LevelSaveTime);
	}
#else
	{
		if (UGameplayStatics::DoesSaveGameExist(LevelSaveTime, 0) == false)
		{
			UE_LOG(LogGame, Error, TEXT("Couldn't find level save file for index:%d"), Index);
		}

		if (UGameplayStatics::DeleteGameInSlot(LevelSaveTime, 0))
		{
			UE_LOG(LogGame, Log, TEXT("Level for index:%d deleted succesfully!"), Index);
		}
		else
		{
			UE_LOG(LogGame, Error, TEXT("Couldn't delete level save file for index:%d"), Index);
		}
	}
#endif // ENABLE_REMOTE_STORAGE

	// must save the manifest after deleting a level
	ManifestData.LevelTimestampList.RemoveAt(Index);
	SaveManifest();
}

//~==============================================================================
// HTTP

void UEAE_6900GameInstance::Request_PostManifest(const FString& JsonString)
{
	static const FString PostManifestURL("http://eae-6900.getsandbox.com/manifest");

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UEAE_6900GameInstance::Response_PostManifest);
	Request->SetURL(PostManifestURL);
	Request->SetVerb("POST");
	Request->SetHeader("User-Agent", "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(JsonString);
	Request->ProcessRequest();
}

void UEAE_6900GameInstance::Response_PostManifest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful &&
		Response->GetResponseCode() == EHttpResponseCodes::Ok)
	{
		UE_LOG(LogGame, Log, TEXT("POST manifest succeeded!"));
	}
	else
	{
		UE_LOG(LogGame, Warning, TEXT("POST manifest failed!"));
	}
}

void UEAE_6900GameInstance::Request_GetManifest()
{
	static const FString GetManifestURL("http://eae-6900.getsandbox.com/manifest");

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UEAE_6900GameInstance::Response_GetManifest);
	Request->SetURL(GetManifestURL);
	Request->SetVerb("GET");
	Request->SetHeader("User-Agent", "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", "application/json");
	Request->ProcessRequest();
}

void UEAE_6900GameInstance::Response_GetManifest(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful &&
		Response->GetResponseCode() == EHttpResponseCodes::Ok)
	{
		FManifestData RemoteManifestData;
		FJsonObjectConverter::JsonObjectStringToUStruct<FManifestData>(Response->GetContentAsString(), &RemoteManifestData, 0, 0);

		ManifestData.LevelTimestampList = RemoteManifestData.LevelTimestampList;
		UE_LOG(LogGame, Log, TEXT("GET manifest succeeded!"));
	}
	else
	{
		SaveManifest();
		UE_LOG(LogGame, Warning, TEXT("GET manifest failed!"));
	}
}

void UEAE_6900GameInstance::Request_PostLevel(const FString& JsonString)
{
	static const FString PostLevelURL("http://eae-6900.getsandbox.com/level");

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UEAE_6900GameInstance::Response_PostLevel);
	Request->SetURL(PostLevelURL);
	Request->SetVerb("POST");
	Request->SetHeader("User-Agent", "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", "application/json");
	Request->SetContentAsString(JsonString);
	Request->ProcessRequest();
}

void UEAE_6900GameInstance::Response_PostLevel(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful &&
		Response->GetResponseCode() == EHttpResponseCodes::Ok)
	{
		UE_LOG(LogGame, Log, TEXT("POST level succeeded!"));
	}
	else
	{
		UE_LOG(LogGame, Warning, TEXT("POST level failed!"));
	}
}

void UEAE_6900GameInstance::Request_GetLevel(const FString& LevelSaveTime)
{
	static const FString GetLevelURL("http://eae-6900.getsandbox.com/level");

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UEAE_6900GameInstance::Response_GetLevel);
	Request->SetURL(FString::Printf(TEXT("%s/%s"), *GetLevelURL, *LevelSaveTime));
	Request->SetVerb("GET");
	Request->SetHeader("User-Agent", "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", "application/json");
	Request->ProcessRequest();
}

void UEAE_6900GameInstance::Response_GetLevel(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful &&
		Response->GetResponseCode() == EHttpResponseCodes::Ok)
	{
		FLevelSaveData RemoteLevelData;
		FJsonObjectConverter::JsonObjectStringToUStruct<FLevelSaveData>(Response->GetContentAsString(), &RemoteLevelData, 0, 0);
		
		bCurrentLevelDataExists = true;
		CurrentlyLoadedLevelData = RemoteLevelData;

		UE_LOG(LogGame, Log, TEXT("GET level succeeded!"));
	}
	else
	{
		UE_LOG(LogGame, Warning, TEXT("GET level failed!"));
	}
}

void UEAE_6900GameInstance::Request_DeleteLevel(const FString& LevelSaveTime)
{
	static const FString DeleteLevelURL("http://eae-6900.getsandbox.com/level");

	TSharedRef<IHttpRequest> Request = Http->CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UEAE_6900GameInstance::Response_DeleteLevel);
	Request->SetURL(FString::Printf(TEXT("%s/%s"), *DeleteLevelURL, *LevelSaveTime));
	Request->SetVerb("DELETE");
	Request->SetHeader("User-Agent", "X-UnrealEngine-Agent");
	Request->SetHeader("Content-Type", "application/json");
	Request->ProcessRequest();
}

void UEAE_6900GameInstance::Response_DeleteLevel(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful &&
		Response->GetResponseCode() == EHttpResponseCodes::Ok)
	{
		UE_LOG(LogGame, Log, TEXT("DELETE level succeeded!"));
	}
	else
	{
		UE_LOG(LogGame, Warning, TEXT("DELETE level failed!"));
	}
}
