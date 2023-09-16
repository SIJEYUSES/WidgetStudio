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
#include "Components/Overlay.h"
#include "Components/ScaleBox.h"
#include "WSDivider.generated.h"

/**
 * A visual divider used to divide content.
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UWidgetStudioDivider : public UWidgetStudioBase
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
	UOverlay* SpaceAllocation = nullptr;
	
	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* SizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UScaleBox* ScaleBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UImage* ImageItem = nullptr;

private:
	/* Properties  */

	/** The orientation of the divider. */
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	TEnumAsByte<EOrientation> Orientation = Orient_Vertical;

	/** The thickness of the divider. */
	UPROPERTY(EditAnywhere, Category="Widget Studio", Meta = (MinClamp = "1", UIMin = "1"))
	float Thickness = 3;

	/** The the length of the divider. */
	UPROPERTY(EditAnywhere, Category="Widget Studio", Meta = (MinClamp = "1", UIMin = "1"))
	float Length = 64;

	/** How opaque the divider is. */
	UPROPERTY(EditAnywhere, Category="Widget Studio", Meta = (MinClamp = "0", MaxClamp = "1", UIMin = "0", UIMax = "1"))
	float Opacity = 0.7f;
	
	/** The amount of space above (Horizontal Orientation) or left (Vertical Orientation) of the divider. */
	UPROPERTY(EditAnywhere, Category="Widget Studio", Meta = (MinClamp = "0", UIMin = "0"))
	float LeadingSpace = 10;

	/** The amount of space below (Horizontal Orientation) or right (Vertical Orientation) of the divider. */
	UPROPERTY(EditAnywhere, Category="Widget Studio", Meta = (MinClamp = "0", UIMin = "0"))
	float TrailingSpace = 10;

	// Color

	/** The color of the divider. */
	UPROPERTY(EditAnywhere, Category="Widget Studio|Color")
	EPalette Color = EPalette::TertiaryContent;

public:
	/* Helpers */

	/** Returns the orientation of the divider. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	TEnumAsByte<EOrientation> GetOrientation() const;

	/** Returns the thickness of the divider. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float GetThickness() const;

	/** Returns the length of the divider. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float GetLength() const;

	/** Returns the opacity of the divider. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float GetOpacity() const;

	/** Returns the space allocated above (Horizontal Orientation) or left (Vertical Orientation) of the divider. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float GetLeadingSpace() const;

	/** Returns the space allocated below (Horizontal Orientation) or right (Vertical Orientation) of the divider. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float GetTrailingSpace() const;

	/** Returns the color of the divider. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper|Color")
	EPalette GetColor() const;

	
	/* Modifiers */

	/**
	 * Set the orientation of the divider.
	 * @param NewOrientation The orientation to set the divider to.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetOrientation(TEnumAsByte<EOrientation> NewOrientation);

	/**
	 * Set the thickness of the divider.
	 * @param NewValue The value to set the thickness of the divider.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetThickness(float NewValue);

	/**
	 * Set the length of the divider.
	 * @param NewValue The value to set the length of the divider.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetLength(float NewValue);

	/**
	 * Set the opacity of the divider.
	 * @param NewValue The value to set the opacity of the divider.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetOpacity(float NewValue);

	/**
	 * Sets the space allocated above (Horizontal Orientation) or left (Vertical Orientation) of the divider.
	 * @param NewValue The value to set how much space/padding is before the divider.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetLeadingSpace(float NewValue);

	/**
	 * Sets the space allocated below (Horizontal Orientation) or right (Vertical Orientation) of the divider.
	 * @param NewValue The value to set how much space/padding is after the divider.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetTrailingSpace(float NewValue);

	/**
	 * Set the color of the divider.
	 * @param NewColor The color to set the divider to.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier|Color")
	void SetColor(EPalette NewColor);
};
