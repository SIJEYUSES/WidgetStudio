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

#include "Widgets/Utility/WSButtonGroup.h"
#include "Widgets/WSContainer.h"
#include "Components/UniformGridPanel.h"
#include "Widgets/Panel/WSScrollBox.h"

#include "WSModernTabBar.generated.h"

UENUM(BlueprintType)
enum class ETabBarSelectionStyle : uint8
{
	Full						UMETA(DisplayName="Full"),
	Slim						UMETA(DisplayName="Slim"),
	Hidden						UMETA(DisplayName="Hidden"),

	TabBarSelectionStyle_Max	UMETA(Hidden),
};

/**
 * A modern styled tab bar. Useful for multi-tab navigation or selecting an option from many.
 */

UCLASS()
class WIDGETSTUDIORUNTIME_API UWidgetStudioModernTabBar : public UWidgetStudioContainer
{
	GENERATED_BODY()

protected:
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual void SynchronizeProperties() override;
	virtual void InitializeStyling() override;
	virtual void UpdateStyling() override;
	virtual void ConstructOption(FButtonOptions Option) override;
	virtual void ClearOptions() override;
	virtual void ConstructButtonGroup();

	UFUNCTION()
	void UpdateIndexFromButtonGroup(int32 NewIndex);
	
	
	// Widget Components

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UInvalidationBox* Retainer = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* SizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWSScrollBox* ScrollBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* Overlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioButtonGroup* ButtonGroup = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* BackgroundScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* BackgroundOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* BackgroundDropShadow = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* Background = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* SelectionOverlaySize = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* SelectionOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UUniformGridPanel* Grid = nullptr;

	// Properties

	
	/** How the content should be horizontally aligned. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	TEnumAsByte<EHorizontalAlignment> ContentAlignment;

	/** The direction to grow the bar.
	 * Requires a Re-Compile to take effect.
	 */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	TEnumAsByte<EOrientation> Orientation = Orient_Horizontal;
	
	/** The text styling options for the container. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Text")
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
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Icon")
	FWSIconStyle IconStyle;

	


	// Scroll Properties
	
	/**
	 * When enabled, the Tab Bar will be scrollable. 
	 * Scrollability starts when the Tab Bar's dimensions exceeds the parent widgets or set Size Constraints.
	 * 
	 * When enabled, the Tab Bar cannot fill the parent widgets size and will be dependent on it's own dimensions.
	 * Will require a re-compile of the Widget Blueprint to take effect.
	 */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Scrolling")
	bool bEnableScrolling = false;
	
	/** Constrains the size of the Tab Bar.
	 * - If the tab bar extends beyond the giving size constraint, the tab bar will become scrollable.
	 * - Size and scroll direction is dependent on set orientation.
	 * - If set to 0, or Horizontal/Vertical Alignment set to Align Fill, size constraint will be ignored.
	 */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Scrolling", Meta = (ClampMin = "0", UIMin = "0", EditCondition="bEnableScrolling", EditConditionHides))
	float SizeConstraint = 0.f;

	/** Set the scroll visibility of the scroll bar. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Scrolling", Meta = (EditCondition="bEnableScrolling", EditConditionHides))
	ESlateVisibility ScrollBarVisibility = ESlateVisibility::Visible;

	// Style Properties

	/** Enables the drop shadow effect when hovered. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style")
	bool bEnableDropShadow = true;

	/** How the corners of the button will appear visually. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style")
	ECornerStyle CornerStyle = ECornerStyle::Rounded;

	/** The style of the selection indicator. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style")
	ETabBarSelectionStyle SelectionStyle = ETabBarSelectionStyle::Full;


	// Color Properties

	/** The color of the background. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette BackgroundColor = EPalette::TertiaryBackground;

	/** The color of the content. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette ContentColor = EPalette::PrimaryContent;

	/** The color of the selection. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette SelectionColor = EPalette::PrimaryAccent;


	// State Properties

	/** Set if the tab bar is selectable or not. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|State")
	bool bSelectable = true;

public:

	// Helpers

	/** Returns the content alignment. */
	UFUNCTION(BlueprintGetter, Category = "Widget Studio")
	TEnumAsByte<EHorizontalAlignment> GetContentAlignment() const;

	/** Returns the text style options. */
	UFUNCTION(BlueprintGetter, Category = "Widget Studio")
	FWSTextStyle GetTextOptions() const;

	/** Returns the icon style options. */
	UFUNCTION(BlueprintGetter, Category = "Widget Studio")
	FWSIconStyle GetIconOptions() const;

	/** Returns the icon placement. */
	UFUNCTION(BlueprintGetter, Category = "Widget Studio")
	EWSPlacement GetIconPlacement() const;
	
