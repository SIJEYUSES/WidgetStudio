/*
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/

#include "WidgetStudioEditor.h"
#include "WSSubsystem.h"
#include "WSSettings.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#include "ISettingsContainer.h"
#include "WSGlobals.h"
#include "PropertyEditorModule.h"
#include "Settings/ProjectPackagingSettings.h"

#include "DetailCustomization/WSBaseDetails.h"
#include "Widgets/Modern/WSModernButton.h"

#define LOCTEXT_NAMESPACE "FWidgetStudioEditor"
DEFINE_LOG_CATEGORY(LogWidgetStudio);

void FWidgetStudioEditor::StartupModule()
{
	RegisterDetailCustomization();

	// Register Settings
	RegisterSettings();
	WfSettings = GetMutableDefault<UWidgetStudioSettings>();

	WfSettings->Theme = DefaultThemePath;
	WfSettings->IconSet = DefaultIconSetPath;
	WfSettings->Typography = DefaultTypographyPath;

	// If there are values in ini file, override defaults
	WfSettings->LoadConfig();

	// Initial loading of assets
	LoadObject<UWidgetStudioTheme>(nullptr, *WfSettings->Theme.ToString());
	LoadObject<UWidgetStudioIconSet>(nullptr, *WfSettings->IconSet.ToString());
	LoadObject<UWidgetStudioTypography>(nullptr, *WfSettings->Typography.ToString());

	// Save values to ini file in case any of them are missing
	WfSettings->SaveConfig();
	VerifyWidgetStudioInCookDirs();
}

void FWidgetStudioEditor::ShutdownModule()
{
	if (UObjectInitialized())
	{
		UnregisterSettings();
	}

	UnregisterDetailCustomization();
}

bool FWidgetStudioEditor::SupportsDynamicReloading()
{
	return true;
}

/**
* Check if "/WidgetStudio" exists in DirectoriesToAlwaysCook.  If not, add it.
*/
void FWidgetStudioEditor::VerifyWidgetStudioInCookDirs()
{
	UProjectPackagingSettings* PackagingSettings = GetMutableDefault<UProjectPackagingSettings>();
	if (!PackagingSettings) { UE_LOG(LogWidgetStudio, Warning, TEXT("Could not get Packaging Settings.  Could not add '/WidgetStudio' to cook directories.")); return; }

#if ENGINE_MAJOR_VERSION <= 4
	PackagingSettings->UpdateDefaultConfigFile();
#else
	if (!PackagingSettings->TryUpdateDefaultConfigFile())
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not update the default config file."));
	}
#endif

	FDirectoryPath WidgetStudioDirPath;
	WidgetStudioDirPath.Path = WSShortPath;
	const FDirectoryPath ConstWidgetStudioDirPath = WidgetStudioDirPath;

	TArray<FString> AddCookDirsArray;

	if (!GConfig)
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not get GConfig."));
		return;
	}
	GConfig->GetArray(*PackagingSettingsSection, *AddCookDirs, AddCookDirsArray, DefaultGameIni);

	int32 Index = AddCookDirsArray.Find(WSPathIni);

	bool bIsDirFound = false;
	for (int i = 0; i < PackagingSettings->DirectoriesToAlwaysCook.Num(); i++)
	{
		if (PackagingSettings->DirectoriesToAlwaysCook[i].Path == WSShortPath)
		{
			bIsDirFound = true;
			break;
		}
	}

	if (Index == INDEX_NONE && !bIsDirFound)
	{
		PackagingSettings->DirectoriesToAlwaysCook.Add(WidgetStudioDirPath);
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

/**
*
*/
bool FWidgetStudioEditor::HandleSettingsSaved() const
{
	UWidgetStudioSettings* Settings = GetMutableDefault<UWidgetStudioSettings>();
	constexpr bool ReSaveSettings = false; // FIXME: This bool is always false, thus Settings->SaveConfig() is never used.

	if (ReSaveSettings)
	{
		Settings->SaveConfig();
	}

	return true;
}

void FWidgetStudioEditor::RegisterSettings() const
{
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		// Create a new category
		const ISettingsContainerPtr SettingsContainer = SettingsModule->GetContainer("Project");

		SettingsContainer->DescribeCategory(
			"Theia Interactive",
			LOCTEXT("RuntimeWDCategoryName", "Theia Interactive"),
			LOCTEXT("RuntimeWDCategoryDescription", "Game configuration settings for plugins created by Theia Interactive.")
			);

		// Register Settings
		const ISettingsSectionPtr SettingsSection = SettingsModule->RegisterSettings(
			"Project",
			"Theia Interactive",
			"Widget Studio",
			LOCTEXT("RuntimeGeneralSettingsName", "Widget Studio"),
			LOCTEXT("RuntimeGeneralSettingsDescription", "Configure the properties of the Widget Studio."),
			GetMutableDefault<UWidgetStudioSettings>()
			);

		// Register the save handler
		if (SettingsSection.IsValid())
		{
			SettingsSection->OnModified().BindRaw(this, &FWidgetStudioEditor::HandleSettingsSaved);
		}
	}
}

void FWidgetStudioEditor::UnregisterSettings() const
{
	// Ensure to unregister all of your registered settings here, hot-reload would
	// otherwise yield unexpected results.

	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Theia Interactive", "General");
	}
}

void FWidgetStudioEditor::RegisterDetailCustomization() const
{
	FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

	PropertyModule.RegisterCustomClassLayout(
		UWidgetStudioBase::StaticClass()->GetFName(),
		FOnGetDetailCustomizationInstance::CreateStatic(&FModernButtonDetails::MakeInstance)
		);

	PropertyModule.NotifyCustomizationModuleChanged();

	UE_LOG(LogWidgetStudio, Verbose, TEXT("Detail Customizations Registered."))
}

void FWidgetStudioEditor::UnregisterDetailCustomization() const
{
	if (FModuleManager::Get().IsModuleLoaded("PropertyEditor"))
	{
		auto& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");

		PropertyModule.UnregisterCustomClassLayout("UWidgetStudioBase");
	}
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FWidgetStudioEditor, WidgetStudioEditor)
