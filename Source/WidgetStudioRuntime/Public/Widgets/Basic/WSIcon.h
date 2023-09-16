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

#include "Widgets/WSBase.h"
#include "Components/Image.h"
#include "Components/InvalidationBox.h"
#include "Components/ScaleBox.h"
#include "Components/SizeBox.h"

#include "WSIcon.generated.h"

/**
 * The settings for the Widget Studio Icon Widget
 */
USTRUCT(BlueprintType)
struct FWSIconStyle
{
	GENERATED_BODY()
	
	/** The size of the icon. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Studio", Meta = (ClampMin = "16", UIMin = "16"))
	float Size;

	/** Will use the icons native color when enabled. Otherwise, will resort to using the designated palette color. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget Studio|Color")
	bool bUseNativeColor;

	explicit FWSIconStyle(
		const float InSize = 16,
		const bool bInUseNativeColor = false
		)
	{
		Size = InSize;
		bUseNativeColor = bInUseNativeColor;
	}
};

/**
 * A pre-setup widget that works with power of two icons. For example, 64x64.
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UWidgetStudioIcon : public UWidgetStudioBase
{
	GENERATED_BODY()

protected:
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual void SynchronizeProperties() override;
	virtual void InitializeStyling() override;
	virtual void UpdateStyling() override;

	// Widget Components

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UInvalidationBox* Retainer = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* SizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* ScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* IconItem = nullptr;

private:
	/* Properties  */

	/** Deprecated. Use IconStyle instead. */
	UPROPERTY(EditAnywhere, Category="Widget Studio", Meta = (EditCondition="CustomIcon == nullptr"))
	EIconItem Icon = EIconItem::Heart;

	/** Use IconStyle instead. */
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	UTexture2D* CustomIcon;

	/** The size of the icon. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta = (ClampMin = "16", UIMin = "16"))
	float Size = 24;

	/** Will use the icons native color when enabled. Otherwise, will resort to using the designated palette color. */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color")
	bool bUseNativeColor = false;

	/** Settings to adjust the icon visual settings. */
	UPROPERTY()
	FWSIconStyle IconStyle;

	/** The color of the icon. */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color")
	EPalette Color = EPalette::PrimaryContent;

	


public:
	/* Helpers */

	/** Returns the icon used from the icon library. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EIconItem GetIcon() const;

	/** Returns the color used for the icon. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EPalette GetColor() const;

	/** Returns true if a custom icon has been set and is being used. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsUsingCustomIcon() const;

	/** Returns the custom icon. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	UTexture2D* GetCustomIcon() const;

	/** Returns the size of the icon. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float GetSize() const;

	/** Returns true if the icon is using it's native color. False if using the palette color. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float IsUsingNativeColor() const;

	/* Modifiers */

	/**
	 * Sets the icon to use an icon from the Icon Set.
	 * @param NewIcon The icon to apply to the icon widget.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetIcon(EIconItem NewIcon);

	/**
	 * Sets the color of the icon.
	 * @param NewColor The color to apply to the icon widget.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetColor(EPalette NewColor);

	/**
	 * Sets the icon to use a custom icon.
	 * @param NewCustomIcon The texture2D icon to apply to the icon widget.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetCustomIcon(UTexture2D* NewCustomIcon);

	/** Clears the set custom icon and automatically switches the widget to use the icon library. */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void ClearCustomIcon();

	/**
	 * Sets the size of the icon.
	 * @param NewSize The size to apply to the icon widget.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetSize(float NewSize);

	/**
	 * Set to true to use the icons native color. Set to false to use the palette color.
	 * @param NewState The state to apply to use the icons own color (true), or to use the palette color (false).
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetUseNativeColor(bool NewState);

	/**
	 * Set the icon styling.
	 * @param NewOptions The new icon styling to apply.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetIconStyle(const FWSIconStyle& NewOptions);
};
