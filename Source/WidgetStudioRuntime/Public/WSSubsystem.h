/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/

#pragma once

#include "CoreMinimal.h"

/* used for log category - do not remove */
#include "WidgetStudioRuntime.h"

#include "Subsystems/EngineSubsystem.h"
#include "Theme/WSIconSet.h"
#include "Theme/WSTheme.h"
#include "Theme/WSTypography.h"
#include "WSSubsystem.generated.h"

extern int32 DefaultBorderRadius;
extern float DefaultIdealWidth;
extern float DefaultIdealHeight;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStyleDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FStartInitDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndInitDelegate);

//To access:  UWidgetStudioSubsystem* WidgetStudio = GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>();

/**
 * A singleton for managing widget styles and managing widget state instantiates when the plugin loads.
 */
UCLASS(Blueprintable, BlueprintType, Category = "Widget Studio")
class WIDGETSTUDIORUNTIME_API UWidgetStudioSubsystem final : public UEngineSubsystem
{
	GENERATED_BODY()

public:
	/*
	 * Settings
	 */

	UPROPERTY(EditAnywhere, Category = "Theme", Meta = (HideViewOptions, DisplayThumbnail = "false", ToolTip = "Choose the current Widget Studio theme"))
	TSoftObjectPtr<UWidgetStudioTheme> Theme;

	UPROPERTY(EditAnywhere, Category = "Typography", Meta = (HideViewOptions, ToolTip = "Choose the font/type look and feel"))
	TSoftObjectPtr<UWidgetStudioTypography> Typography;

	UPROPERTY(EditAnywhere, Category = "Icon Set", Meta = (HideViewOptions, ToolTip = "Choose the icon style"))
	TSoftObjectPtr<UWidgetStudioIconSet> IconSet;

	UPROPERTY(EditAnywhere, Category = "Style", Meta = (ToolTip = "The corner roundness found on controls, such as buttons, comboboxs, etc."))
	int32 BorderRadius = DefaultBorderRadius;

	UPROPERTY(EditAnywhere, Category = "Style", Meta = (ToolTip = "The default x dimension of controls if not overridden via layouts."))
	float IdealWidth = DefaultIdealWidth;

