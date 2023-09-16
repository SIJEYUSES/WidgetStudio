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
#include "Components/HorizontalBox.h"
#include "Components/Overlay.h"
#include "Widgets/WSSliderBase.h"
#include "Widgets/Basic/WSIcon.h"
#include "Widgets/Basic/WSText.h"
#include "WSModernSpinBox.generated.h"

/**
 * A modernly styled spin box from Widget Studio.
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UWidgetStudioModernSpinBox : public UWidgetStudioSliderBase
{
	GENERATED_BODY()

protected:
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseWheel(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void SynchronizeProperties() override;
	virtual void InitializeStyling() override;
	virtual void UpdateStyling() override;

	// Tick is being used instead of timers due to an issue where the slate timing gets halted when
	// the left mouse button is pressed.
	// TODO: Find an alternative method that can utilize timers WHEN the left mouse button is being pressed.
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UFUNCTION()
	void OnTextCommitted(const FText& InText, ETextCommit::Type InCommitMethod);

	/**
	 * Logic for updating the value based on X axis mouse movement.
	 * Note: This is an internal function only and is called during tick when required.
	 */
	UFUNCTION()
	void UpdateValueOnMouseMovement();
	
	/* Widget Components */

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* SizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* Overlay = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioModernCard* BackgroundIndicator = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UHorizontalBox* HorizontalBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioText* LabelItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* BackgroundOverlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioModernCard* Background = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioModernCard* BackgroundFill = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UHorizontalBox* ContentHorizontalBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioIcon* ArrowLeft = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioIcon* ArrowRight = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UEditableTextBox* ValueInput = nullptr;

	
	/* Internal Properties */

	/** States */
	bool bIsPressed = false;
	bool bIsLeftArrowPressed = false;
	bool bIsRightArrowPressed = false;
	
	/** Mouse positions */
	float InitialMousePos = 0.f;
	float LastMousePos = 0.f;

	/* The amount of time the left mouse button has been pressed */
	float PressTime = 0.f;

	
	/* Properties */

	/** The text on the label. Label is hidden when empty. */
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	FText Label;

	/**
	 * Allows the user to adjust the value by pressing the arrow buttons.
	 * Note: The buttons will be hidden when disabled.
	 */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Input")
	bool bEnableArrowButtons = true;

	/** Allows the user to adjust the value by using the mouse wheel. */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Input")
	bool bEnableMouseWheelInput = true;

	/**
	 * Allows the user to adjust the value by dragging left/right.
	 * Note: Drag location originates from the left lip.
	 */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Input")
	bool bEnableDragInput = true;
	
	/** The color of the background. */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color")
	EPalette BackgroundColor = EPalette::PrimaryBackground;

	/** The color of the content. */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color")
	EPalette ContentColor = EPalette::PrimaryContent;

	/** The color of the indicator. */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color")
	EPalette IndicatorColor = EPalette::PrimaryAccent;

public:
	
	/* Helpers */
	
	/** Returns the label. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	FText GetLabel() const;

	/** Returns true if the user can interface with the Spin Box via the arrow buttons. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool AreArrowButtonsEnabled() const;

	/** Returns true if the user can interface with the Spin Box via mouse wheel. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsMouseWheelInputEnabled() const;

	/** Returns true if the user can interface with the Spin Box via dragging. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsDragInputEnabled() const;
	
	/** Returns the background color. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EPalette GetBackgroundColor() const;

	/** Returns the content color. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EPalette GetContentColor() const;

	/** Returns the indicator color. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EPalette GetIndicatorColor() const;

	/* Modifiers */

	/**
	 * Set the text on the label. 
	 * @param NewText The new text to display on the label. If text is left blank, then the label will be hidden.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetLabelText(const FText NewText);

	/**
	 * Set the Spin Box to be adjustable via the arrow buttons.
	 * @param bState The new enabled state. When enabled, the user will be able to adjust the value via the arrow buttons. When disabled, the buttons will be hidden.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetArrowButtonsEnabled(const bool bState);

	/**
	 * Set the Spin Box to be adjustable via mouse wheel.
	 * @param bState The new enabled state. When enabled, the user will be able to adjust the value via the mouse wheel.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetMouseWheelInputEnabled(const bool bState);

	/**
	 * Set the Spin Box to be adjustable via dragging.
	 * @param bState The new enabled state. When enabled, the user will be able to adjust the value via the dragging.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetDragInputEnabled(const bool bState);

	/**
	 * Set the background color. 
	 * @param NewColor The new color to apply to the background.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetBackgroundColor(const EPalette NewColor);

	/**
	 * Set the background color. 
	 * @param NewColor The new color to apply to the content.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetContentColor(const EPalette NewColor);

	/**
	 * Set the background color. 
	 * @param NewColor The new color to apply to the indicator.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetIndicatorColor(const EPalette NewColor);

	/* Overrides */
	virtual void SetValue(float NewValue, bool bBroadcast = true) override;
};
