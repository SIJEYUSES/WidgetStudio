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
#include "Widgets/Modern/WSModernButton.h"

#include "Widgets/WSContainer.h"
#include "Widgets/Modern/WSModernTabBar.h"
#include "Components/InvalidationBox.h"
#include "Components/MenuAnchor.h"

#include "WSModernComboBox.generated.h"

/**
 * A modern styled combo box
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UWidgetStudioModernComboBox : public UWidgetStudioContainer
{
	GENERATED_BODY()

protected:
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual void SynchronizeProperties() override;
	virtual void InitializeStyling() override;
	virtual void UpdateStyling() override;
	
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

	UFUNCTION()
	void OnMenuButtonSelected(int32 Index, FButtonOptions Option);

	UFUNCTION()
	UUserWidget* ConstructMenu() const;

	/* * Widget Components */

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UInvalidationBox* Retainer = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UVerticalBox* VBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioText* LabelItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* ContentSizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* ContentScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* ContentOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* DropShadow = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* Background = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UHorizontalBox* HBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioIcon* IconItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioText* TextItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* ArrowScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioIcon* ArrowItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UMenuAnchor* MenuAnchor = nullptr;

private:

	// General Properties

	/** Deprecated. Use the Label Widget instead */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Advanced|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	FText Label;

	/** The text displayed when an option isn't selected (Current index = -1) */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	FText PlaceholderText = FText().FromString("Select option");

	/** How the content should be horizontally aligned. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	TEnumAsByte<EHorizontalAlignment> ContentAlignment = HAlign_Left;

	/** The text styling options for the container. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Text", meta = (DisplayName = "Text Style"))
	FWSTextStyle TextStyle = FWSTextStyle(
		EWSFontType::Button,
		FFontStyle(),
		EFontCase::Sentence,
		ETextJustify::Center,
		false,
		0,
		ETextWrappingPolicy::DefaultWrapping);

	/**
	 * Where the icons should be placed in conjunction with the text.
	 * This is ignored for options set to Icon Only.
	 */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Icon")
	EWSPlacement IconPlacement;

	/** The icon styling options for the container. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Icon", meta = (DisplayName = "Icon Style"))
	FWSIconStyle IconStyle;
	
	// Colors Properties

	/** The color of the background. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette BackgroundColor = EPalette::TertiaryBackground;

	/** The color of the content. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette ContentColor = EPalette::PrimaryContent;

	/** The color of the overlay. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette SelectionColor = EPalette::PrimaryAccent;

	/** Deprecated. Use the Label Widget instead. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Advanced|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	EPalette LabelColor = EPalette::PrimaryContent;

	
	// Style Properties
	
	/** How the corners of the button will appear visually. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style")
	ECornerStyle CornerStyle = ECornerStyle::Rounded;

	// Scroll Properties
	
	/** The maximum height of the dropdown menu. If the number of options exceeds this height, the menu will become scrollable. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style")
	float MenuHeight = 225;
	
	// State Properties
	
	/** Are the options in this Combo Box selectable? Disabling this option will have the Combo Box act as a drop down menu instead. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|State")
	bool bIsSelectable = true;


	
public:
	// Overrides
	virtual void SetCurrentIndex(int32 Index, bool bBroadcast) override;

	// Helpers

	/** Returns the label's text. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Advanced|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	FText GetLabel() const;

	/** Returns the placeholder's text. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	FText GetPlaceholderText() const;

	/** Returns the corner style. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	ECornerStyle GetCornerStyle() const;
	
	/** Returns the background color. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Color")
	EPalette GetBackgroundColor() const;

	/** Returns the content color (Text/Icon). */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Color")
	EPalette GetContentColor() const;

	/** Returns the selection color. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Color")
	EPalette GetSelectionColor() const;

	/** Returns the label color. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Advanced|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	EPalette GetLabelColor() const;

	/** Are the Combo Box items selectable? */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|Color")
	bool IsSelectable() const;

	
	// Modifiers

	/**
	 * Sets the label's text.
	 * @param NewLabel The text to display on the label.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Advanced|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	void SetLabel(FText NewLabel);

	/**
	 * Sets the placeholder's text.
	 * @param NewText The text to display as the placeholder.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetPlaceholderText(FText NewText);

	/**
	 * Sets the how the corners look.
	 * @param NewStyle The style to use for the widgets corners.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetCornerStyle(ECornerStyle NewStyle);

	/**
	 * Set the background color.
	 * @param NewColor The color to apply to the widgets background.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetBackgroundColor(EPalette NewColor);

	/**
	 * Set the content color.
	 * @param NewColor The color to apply to the widgets content.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetContentColor(EPalette NewColor);

	/**
	 * Set the selection color.
	 * @param NewColor The color to apply to the widget when selected.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetSelectionColor(EPalette NewColor);

	/**
	 * Set the label color.
	 * @param NewColor The color to apply to the label.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Advanced|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	void SetLabelColor(EPalette NewColor);

	/**
	 * Set the selectable state of the Combo Box.
	 * @param NewState The selectivity state.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetSelectable(bool NewState);

};