	UPROPERTY(EditAnywhere, Category = "Style", Meta = (ToolTip = "The default y dimension of controls if not overridden via layouts."))
	float IdealHeight = DefaultIdealHeight;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, BlueprintCallable, Category = "Widget Studio|Events")
	FStyleDelegate OnStyleChanged;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, BlueprintCallable, Category = "Widget Studio|Inititalization")
	FStartInitDelegate OnPluginStartedInit;

	UPROPERTY(BlueprintAssignable, BlueprintReadWrite, BlueprintCallable, Category = "Widget Studio|Initialization")
	FEndInitDelegate OnPluginFinishedInit;

	// Begin BP Interface

	/**
	* Make sure current Theme DataAsset is loaded and valid, then retrieve it
	* @return the UWidgetStudioTheme*
	*/
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Style")
	UWidgetStudioTheme *GetTheme();

	/**
	* Make sure current IconSet DataAsset is loaded and valid, then retrieve it
	* @return the UWidgetStudioIconSet*
	*/
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Style")
	UWidgetStudioIconSet *GetIconSet();

	/**
	* Make sure current Typography DataAsset is loaded and valid, then retrieve it
	* @return the UWidgetStudioTypography*
	*/
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Style")
	UWidgetStudioTypography *GetTypography();

	/**
	* Get current border radius value
	* @return BorderRadius
	*/
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Style")
	static int32 GetBorderRadius();

	/**
	* Get current control dimensions x and y values as FVector2D
	* @return ControlDimensions
	*/
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Style")
	static FVector2D GetControlDimensions();

	/**
	* Sets the theme (colors) for widgets -- checks to make sure the incoming Theme is valid and loaded, then updates
	* the current Theme, including the "Theme" value in the .ini file. Broadcasts OnStyleChanged() event.
	* @pre Theme passed in must point to a valid data asset
	* @post Passed in Theme data asset will be loaded (if not already) and applied as the global theme
	* @post OnStyleChanged event will be broadcast, so Editor module WS Settings are updated
	* @param InTheme - the Theme to set
	* @return true if Theme is successfully set, false otherwise 
	*/
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Style")
	bool SetTheme(TSoftObjectPtr<UWidgetStudioTheme> InTheme);

	/**
	* Sets the IconStyle for widgets -- checks to make sure the incoming IconSet is valid and loaded, then updates
	* the current IconSet, including the "IconSet" value in the .ini file. Broadcasts OnStyleChanged() event.
	* @pre IconSet passed in must point to a valid data asset
	* @post Passed in IconSet data asset will be loaded (if not already) and applied as the global icon set
	* @post OnStyleChanged event will be broadcast, so Editor module WS Settings are updated
	* @param InIconSet - the IconSet to set
	* @return true if IconSet is successfully set, false otherwise
	*/
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Style")
	bool SetIconSet(TSoftObjectPtr<UWidgetStudioIconSet> InIconStyle);

	/**
	* Sets the typography (font settings) for widgets -- checks to make sure the incoming Typography is valid and loaded, then updates
	* the current Typography, including the "Typography" value in the .ini file. Broadcasts OnStyleChanged() event.
	* @pre Typography passed in must point to a valid data asset
	* @post Passed in Typography data asset will be loaded (if not already) and applied as the global typography
	* @post OnStyleChanged event will be broadcast, so Editor module WS Settings are updated
	* @param InTypography - the Typography to set
	* @return true if Typography is successfully set, false otherwise
	*/
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Style")
	bool SetTypography(TSoftObjectPtr<UWidgetStudioTypography> InTypography);

	/**
	* Sets the border radius for widgets -- updates BorderRadius, oncluding "BorderRadius" value in the .ini file.
	* Broadcasts OnStyleChanged() event.
	* @param InRadius
	* @return true if border radius is successfully set, false otherwise
	*/
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Style")
	bool SetBorderRadius(int32 InRadius);

	/**
	* Sets the control dimensions for widgets -- updates ControlDimensions, oncluding "ControlDimensions" value in the .ini file.
	* Broadcasts OnStyleChanged() event.
	* @param InDimensions
	* @return true if control dimensions are successfully set, false otherwise
	*/
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Style")
	bool SetControlDimensions(FVector2D InDimensions);

	UFUNCTION(BlueprintPure, Category = "Widget Studio|Initialization")
	bool IsPluginInitialized() const
	{
		return bIsPluginInitialized;
	}
	//End BP Interface

	// Begin USubsystem
	virtual void Initialize(FSubsystemCollectionBase &Collection) override;
	virtual void Deinitialize() override;
	// End USubsystem

	/**
	* Get WidgetStudioSingleton instance.  Create if non-existent.
	* @return the instance
	*/
	static UWidgetStudioSubsystem *GetInstance();

private:
	friend class UWidgetStudioTestClass;

	/**
	* constructor -- initialize with default values in case values are not contained in config file.  Then load values from config file (replaces defaults).
	* Then save values to config file in case any did not exist there previously.
	*/
	UWidgetStudioSubsystem();
	
	virtual ~UWidgetStudioSubsystem() override;

	/**
	 * retrieve current Theme, IconSet, Typography, BorderRadius, IdealWidth, IdealHeight
	 * from default .ini file
	 */
	void LoadValuesFromIni();

	/**
	 * save current Theme, IconSet, Typography, BorderRadius, IdealWidth, IdealHeight
	 * to default .ini file
	 */
	void SaveValuesToIni() const;

	/**
	* Keeps Theme, IconSet, Typography valid if data assets are renamed
	* @param AssetData Asset data which includes the asset with the new name
	* @param OldName The name before the change
	*/
	void HandleAssetRenamed(const FAssetData& AssetData, const FString& NewName);
	
	void HandleAssetAdded(const FAssetData& AssetData);

	bool bIsPluginInitialized = false;
	bool bIsDataAssetRenameInProcess = false;

	FString ChangedDataAssetName = TEXT("");
	FString ChangedDataAssetType = TEXT("");

	static UWidgetStudioSubsystem *Instance;
};
