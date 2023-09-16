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
#include "Widgets/Basic/WSText.h"

#include "Components/EditableTextBox.h"
#include "Components/EditableText.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/Overlay.h"
#include "Components/ScaleBox.h"
#include "Components/SizeBox.h"
#include "Components/Slider.h"
#include "Components/VerticalBox.h"
#include "Types/SlateEnums.h"
#include "Types/WSEnums.h"
#include "Widgets/WSSliderBase.h"
#include "WSModernSlider.generated.h"

/**
 * A modern styled slider. Useful for setting a value between a minimum and a maximum.
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UWidgetStudioModernSlider : public UWidgetStudioSliderBase
{
	GENERATED_BODY()

protected:
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual void SynchronizeProperties() override;
	virtual void InitializeStyling() override;
	virtual void UpdateStyling() override;

	/* Delegates */

	UFUNCTION()
	void OnTextCommitted(const FText& InText, ETextCommit::Type InCommitMethod);

	/* Widget Components */

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UVerticalBox* VBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* SizeBox = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UHorizontalBox* HBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* Overlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* TrackScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* TrackOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* Track = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* TrackDropShadow = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* FillTrackSizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* FillTrack = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* HandleScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* HandleSizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* HandleOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* HandleDropShadow = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* Handle = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USlider* SliderItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* TextInputSizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UEditableTextBox* TextInput = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioText* LabelItem = nullptr;

private:
	/* Internal Functions */

	/** Calls SetValue when built-in slider is changed. No NOT touch. */
	UFUNCTION()
	void SyncSliderValue(float NewValue);
	
	/* Properties */

	/** Deprecated. Use the Label Widget instead. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Advanced|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	FText Label;

	/** Allow users to interact with the slider. */
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	bool bEnableInput = true;

	/** Display the handle of the slider. */
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	bool bDisplayHandle = true;

	/** Display the value next to the slider. */
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	bool bDisplayValue = true;
	

	/* Color */

	/** Instead of using the accent color, the slider color will be based on thresholds. [0] Friendly -> Hostile [1] */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color")
	bool bColorBasedOnValue = false;

	/** Reverses the color based on value's order. Changes to [0] Hostile -> Friendly [1] */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color", Meta = (EditCondition="bColorBasedOnValue == true", EditConditionHides))
	bool bReverseColorOrder = false;
	
	/** The color of the slider track. */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color")
	EPalette TrackColor = EPalette::TertiaryBackground;

	/** The color of the slider handle. */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color", Meta = (EditCondition="bColorBasedOnValue == false", EditConditionHides))
	EPalette HandleColor = EPalette::PrimaryAccent;

	/** The color of both the Label and the Value. */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color", Meta = (EditCondition="bDisplayValue == true", EditConditionHides))
	EPalette TextColor = EPalette::PrimaryContent;

public:

	
	/* Helpers */

	/** Deprecated. Use the Label Widget instead. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Advanced|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	FText GetLabelText() const;
	
	/** Is the value visible? */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsValueVisible() const;

	/** Is the handle visible? */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsHandleVisible() const;

	/** Returns true if the color is based on value. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsColorBasedOnValue() const;

	/** Returns true if the color based on value order is reversed. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsColorBasedOnValueReversed() const;
	
	/** Can the user interact with the slider? */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsInputEnabled() const;
	
	
	/* Modifier Functions */

	/** Deprecated. Use the Label Widget instead.*/
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Advanced|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	void SetLabelText(FText NewText);
	
	/**
	 * Set the values visibility.
	 * @param bNewState The new visibility state of the value. Enable to display the value. Disable to hide the value and have the slider fill in the remaining space.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetValueVisibility(bool bNewState);

	/**
	 * Set the handles visibility.
	 * @param bNewState The new visibility state of the handle. Enable to display the handle. Disable to hide the handle. Functionality remains the same.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetHandleVisibility(bool bNewState);

	/**
	 * Set the color to be based on value (true) or to use the handle color (false).
	 * @param bNewState The new color based on value state. Enable to have the color change based on the current value compared to the min/max. Disable to have the color controlled by the set palette.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetColorBasedOnValue(bool bNewState);

	/**
	 * Reverses the color based on value's order. Changes to [0] Hostile -> Friendly [1].
	 * @param bNewState The new color order state. Enable to reverse the order of the color based on value range.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetReverseColorOrder(bool bNewState);
	
	/**
	 * Allow or disallow the user to interact with the slider.
	 * @param NewState The new input state. Enable to allow user input. Disable to block user input.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetInputState(bool NewState);
};
