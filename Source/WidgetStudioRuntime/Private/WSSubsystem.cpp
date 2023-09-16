/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/

#include "WSSubsystem.h"
#include "Misc/ConfigCacheIni.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Modules/ModuleManager.h"
#include "WSGlobals.h"
#include "Engine/Engine.h"

UWidgetStudioSubsystem *UWidgetStudioSubsystem::Instance = nullptr;


UWidgetStudioSubsystem::UWidgetStudioSubsystem()
{
	OnPluginStartedInit.Broadcast();

	// bind to OnAssetUpdated so that, if a data asset is moved, the path of the object (i.e. UWidgetStudioSubsystem::Theme),
	// can be updated, even if no redirectors exist
	IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(TEXT("AssetRegistry")).Get();
	AssetRegistry.OnAssetRenamed().AddUObject(this, &UWidgetStudioSubsystem::HandleAssetRenamed);
	AssetRegistry.OnAssetAdded().AddUObject(this, &UWidgetStudioSubsystem::HandleAssetAdded);

	//set defaults
	Theme = DefaultThemePath;
	IconSet = DefaultIconSetPath;
	Typography = DefaultTypographyPath;

	//if there are values in ini file, override defaults
	LoadValuesFromIni();

	//initial loading of assets
	LoadObject<UWidgetStudioTheme>(nullptr, *Theme.ToString());
	if (!IsValid(Theme.Get()))
	{
		const TSoftObjectPtr<UWidgetStudioTheme> DefaultThemePtr(DefaultThemePath);
		SetTheme(DefaultThemePtr);
	}

	LoadObject<UWidgetStudioIconSet>(nullptr, *IconSet.ToString());
	if (!IsValid(IconSet.Get()))
	{
		const TSoftObjectPtr<UWidgetStudioIconSet> DefaultIconSetPtr(DefaultIconSetPath);
		SetIconSet(DefaultIconSetPtr);
	}

	LoadObject<UWidgetStudioTypography>(nullptr, *Typography.ToString());
	if (!IsValid(Typography.Get()))
	{
		const TSoftObjectPtr<UWidgetStudioTypography> DefaultTypographyPtr(DefaultTypographyPath);
		SetTypography(DefaultTypographyPtr);
	}

	OnStyleChanged.Broadcast();

	//save values to ini file in case any of them are missing
	SaveValuesToIni();
}

UWidgetStudioSubsystem::~UWidgetStudioSubsystem()
{
}

UWidgetStudioSubsystem *UWidgetStudioSubsystem::GetInstance()
{
	if (!Instance)
		Instance = NewObject<UWidgetStudioSubsystem>(UWidgetStudioSubsystem::StaticClass());
	return Instance;
}

UWidgetStudioTheme *UWidgetStudioSubsystem::GetTheme()
{
	if (!GConfig)
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not get GConfig."));
		return nullptr;
	}

	UWidgetStudioTheme* ReturnTheme = nullptr;
	const TCHAR* Section = *WSDefaultGameSection;
	const TCHAR* Key = TEXT("Theme");
	FString ThemeValue;
	const FString IniFile = DefaultGameIni;

	if (GConfig->GetString(Section, Key, ThemeValue, IniFile))
	{
		ReturnTheme = LoadObject<UWidgetStudioTheme>(nullptr, *ThemeValue);
		if (IsValid(ReturnTheme))
		{
			return ReturnTheme;
		}
	}

	// if ini theme is invalid or we couldn't read from ini, set default theme
	UE_LOG(LogWidgetStudio, Warning, TEXT("Current theme not valid.  Setting default theme."));
	ThemeValue = DefaultThemePath;
	const FString Default = DefaultThemePath;
	Theme = Default;
	if (SetTheme(Theme))
	{
		ReturnTheme = LoadObject<UWidgetStudioTheme>(nullptr, *ThemeValue);
	}
	else
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not set default theme."));
	}

	if (!IsValid(ReturnTheme))
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not get theme."));	
	}

	return ReturnTheme;
}

