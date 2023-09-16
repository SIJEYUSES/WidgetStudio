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
#include "UObject/Object.h"
#include "Theme/WSTheme.h"
#include "Theme/WSTypography.h"
#include "Theme/WSIconSet.h"
#include "WSSettings.generated.h"

extern int32 DefaultBorderRadius;
extern float DefaultIdealWidth;
extern float DefaultIdealHeight;

/**
 * The Widget Studio editor settings.
 */
UCLASS(config = Game, defaultconfig)
class WIDGETSTUDIOEDITOR_API UWidgetStudioSettings final : public UObject
{
	GENERATED_BODY()
	UWidgetStudioSettings();

	/**
	* Handler for WidgetStudio Project Settings menu actions
	* @param PropertyChangedEvent - FPropertyChangedEvent
	*/
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

public:
	explicit UWidgetStudioSettings(const FObjectInitializer& ObjectInitializer);

	/* Theme Settings */

	/** Choose the current Widget Studio theme */
	UPROPERTY(Config, EditAnywhere, Category = "Theme", Meta = (HideViewOptions, DisplayThumbnail = "false"))
	TSoftObjectPtr<UWidgetStudioTheme> Theme;

	/**
	 * Create a new custom theme to use with the Widget Studio.
	 * By default, this theme will be stored in '/Game/Widget Studio/Themes'
	 */
	UPROPERTY(EditAnywhere, Category = "Theme", Meta = (DisplayName = "Create New Theme"))
	bool CreateThemeButton = false;

	UPROPERTY(VisibleAnywhere, Category = "Theme|Preview", Meta = (HideAlphaChannel = "true"))
	FLinearColor PrimaryAccent;

	UPROPERTY(VisibleAnywhere, Category = "Theme|Preview", Meta = (HideAlphaChannel = "true"))
	FLinearColor SecondaryAccent;

	UPROPERTY(VisibleAnywhere, Category = "Theme|Preview", Meta = (HideAlphaChannel = "true"))
	FLinearColor PrimaryContent;

	UPROPERTY(VisibleAnywhere, Category = "Theme|Preview", Meta = (HideAlphaChannel = "true"))
	FLinearColor SecondaryContent;

	UPROPERTY(VisibleAnywhere, Category = "Theme|Preview", Meta = (HideAlphaChannel = "true"))
	FLinearColor TertiaryContent;

	UPROPERTY(VisibleAnywhere, Category = "Theme|Preview", Meta = (HideAlphaChannel = "true"))
	FLinearColor PrimaryBackground;

	UPROPERTY(VisibleAnywhere, Category = "Theme|Preview", Meta = (HideAlphaChannel = "true"))
	FLinearColor SecondaryBackground;

	UPROPERTY(VisibleAnywhere, Category = "Theme|Preview", Meta = (HideAlphaChannel = "true"))
	FLinearColor TertiaryBackground;

	
	/* Typography Settings */

	/** Choose the font/type look and feel. */
	UPROPERTY(Config, EditAnywhere, Category = "Typography", Meta = (HideViewOptions, DisplayThumbnail = "false"))
	TSoftObjectPtr<UWidgetStudioTypography> Typography;

	/**
	 * Create a new custom typography to use with the Widget Studio.
	 * By default, this theme will be stored in '/Game/Widget Studio/Typography'
	 */
	UPROPERTY(EditAnywhere, Category = "Typography", Meta = (DisplayName = "Create New Typography"))
	bool CreateTypographyButton = false;

	
	/* Icon Settings */

	/** Choose the icon style. */ 
	UPROPERTY(Config, EditAnywhere, Category = "Icon Set", Meta = (HideViewOptions, DisplayThumbnail = "false"))
	TSoftObjectPtr<UWidgetStudioIconSet> IconSet;

	/**
	 * Create a new custom icon set to use with the Widget Studio.
	 * By default, this theme will be stored in '/Game/Widget Studio/IconSet'
	 */
	UPROPERTY(EditAnywhere, Category = "Icon Set", Meta = (DisplayName = "Create New Icon Set"))
	bool CreateIconSetButton = false;

	
	/* Style Settings */

	/** The corner roundness found on controls, such as buttons, combo boxes, etc. */
	UPROPERTY(Config, EditAnywhere, Category = "Style", Meta = (ClampMin = "0", UIMin = "0", ClampMax = "45", UIMax = "45"))
	int32 BorderRadius = DefaultBorderRadius;

	/** The ideal minimum width for certain widgets --- i.e: Slider, Progress Bar, and Text Field */
	UPROPERTY(Config, EditAnywhere, Category = "Style", Meta = (ClampMin = "75", UIMin = "75", ClampMax = "1000", UIMax = "1000"))
	float IdealWidth = DefaultIdealWidth;

	/** The ideal minimum height for certain widgets --- i.e: Slider, Progress Bar, and Text Field. */
	UPROPERTY(Config, EditAnywhere, Category = "Style", Meta = (ClampMin = "35", UIMin = "35", ClampMax = "1000", UIMax = "1000"))
	float IdealHeight = DefaultIdealHeight;

private:
	/**
	* Updates theme preview from current theme
	*/
	UFUNCTION()
	void UpdateThemePreview();

	/**
	* Creates new theme DataAsset of type UWidgetStudioTheme
	* @param Filename - for new asset
	* @param PackageName - name of package for new asset
	*/
	void CreateNewThemeDataAssetObject(const FString FileName, const FString PackageName) const;

	/**
	* Creates new typography DataAsset of type UWidgetStudioTypography
	* @param Filename - for new asset
	* @param PackageName - name of package for new asset
	*/
	void CreateNewTypographyDataAssetObject(const FString FileName, const FString PackageName) const;

	/**
	* Creates new icon set DataAsset of type UWidgetStudioIconSet
	* @param Filename - for new asset
	* @param PackageName - name of package for new asset
	*/
	void CreateNewIconSetDataAssetObject(const FString FileName, const FString PackageName) const;

};
