/*
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/

#include "WSEditorObject.h"
#include "WidgetStudioRuntime.h"
#include "Logging/LogMacros.h"
#include "Engine/Engine.h"
#include "WSSubsystem.h"
#include "WSSettings.h"
#include "Settings/ProjectPackagingSettings.h"

extern FString DefaultGameIni;
extern FString WSShortPath;

UWSEditorObject::UWSEditorObject()
{
}

UWSEditorObject::UWSEditorObject(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	FCoreDelegates::OnEnginePreExit.AddUObject(this, &UWSEditorObject::RemoveWidgetStudioFromCookDirs);
	FCoreDelegates::OnPostEngineInit.AddUObject(this, &UWSEditorObject::BindStyleChangedEvent);
}

void UWSEditorObject::BindStyleChangedEvent()
{
	if (GEngine)
	{
		UWidgetStudioSubsystem* WSSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
		if (IsValid(WSSubsystem))
		{
			WSSubsystem->OnStyleChanged.AddDynamic(this, &UWSEditorObject::HandleStyleChanged);
		}
		else
		{
			UE_LOG(LogWidgetStudio, Warning, TEXT("Attempted to access the Widget Studio Subsystem before it exists."));
		}
	}
	else
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not get GEngine."));
	}

}

void UWSEditorObject::RemoveWidgetStudioFromCookDirs()
{
	UProjectPackagingSettings* PackagingSettings = GetMutableDefault<UProjectPackagingSettings>();
	if (!PackagingSettings) { UE_LOG(LogWidgetStudio, Warning, TEXT("Could not get Packaging Settings.  Could not add '/WidgetStudio' to cook directories.")); return; }

	TArray<FString> AddCookDirsArray;

	bool bAllInstancesRemoved = false;

	while (!bAllInstancesRemoved)
	{
		int FoundIndex = -1;
		for (int i = 0; i < PackagingSettings->DirectoriesToAlwaysCook.Num(); i++)
		{
			if (PackagingSettings->DirectoriesToAlwaysCook[i].Path == WSShortPath)
			{
				FoundIndex = i;
				break;
			}
		}

		if (FoundIndex == -1)
		{
			bAllInstancesRemoved = true;
		}

		else if (FoundIndex >= 0 && FoundIndex < PackagingSettings->DirectoriesToAlwaysCook.Num())
		{
			PackagingSettings->DirectoriesToAlwaysCook.RemoveAt(FoundIndex);
#if ENGINE_MAJOR_VERSION <= 4
			PackagingSettings->UpdateDefaultConfigFile();
#else
			if (!PackagingSettings->TryUpdateDefaultConfigFile())
			{
				UE_LOG(LogWidgetStudio, Warning, TEXT("Could not update the default config file."));
			}
#endif
		}
	}
}

void UWSEditorObject::HandleStyleChanged()
{
	const UWidgetStudioSubsystem* WSSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	UWidgetStudioSettings* WSSettings = GetMutableDefault<UWidgetStudioSettings>();

	WSSettings->Theme = WSSubsystem->Theme;
	WSSettings->IconSet = WSSubsystem->IconSet;
	WSSettings->Typography = WSSubsystem->Typography;
	WSSettings->BorderRadius = WSSubsystem->BorderRadius;
	WSSettings->IdealWidth = WSSubsystem->IdealWidth;
	WSSettings->IdealHeight = WSSubsystem->IdealHeight;
}