UWidgetStudioIconSet *UWidgetStudioSubsystem::GetIconSet()
{
	if (!GConfig)
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not get GConfig."));
		return nullptr;
	}

	UWidgetStudioIconSet* ReturnIconSet = nullptr;
	const TCHAR* Section = *WSDefaultGameSection;
	const TCHAR* Key = TEXT("IconSet");
	FString IconSetValue;
	const FString IniFile = DefaultGameIni;

	if (GConfig->GetString(Section, Key, IconSetValue, IniFile))
	{
		ReturnIconSet = LoadObject<UWidgetStudioIconSet>(nullptr, *IconSetValue);
		if (IsValid(ReturnIconSet))
		{
			return ReturnIconSet;
		}
	}

	// if ini icon set is invalid or we couldn't read from ini, set default icon set
	UE_LOG(LogWidgetStudio, Warning, TEXT("Current icon set not valid.  Setting default icon set."));
	IconSetValue = DefaultIconSetPath;
	const FString Default = DefaultIconSetPath;
	IconSet = Default;
	if (SetIconSet(IconSet))
	{
		ReturnIconSet = LoadObject<UWidgetStudioIconSet>(nullptr, *IconSetValue);
	}
	else
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not set default icon set."));
	}

	if (!IsValid(ReturnIconSet))
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not get icon set."));	
	}

	return ReturnIconSet;
}

UWidgetStudioTypography *UWidgetStudioSubsystem::GetTypography()
{
	if (!GConfig)
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not get GConfig."));
		return nullptr;
	}

	UWidgetStudioTypography* ReturnTypography = nullptr;
	const TCHAR* Section = *WSDefaultGameSection;
	const TCHAR* Key = TEXT("Typography");
	FString TypographyValue;
	const FString IniFile = DefaultGameIni;

	if (GConfig->GetString(Section, Key, TypographyValue, IniFile))
	{
		ReturnTypography = LoadObject<UWidgetStudioTypography>(nullptr, *TypographyValue);
		if (IsValid(ReturnTypography))
		{
			return ReturnTypography;
		}
	}

	// if ini typography is invalid or we couldn't read from ini, set default typography
	UE_LOG(LogWidgetStudio, Warning, TEXT("Current typography not valid.  Setting default typography."));
	TypographyValue = DefaultTypographyPath;
	const FString Default = DefaultTypographyPath;
	Typography = Default;
	if (SetTypography(Typography))
	{
		ReturnTypography = LoadObject<UWidgetStudioTypography>(nullptr, *TypographyValue);
	}
	else
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not set default typography."));
	}

	if (!IsValid(ReturnTypography))
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not get typography."));	
	}

	return ReturnTypography;
}

int32 UWidgetStudioSubsystem::GetBorderRadius()
{
	const TCHAR *Section = *WSDefaultGameSection;
	const TCHAR *Key = TEXT("BorderRadius");
	int32 Radius;
	const FString IniFile = DefaultGameIni;

	if (!GConfig->GetInt(Section, Key, Radius, IniFile))
	{
		UE_LOG(LogWidgetStudio, Warning, TEXT("Could not read border radius from .ini."));
	}

	return Radius;
}

FVector2D UWidgetStudioSubsystem::GetControlDimensions()
{
	FVector2D ControlDimensions;
	if (GConfig)
	{
		const TCHAR *KeyWidth = TEXT("IdealWidth");
		const TCHAR *KeyHeight = TEXT("IdealHeight");
		const TCHAR *Section = *WSDefaultGameSection;
		float DimX, DimY;
		const FString IniFile = DefaultGameIni;

		if (!GConfig->GetFloat(Section, KeyWidth, DimX, IniFile))
		{
			UE_LOG(LogWidgetStudio, Warning, TEXT("Could not read IdealWidth from .ini."));
		}
		ControlDimensions.X = DimX;

		if (!GConfig->GetFloat(Section, KeyHeight, DimY, IniFile))
		{
			UE_LOG(LogWidgetStudio, Warning, TEXT("Could not read IdealHeight from .ini."));
		}
		ControlDimensions.Y = DimY;
	}
	return ControlDimensions;
}

bool UWidgetStudioSubsystem::SetTheme(const TSoftObjectPtr<UWidgetStudioTheme> InTheme)
{
	LoadObject<UWidgetStudioTheme>(nullptr, *InTheme.ToString());
	if (InTheme.IsValid())
	{
		const TSoftObjectPtr<UWidgetStudioTheme> TempTheme = Theme;
		Theme = InTheme;
		LoadObject<UWidgetStudioTheme>(nullptr, *Theme.ToString());
		if (Theme.IsValid())
		{
			GConfig->SetString(*WSDefaultGameSection, *StringTheme, *Theme.ToString(), DefaultGameIni);
			GConfig->Flush(false, DefaultGameIni);
			OnStyleChanged.Broadcast();
			return true;
		}
		else
		{
			Theme = TempTheme;
			UE_LOG(LogWidgetStudio, Warning, TEXT("Could not load Theme asset"));
			return false;
		}
	}

	UE_LOG(LogWidgetStudio, Warning, TEXT("Theme %s is invalid"), *GetNameSafe(InTheme.Get()));
	return false;
}