	/** Returns the Tab Bar orientation. */
	UFUNCTION(BlueprintGetter, Category="Widget Studio|Helper")
	TEnumAsByte<EOrientation> GetOrientation() const;

	/** Returns the Tab Bar corner style. */
	UFUNCTION(BlueprintGetter, Category="Widget Studio|Helper")
	ECornerStyle GetCornerStyle() const;

	/** Returns the Tab Bar selection style. */
	UFUNCTION(BlueprintGetter, Category="Widget Studio|Helper")
	ETabBarSelectionStyle GetSelectionStyle() const;

	/** Returns the Tab Bar background color. */
	UFUNCTION(BlueprintGetter, Category="Widget Studio|Helper")
	EPalette GetBackgroundColor() const;

	/** Returns the Tab Bar content color. */
	UFUNCTION(BlueprintGetter, Category="Widget Studio|Helper")
	EPalette GetContentColor() const;

	/** Returns the Tab Bar selection color. */
	UFUNCTION(BlueprintGetter, Category="Widget Studio|Helper")
	EPalette GetSelectionColor() const;

	/** Returns the Tab Bar checkable state. */
	UFUNCTION(BlueprintGetter, Category="Widget Studio|Helper")
	bool IsSelectable() const;


	// Scrolling Helper

	/** Returns if the Tab Bar is scrollable or not. */
	UFUNCTION(BlueprintGetter, Category="Widget Studio|Helper")
	float IsScrollable() const;

	/** Returns the current size constraint value.. */
	UFUNCTION(BlueprintGetter, Category="Widget Studio|Helper")
	float GetSizeConstraint() const;

	/** Returns the current scroll bar visibility. */
	UFUNCTION(BlueprintGetter, Category="Widget Studio|Helper")
	ESlateVisibility GetScrollbarVisibility() const;

	// Modifiers
	
	/**
	 * Set the current index.
	 * @param Index The new index to set the Tab bBr to.
	 * @param bBroadcast Enable to broadcast the OnCurrentIndexChanged event.
	 */
	virtual void SetCurrentIndex(int32 Index, bool bBroadcast = true) override;

	/**
	 * Set the grow direction.
	 * @param NewOrientation The new orientation to apply to the Tab Bar.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier")
	void SetOrientation(TEnumAsByte<EOrientation> NewOrientation);

	/**
	 * Set how the corner style looks.
	 * @param NewStyle The new corner style to apply to the widget.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier")
	void SetCornerStyle(ECornerStyle NewStyle);

	/**
	 * Set the color of the background.
	 * @param NewColor The new color to apply to the widgets background.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier")
	void SetBackgroundColor(EPalette NewColor);

	/**
	 * Set the color of the content.
	 * @param NewColor The new color to apply to the widgets content.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier")
	void SetContentColor(EPalette NewColor);

	/**
	 * Set the color of the selection.
	 * @param NewColor The new color to apply to the widgets selection.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier")
	void SetSelectionColor(EPalette NewColor);

	/**
	 * Set the selectable state.
	 * @param NewState Enable to allow the Tab Bar options to be selectable.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier")
	void SetSelectable(bool NewState);


	// Scrolling Modifiers

	/**
	 * Enable or disable the scrollability of the Tab Bar.
	 * @param NewState Enable to allow the Tab Bar to be scrollable
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier")
	void SetScrollable(bool NewState);

	/** Constrains the size of the Tab Bar. \n
	* If the tab bar extends beyond the giving size constraint, the tab bar will become scrollable. \n \n
	* Size and scroll direction is dependent on set orientation. \n \n
	* If set to 0, size constraint will be ignored.
	* @param NewSize The new size to constrain the Tab Bar to.
	*/
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier")
	void SetSizeConstraint(float NewSize);

	/**
	 * Set the scroll visibility of the scroll bar.
	 * @param NewVisibility The new visibility state of the scrollbar.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier")
	void SetScrollbarVisibility(ESlateVisibility NewVisibility);

	/**
	 * Set the alignment of the content.
	 * @param NewAlignment The new alignment to apply to the content.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier")
	void SetContentAlignment(TEnumAsByte<EHorizontalAlignment> NewAlignment);

	/**
	 * Set the text styling options
	 * @param NewTextOptions The new text style to apply to the Tab Bar.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier")
	void SetTextOptions(FWSTextStyle NewTextOptions);

	/**
	 * Set the icon styling options
	 * @param NewIconOptions The new icon style to apply to the Tab Bar.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier")
	void SetIconOptions(FWSIconStyle NewIconOptions);

	/**
	 * Set the icon placement in conjunction to the text.
	 * @param NewIconPlacement The new icon placement to apply to the Tab Bar.
	 */
	UFUNCTION(BlueprintSetter, Category = "Widget Studio|Modifier")
	void SetIconPlacement(EWSPlacement NewIconPlacement);
};
