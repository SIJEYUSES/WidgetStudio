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
#include "WSText.h"
#include "Components/HorizontalBox.h"
#include "Components/Overlay.h"
#include "Components/VerticalBox.h"
#include "Widgets/WSBase.h"
#include "Components/NamedSlot.h"
#include "WSLabel.generated.h"

/**
 * A widget that displays a text label and accepts a single widget as a child.
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UWidgetStudioLabel : public UWidgetStudioBase
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
	UOverlay* Retainer = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UHorizontalBox* HBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UVerticalBox* VBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioText* TextItem = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets", meta=(BindWidget))
	UNamedSlot* WidgetSlot = nullptr;

	// Properties

	/** The text to display on the label. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta = (DisplayName = "Text"))
	FText LabelText = FText().FromString("Label");

	/** Setup how the text is styled. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	FWSTextStyle TextStyle = FWSTextStyle{};
	
	/** Where to place the label in relevance to the child widget. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta = (DisplayName = "Placement"))
	EWSPlacement LabelPlacement = EWSPlacement::Left;

	/** The spacing/padding between the label and the widget.  */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", meta=(UIMin="5", ClampMin="5", DisplayName = "Spacing"))
	float LabelPadding = 30.0f;

public:

	// Helper Functions

	/** Returns the labels text */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	FText GetLabelText() const;

	/** Returns the text options for the label */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	FWSTextStyle GetLabelOptions() const;

	/** Returns the placement of the label in relevance to the slotted widget. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	EWSPlacement GetLabelPlacement() const;

	/** Returns the padding between the label and the slotted widget. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	float GetLabelPadding() const;

	/** Returns the slotted widget. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	UWidget* GetSlottedWidget() const;

	// Modifier Functions

	/** Set the text displayed on the label. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetLabelText(FText NewText);

	/**
	 * Set the text options for the label.
	 * @param NewLabelOptions The text options for the label.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetLabelOptions(FWSTextStyle NewLabelOptions);

	/**
	 * Set the placement of the label in relevance to the slotted widget.
	 * @param NewPlacement The placement of the label in relevance to the slotted widget.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetLabelPlacement(EWSPlacement NewPlacement);

	/**
	 * Set the padding between the label and the slotted widget.
	 * @param NewPadding The padding between the label and the slotted widget.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetLabelPadding(float NewPadding);
};