bool UWidgetStudioSubsystem::SetIconSet(const TSoftObjectPtr<UWidgetStudioIconSet> InIconSet)
{
	LoadObject<UWidgetStudioIconSet>(nullptr, *InIconSet.ToString());

	if (InIconSet.IsValid())
	{
		const TSoftObjectPtr<UWidgetStudioIconSet> TempIconSet = IconSet;
		IconSet = InIconSet;
		LoadObject<UWidgetStudioIconSet>(nullptr, *IconSet.ToString());

		if (IconSet.IsValid())
		{
			GConfig->SetString(*WSDefaultGameSection, *StringIconSet, *IconSet.ToString(), DefaultGameIni);
			GConfig->Flush(false, DefaultGameIni);
			OnStyleChanged.Broadcast();
			return true;
		}
		else
		{
			IconSet = TempIconSet;
			UE_LOG(LogWidgetStudio, Warning, TEXT("Could not load IconSet asset"));
			return false;
		}
	}

	UE_LOG(LogWidgetStudio, Warning, TEXT("IconSet %s is invalid"), *GetNameSafe(InIconSet.Get()));
	return false;
}

bool UWidgetStudioSubsystem::SetTypography(const TSoftObjectPtr<UWidgetStudioTypography> InTypography)
{
	LoadObject<UWidgetStudioTypography>(nullptr, *InTypography.ToString());

	if (InTypography.IsValid())
	{
		const TSoftObjectPtr<UWidgetStudioTypography> TempTypography = Typography;
		Typography = InTypography;
		LoadObject<UWidgetStudioTypography>(nullptr, *Typography.ToString());

		if (Typography.IsValid())
		{
			GConfig->SetString(*WSDefaultGameSection, *StringTypography, *Typography.ToString(), DefaultGameIni);
			GConfig->Flush(false, DefaultGameIni);
			OnStyleChanged.Broadcast();
			return true;
		}
		else
		{
			Typography = TempTypography;
			UE_LOG(LogWidgetStudio, Warning, TEXT("Could not load Typography asset"));
			return false;
		}
	}

	UE_LOG(LogWidgetStudio, Warning, TEXT("Typography %s is invalid"), *GetNameSafe(InTypography.Get()));
	return false;
}

bool UWidgetStudioSubsystem::SetBorderRadius(int32 InRadius)
{
	BorderRadius = InRadius;
	GConfig->SetInt(*WSDefaultGameSection, *StringBorderRadius, BorderRadius, DefaultGameIni);
	GConfig->Flush(false, DefaultGameIni);
	OnStyleChanged.Broadcast();
	return true;
	//should return false if write fails
}

bool UWidgetStudioSubsystem::SetControlDimensions(const FVector2D InDimensions)
{
	IdealWidth = InDimensions.X;
	IdealHeight = InDimensions.Y;
	GConfig->SetFloat(*WSDefaultGameSection, *StringIdealWidth, IdealWidth, DefaultGameIni);
	GConfig->SetFloat(*WSDefaultGameSection, *StringIdealHeight, IdealHeight, DefaultGameIni);
	GConfig->Flush(false, DefaultGameIni);
	OnStyleChanged.Broadcast();
	return true;
	//should return false if write fails
}

void UWidgetStudioSubsystem::LoadValuesFromIni()
{
	FString ReadString;

	if (GConfig->GetString(*WSDefaultGameSection, *StringTheme, ReadString, DefaultGameIni))
	{
		Theme = ReadString;
	}
	if (GConfig->GetString(*WSDefaultGameSection, *StringIconSet, ReadString, DefaultGameIni))
	{
		IconSet = ReadString;
	}
	if (GConfig->GetString(*WSDefaultGameSection, *StringTypography, ReadString, DefaultGameIni))
	{
		Typography = ReadString;
	}
	GConfig->GetInt(*WSDefaultGameSection, *StringBorderRadius, BorderRadius, DefaultGameIni);
	GConfig->GetFloat(*WSDefaultGameSection, *StringIdealWidth, IdealWidth, DefaultGameIni);
	GConfig->GetFloat(*WSDefaultGameSection, *StringIdealHeight, IdealHeight, DefaultGameIni);
}

