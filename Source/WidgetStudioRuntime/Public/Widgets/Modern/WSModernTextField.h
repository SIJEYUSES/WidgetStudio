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

#include "WSModernCard.h"
#include "Components/EditableText.h"
#include "Widgets/WSBase.h"
#include "Components/HorizontalBox.h"
#include "Components/InvalidationBox.h"
#include "Widgets/Custom/CustomMultiLineTextBox.h"
#include "Components/Overlay.h"
#include "Components/ScaleBox.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "Widgets/WSTextFieldBase.h"
#include "Widgets/Custom/WSEditableText.h"
#include "WSModernTextField.generated.h"

/**
 * A modern styled text field
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UWidgetStudioModernTextField : public UWidgetStudioTextFieldBase
{
	GENERATED_BODY()

protected:
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual void SynchronizeProperties() override;
	virtual void InitializeStyling() override;
	virtual void UpdateStyling() override;

	UFUNCTION()
	EPalette GetColorByState() const;

	UPROPERTY()
	FSlateBrush ScrollBarBrush;
	
	/* Widget Components */

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UInvalidationBox* Retainer = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UVerticalBox* VBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UTextBlock* LabelItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* SizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* Overlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* BackgroundScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* BackgroundOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* DropShadow = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* Background = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* BackgroundOutline = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UHorizontalBox* HBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* LeadingIconScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* LeadingIconItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* TrailingIconScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* TrailingIconItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWSEditableText* TextInput = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UCustomMultiLineTextBox* TextInputMultiLine = nullptr;


private:

	// Properties

	/** Set the maximum dimensions of the Text Field when MultiLine is enabled. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", meta = (ClampMin = "0", UIMin = "0"), AdvancedDisplay)
	FVector2D MaxDimensions = FVector2D(0, 0);
	
	/** The style of the corners. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style")
	ECornerStyle CornerStyle = ECornerStyle::Rounded;
	
	/* Icons */

	/** Set the visibility of the leading icon. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Icon")
	ETextFieldIconVisibility IconVisibility = ETextFieldIconVisibility::Hidden;
	
	/** The icon displayed on the left side. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Icon")
	EIconItem LeadingIcon = EIconItem::LockClose;

	/** The icon displayed on the right side. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Icon")
	EIconItem TrailingIcon = EIconItem::Heart;

	/** Adjust the size of the icons. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Icon", Meta = (ClampMin = "12", UIMin = "12"))
	float IconSize = 16;
	
	
	/* Colors */

	/** The color of the background. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette BackgroundColor = EPalette::TertiaryBackground;

	/** The color of the outline when in a normal state. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette DefaultOutline = EPalette::Transparent;

	/** The color of the content (Text/Icon). */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette ContentColor = EPalette::PrimaryContent;
	
	/** Deprecated. Use the Label Widget instead. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	EPalette LabelColor = EPalette::PrimaryContent;

public:
	
	/* Helpers */

	/** Returns if multi-line support is enabled or not. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	bool IsMultiLineSupportEnabled() const;

	/** Returns the current icon visibility setting. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	ETextFieldIconVisibility GetIconVisibility() const;

	/** Returns the leading icon. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	EIconItem GetLeadingIcon() const;

	/** Returns the trailing icon. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	EIconItem GetTrailingIcon() const;
	
	/** Returns the background color. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	EPalette GetBackgroundColor() const;

	/** Returns the background color. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	EPalette GetContentColor() const;

	/** Returns the icon size */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	float GetIconSize() const;

	/* Modifiers */

	/**
	 * Enable or disable the multi-line support.
	 * @param bNewState The new visibility state of the icon.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetMultiLineSupportEnabled(bool bNewState);

	/**
	 * Sets the icon visibility.
	 * @param NewState The new visibility state of the icon.
	 */
    UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
    void SetIconVisibility(ETextFieldIconVisibility NewState);

	/**
	 * Sets the leading icon.
	 * @param NewIcon The new icon to display to the left of the Text Field.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetLeadingIcon(EIconItem NewIcon);

	/**
	 * Sets the trailing icon.
	 * @param NewIcon The new icon to display to the right of the Text Field.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetTrailingIcon(EIconItem NewIcon);

	/**
	 * Set the background color.
	 * @param NewColor The new color to apply to the background of the widget.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetBackgroundColor(EPalette NewColor);

	/**
	 * Set the content color.
	 * @param NewColor The new color to apply to the content of the widget.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetContentColor(EPalette NewColor);

	/**
	 * Set the icon size.
	 * @param NewSize The new size to apply to the icon.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetIconSize(float NewSize);
};
