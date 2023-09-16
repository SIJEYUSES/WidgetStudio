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
#include "Widgets/WSButtonBase.h"
#include "Widgets/Basic/WSText.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/InvalidationBox.h"
#include "Components/Overlay.h"
#include "Components/ScaleBox.h"
#include "Components/SizeBox.h"

#include "WSModernSwitch.generated.h"

UENUM(BlueprintType)
enum class ELabelPlacement : uint8
{
	Hide				UMETA(DisplayName="Hide"),				
	Left				UMETA(DisplayName="Left"),
	Right				UMETA(DisplayName="Right"),

	LabelPlacement_Max	UMETA(Hidden),
};

/**
 * A modern styled switch
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UWidgetStudioModernSwitch : public UWidgetStudioButtonBase
{
	GENERATED_BODY()

protected:
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual void SynchronizeProperties() override;
	virtual void InitializeStyling() override;
	virtual void UpdateStyling() override;

	/* Widget Components */

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UInvalidationBox* Retainer = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UHorizontalBox* HBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* Overlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* TrackSizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* TrackScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* TrackOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* TrackDropShadow = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* Track = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* HandleScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* HandleOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* HandleDropShadow = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* Handle = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioText* LabelItem = nullptr;

private:
	
	/* Properties */

	/** Deprecated. Use the Label Widget instead. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Advanced|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	ELabelPlacement LabelPlacement = ELabelPlacement::Hide;

	/** Deprecated. Use the Label Widget instead. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Advanced|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	FText Label = FText().FromString("Switch");
		
	/** The color of the track. */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color")
	EPalette TrackColor = EPalette::TertiaryBackground;

	/** The color of the handle. */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color")
	EPalette HandleColor = EPalette::PrimaryAccent;

	/** Deprecated. Use the Label Widget instead. */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color", Meta = (EditCondition="LabelPlacement != ELabelPlacement::Hide", EditConditionHides))
	EPalette LabelColor = EPalette::PrimaryBackground;

public:
	
	/* Helpers */

	/** Deprecated. Use the Label Widget instead. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Advanced|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	FText GetLabel() const;

	/** Is the label visible? */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Advanced|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	bool IsLabelVisible() const;

	/** Returns the color of the track. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EPalette GetTrackColor() const;
	
	/** Returns the color of the handle. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EPalette GetHandleColor() const;

	/** Deprecated. Use the Label Widget instead. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Advanced|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	EPalette GetLabelColor() const;
	
	/* Modifier Functions */

	/** Deprecated. Use the Label Widget instead. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Advanced|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	void SetLabelPlacement(const ELabelPlacement NewPlacement);

	/** Deprecated. Use the Label Widget instead. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Advanced|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	void SetLabel(FText NewLabel);

	/**
	 * Set the color of the track.
	 * @param NewColor The new color to apply to the track.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetTrackColor(EPalette NewColor);

	/**
	 * Set the color of the handle.
	 * @param NewColor The new color to apply to the handle.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetHandleColor(EPalette NewColor);

	/** Deprecated. Use the Label Widget instead. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Advanced|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	void SetLabelColor(EPalette NewColor);

};
