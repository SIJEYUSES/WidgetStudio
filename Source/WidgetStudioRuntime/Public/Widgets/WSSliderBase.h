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
#include "WSSliderBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWSSliderDelegate, float, NewValue);

/**
 * Base class for building sliders
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UWidgetStudioSliderBase : public UWidgetStudioBase
{
	GENERATED_BODY()

protected:

	virtual void SynchronizeProperties() override;
	
	/**
	 * Filter Value based on set properties.
	 * Important: Internal Use Only.
	 * @param InValue The value to filter.
	 */
	UFUNCTION()
	float FilterValue(float InValue) const;
	
	/* Properties */
	
	UPROPERTY()
	bool bLockIncrement = false;

	UPROPERTY()
	bool bLockDecrement = false;

	/** The clamping method for the widget. */
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	EValueClamp ClampMethod = EValueClamp::Clamped;

	/**
	 * The value type of the slider.
	 * Note, change the Step Size to a decimal value for this to properly take effect.
	 */ 
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	EEnumeration Enumeration = EEnumeration::Int;

	/** The current value of the slider. */
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	float Value = 50.f;

	/** The minimum value of the slider. */
	UPROPERTY(EditAnywhere, Category="Widget Studio", Meta = (EditCondition = "ClampMethod == EValueClamp::Clamped", EditConditionHides))
	float Min = 0.f;

	/** The maximum value of the slider. */
	UPROPERTY(EditAnywhere, Category="Widget Studio", Meta = (EditCondition = "ClampMethod == EValueClamp::Clamped", EditConditionHides))
	float Max = 100.f;

	/** The step size value of the slider. */
	UPROPERTY(EditAnywhere, Category="Widget Studio", Meta = (ClampMin = "0", UIMin = "0"))
	float StepSize = 1.f;

public:
	
	/* Bindings */
	
	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
	FWSSliderDelegate OnValueChanged;

	/* Helpers */

	/** Returns the clamp method. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EValueClamp GetClampMethod() const;

	/** Returns the current value. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float GetValue() const;

	/** Returns the current value as FString. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	FString GetValueAsString() const;

	/** Returns the current value as FText. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	FText GetValueAsText() const;

	/** Returns the minimum slider value. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float GetMinValue() const;

	/** Returns the maximum slider value. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float GetMaxValue() const;

	/** Returns the step size of the slider. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float GetStepSize() const;

	/** Returns the enumeration type. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EEnumeration GetEnumerationType() const;

	/** Can the user increase the value? */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsIncrementLocked() const;

	/** Can the user decrease the value? */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsDecrementLocked() const;


	/* Modifiers */

	/**
	 * Set the clamping method. 
	 * @param NewMethod The new clamping method. If setting the method to clamped, make sure to update the min/max values.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetClampMethod(const EValueClamp NewMethod);

	/**
	 * Set the value of the slider.
	 * @param NewValue The value to set the slider to.
	 * @param bBroadcast Enable to broadcast the OnValueChanged event.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	virtual void SetValue(float NewValue, bool bBroadcast = true);

	/**
	 * Set the minimum value of the slider.
	 * @param NewValue The new minimum value that the slider will allow.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	virtual void SetMinValue(float NewValue);

	/**
	 * Set the maximum value of the slider.
	 * @param NewValue The new maximum value that the slider will allow.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	virtual void SetMaxValue(float NewValue);

	/**
	 * Set the step size of the slider.
	 * @param NewValue The new step size that the slider will snap to when the current value changes.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	virtual void SetStepSize(float NewValue);

	/**
	 * Set the enumeration type.
	 * @param NewType The new enumeration type.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	virtual void SetEnumerationType(EEnumeration NewType);

	/**
	 * Increases the value by the step value.
	 * @param Multiplier Multiply the step value increase per increment call.
	 * @param bBroadcast Enable to broadcast the OnValueChanged event.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	virtual void Increment(float Multiplier = 1.f, bool bBroadcast = true);

	/**
	 * Decreases the value by the step value.
	 * @param Multiplier Multiply the step value increase per increment call.
	 * @param bBroadcast Enable to broadcast the OnValueChanged event.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	virtual void Decrement(float Multiplier = 1.f, bool bBroadcast = true);

	/**
	 * Allow or disallow the user to increase the value.
	 * @param NewState The new lock increment state. Enable to disable the option to increase the slider value.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	virtual void SetLockIncrement(bool NewState);

	/**
	 * Allow or disallow the user to decrease the value.
	 * @param NewState The new lock increment state. Enable to disable the option to decrease the slider value.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	virtual void SetLockDecrement(bool NewState);
};
