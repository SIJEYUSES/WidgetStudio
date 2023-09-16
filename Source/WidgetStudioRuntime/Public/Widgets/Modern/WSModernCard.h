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
#include "Components/Overlay.h"
#include "Components/ScaleBox.h"

#include "WSModernCard.generated.h"

UENUM(Blueprintable, BlueprintType, META=(Tooltip = "Depicts the background style target of the widget."))
enum class EBackgroundStyle : uint8
{
	Solid					UMETA(DisplayName="Solid"),
	OutLine					UMETA(DisplayName="Outline"),

	BackgroundStyle_Max		UMETA(Hidden),
};


UENUM(Blueprintable, BlueprintType, META=(Tooltip = "The roundness style of the widget corners."))
enum class ECornerStyle : uint8
{
	Rounded				UMETA(DisplayName = "Rounded"),
	Pill				UMETA(DisplayName = "Pill"),

	CornerStyle_Max		UMETA(Hidden),
};

UENUM(Blueprintable, BlueprintType, META=(Tooltip = "The style of the widget drop shadows."))
enum class EShadowStyle : uint8
{
	None				UMETA(DisplayName="None"),
	Small				UMETA(DisplayName="Short"),
	Long				UMETA(DisplayName="Long"),
	
	ButtonShadowStyle_Max		UMETA(Hidden),
};

/**
 * A rounded card for background usage. Ideal to combine with an overlay panel.
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UWidgetStudioModernCard : public UWidgetStudioBase
{
	GENERATED_BODY()

protected:
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual void SynchronizeProperties() override;;
	virtual void InitializeStyling() override;
	virtual void UpdateStyling() override;
	
	// Internal Properties
	const float BaseDropShadowOpacity = .5;

	UPROPERTY()
	mutable float ShadowLength;

	// Widget Components

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* SizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* ScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UOverlay* Overlay = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* DropShadow = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* Background = nullptr;

private:
	// Properties

	/** The color of the background. */
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	EPalette Color = EPalette::PrimaryBackground;

	/** The style of the drop shadow. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style")
	EShadowStyle ShadowStyle = EShadowStyle::Small;

	/** Enable drop shadow animation on hover. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Style")
	bool bEnableShadowHoverAnimation = false;

public:
	// Helpers

	/** Returns the color of the background. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EPalette GetColor() const;

	/** Returns true if the shadow hover animation is enabled. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsShadowHoverAnimationEnabled() const;

	/** Returns the current drop shadow style. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EShadowStyle GetDropShadowStyle() const;

	// Modifiers

	/**
	 * Set the color used for the background.
	 * @param NewColor The color to apply to the card.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetColor(EPalette NewColor);
	
	/**
	 * If enabled, the shadow length will increase when the card is hovered.
	 * @param NewState Enable or disable the hover animation.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetShadowHoverAnimationEnabled(bool NewState);

	/**
	 * Establish the styling of the drop shadow.
	 * @param NewStyle Set which drop shadow style to utilize.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetShadowStyle(EShadowStyle NewStyle);
};
