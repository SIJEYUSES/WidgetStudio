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
#include "Blueprint/UserWidget.h"

/* Hierarchical Includes - Do Not Remove */
#include "Types/WSEnums.h"
#include "WSFunctionLibrary.h"
#include "WSSubsystem.h"

#include "WSBase.generated.h"

class UWidgetStudioBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWSBaseHoverStateDelegate, UWidgetStudioBase*, CallingWidget, bool, bIsHovering);

/**
 * The base User Widget class for Widget Studio.
 * Contains logic that propagates to all other Widget Studio widget classes.
 */
UCLASS(Blueprintable, HideCategories="Developer")
class WIDGETSTUDIORUNTIME_API UWidgetStudioBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	
	/*
	 * Initializes and sets up the initial Widget Styling.
	 * - This function should be overriden in a child widget.
	 * - This function should be placed at the end of the RebuildWidget() function
	 */
	virtual void InitializeStyling();

	/*
	 * Updates the Widget styling. This is primarily to make sure that any styling updates are kept dynamic.
	 * - This function should be overriden in a child widget.
	 * - This function should be placed and used when deemed necessary, for example, within the SynchronizeProperties() function.
	 * - Make sure that static (non-updating) styling is not placed here and is instead placed in the InitializeStyling() function
	 * - Do NOT call on tick or OnPaint.
	 */
	virtual void UpdateStyling();

	/* Used for event passthroughs */
	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent) override;

public:

	// Bindings
	
	/** Called when the hover state has been changed. */
	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
	FWSBaseHoverStateDelegate OnHoverStateChanged;

	// Properties

	/**
	 * Used to enable or disable dimension override's.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Developer")
	bool bCanOverrideDimensions = true;

	/**
	 * Used to enable or disable dimension override's.
	 */
	UPROPERTY(EditDefaultsOnly, Category = "Developer")
	bool bCanOverrideBorderRadius = true;

	/**
	* The minimum dimensions of the widget. The global and custom overrides will not go below these values.
	*/
	UPROPERTY(EditAnywhere, Category = "Developer", Meta = (ClampMin = "0", UIMin = "0"))
	FVector2D MinimumDimensions = FVector2D(50, 7);
	
	/**
	* Used for debugging.
	* Disables the OnPaint event in many of the widgets, thus disabling all animations and smooth transitions. May cause graphical inconsistencies.
	*/
	UPROPERTY(EditAnywhere, Category = "Developer")
	bool bDisablePainting = false;

	/**
	 * The animation interpolation speed.
	 * The lower the value, the slower the speed.
	 * Set to 0 to disable animations.
	 */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", AdvancedDisplay)
	float AnimationTime = 7;

	/**
	* Quickly modify the overall size of the widget.
	* - Mini: 55%
	* - Tiny: 70%
	* - Small: 85%
	* - Regular: 100%
	* - Large: 115%
	* - Big: 130%
	* - Huge: 145%
	* - Massive: 160%
	*/
	UPROPERTY(EditAnywhere, Category = "Widget Studio", AdvancedDisplay)
	ESizeModifier SizeModifier = ESizeModifier::Regular;
	
	/**
	 * Manually adjust the X and Y dimensions of the widget. This overrides the Control Dimensions from the Widget Studio Subsystem.
	 * - Dimensions are overriden if above 0 and above widgets minimum default dimensions.
	 * - Enabled only on select visual widgets.
	 */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", AdvancedDisplay, Meta = (ClampMin = "0", UIMin = "0", AxisName="Width", YAxisName="Height", EditCondition="bCanOverrideDimensions", EditConditionHides))
	FVector2D OverrideDimensions = FVector2D(0, 0);

	/**
	 * Manually adjust the border radius of the widget. This overrides the Border Radius from the Widget Studio Subsystem.
	 * - The Border Radius is overriden if above -1.
	 * - Enabled only on select visual widgets.
	 */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", AdvancedDisplay, Meta = (ClampMin = "-1", UIMin = "-1", EditCondition="bCanOverrideBorderRadius", EditConditionHides))
	int32 OverrideBorderRadius = -1;

	
	// Helper Functions

	/**
	 * Returns the current X and Y dimensions of the Widget.
	 * Derived from Widget Studio Subsystem, or from the Overriden Dimensions.
	 */
	UFUNCTION(BlueprintPure, Category = "Widget Studio")
	FVector2D GetDimensions() const;

	/**
	 * Returns the current border radius of the Widget.
	 * Derived from Widget Studio Subsystem, or from the Overriden Border Radius.
	 */
	UFUNCTION(BlueprintPure, Category = "Widget Studio")
	int32 GetBorderRadius() const;


	// Modifier Functions

	/** Refresh the dynamic styling of the Widget Studio widget. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Advanced")
	void ForceStyleUpdate();

	/** Set the size modifier of the Widget Studio widget. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Advanced")
	void SetSizeModifier(ESizeModifier InSizeModifier);
};