void UWidgetStudioSubsystem::SaveValuesToIni() const
{
	GConfig->SetString(*WSDefaultGameSection, *StringTheme, *Theme.ToString(), DefaultGameIni);
	GConfig->SetString(*WSDefaultGameSection, *StringIconSet, *IconSet.ToString(), DefaultGameIni);
	GConfig->SetString(*WSDefaultGameSection, *StringTypography, *Typography.ToString(), DefaultGameIni);
	GConfig->SetInt(*WSDefaultGameSection, *StringBorderRadius, BorderRadius, DefaultGameIni);
	GConfig->SetFloat(*WSDefaultGameSection, *StringIdealWidth, IdealWidth, DefaultGameIni);
	GConfig->SetFloat(*WSDefaultGameSection, *StringIdealHeight, IdealHeight, DefaultGameIni);
}

void UWidgetStudioSubsystem::HandleAssetRenamed(const FAssetData& AssetData, const FString& OldName)
{
	if (!GEngine) return;

	const UWidgetStudioSubsystem* WidgetStudioSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	const auto Asset = AssetData.GetAsset();
	const FString ChangedName = AssetData.GetSoftObjectPath().ToString(); // Updated here

	if (Asset->GetClass() == UWidgetStudioTheme::StaticClass() || Asset->GetClass()->IsChildOf(UWidgetStudioTheme::StaticClass()))
	{
		if (WidgetStudioSubsystem->Theme->GetPathName() == ChangedName || WidgetStudioSubsystem->Theme->GetPathName() == OldName)
		{
			ChangedDataAssetName = ChangedName;
			ChangedDataAssetType = StringTheme;
			bIsDataAssetRenameInProcess = true;
		}
	}
	if (Asset->GetClass() == UWidgetStudioIconSet::StaticClass() || Asset->GetClass()->IsChildOf(UWidgetStudioIconSet::StaticClass()))
	{
		if (WidgetStudioSubsystem->IconSet->GetPathName() == ChangedName || WidgetStudioSubsystem->IconSet->GetPathName() == OldName)
		{
			ChangedDataAssetName = ChangedName;
			ChangedDataAssetType = StringIconSet;
			bIsDataAssetRenameInProcess = true;
		}
	}
	if (Asset->GetClass() == UWidgetStudioTypography::StaticClass() || Asset->GetClass()->IsChildOf(UWidgetStudioTypography::StaticClass()))
	{
		if (WidgetStudioSubsystem->Typography->GetPathName() == ChangedName || WidgetStudioSubsystem->Typography->GetPathName() == OldName)
		{
			ChangedDataAssetName = ChangedName;
			ChangedDataAssetType = StringTypography;
			bIsDataAssetRenameInProcess = true;
		}
	}
}


void UWidgetStudioSubsystem::HandleAssetAdded(const FAssetData& AssetData)
{
	if (!GEngine) return;

	const UWidgetStudioSubsystem* WidgetStudioSubsystem = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();
	if (bIsDataAssetRenameInProcess && AssetData.GetAsset()->GetPathName() == ChangedDataAssetName)
	{
		if (AssetData.GetAsset()->GetClass() == UWidgetStudioTheme::StaticClass() || AssetData.GetAsset()->GetClass()->IsChildOf(UWidgetStudioTheme::StaticClass()))
		{
			const TSoftObjectPtr<UWidgetStudioTheme> ChangedAssetPtr(ChangedDataAssetName);
			SetTheme(ChangedAssetPtr);

			if (IsValid(WidgetStudioSubsystem->Theme.Get()))
			{
				bIsDataAssetRenameInProcess = false;
			}
		}

		if (AssetData.GetAsset()->GetClass() == UWidgetStudioIconSet::StaticClass() || AssetData.GetAsset()->GetClass()->IsChildOf(UWidgetStudioIconSet::StaticClass()))
		{
			const TSoftObjectPtr<UWidgetStudioIconSet> ChangedAssetPtr(ChangedDataAssetName);
			SetIconSet(ChangedAssetPtr);

			if (IsValid(WidgetStudioSubsystem->IconSet.Get()))
			{
				bIsDataAssetRenameInProcess = false;
			}
		}

		if (AssetData.GetAsset()->GetClass() == UWidgetStudioTypography::StaticClass() || AssetData.GetAsset()->GetClass()->IsChildOf(UWidgetStudioTypography::StaticClass()))
		{
			const TSoftObjectPtr<UWidgetStudioTypography> ChangedAssetPtr(ChangedDataAssetName);
			SetTypography(ChangedAssetPtr);

			if (IsValid(WidgetStudioSubsystem->Typography.Get()))
			{
				bIsDataAssetRenameInProcess = false;
			}
		}
	}
}

void UWidgetStudioSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
	bIsPluginInitialized = true;
	OnPluginFinishedInit.Broadcast();
}

void UWidgetStudioSubsystem::Deinitialize()
{
}
