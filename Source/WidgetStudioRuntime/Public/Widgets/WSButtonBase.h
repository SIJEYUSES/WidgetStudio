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
#include "Blueprint/UserWidget.h"
#include "WSButtonBase.generated.h"

DECLARE_DELEGATE(FWSButtonLambda);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWSButtonDelegate, UWidgetStudioButtonBase*, CallingButton);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWSButtonToggledDelegate, UWidgetStudioButtonBase*, CallingButton, bool, NewCheckedState);

/**
 * Contain the fundamental logic for a Widget Studio button.
 * Subclass to create custom buttons that are compatible with Widget Studio.
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UWidgetStudioButtonBase : public UWidgetStudioBase
{
	GENERATED_BODY()

	// Depicts if the button is currently being pressed down.
	bool bIsPressed = false;

protected:
	virtual void NativeOnInitialized() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseCaptureLost(const FCaptureLostEvent& CaptureLostEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;

	FTimerHandle DoublePressTimer;
	
	/* States */

	/**
	 * Depicts if the button can be checked or not.
	 */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|State")
	bool bIsCheckable = false;

	/**
	 * Depicts if the button is checked (on/true) or not (off/false).
	 * Requires the bIsCheckable state to be enabled to utilize.
	 */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|State", Meta = (EditCondition="bIsCheckable == true"))
	bool bIsChecked = false;

	/**
	* Locks the current checked state. The user will not be able to change the state until this bool is set to false.
	* Requires the bIsCheckable state to be enabled to utilize.
	*/
	UPROPERTY(EditAnywhere, Category = "Widget Studio|State", Meta = (EditCondition="bIsCheckable == true"))
	bool bIsCheckedStateLocked = false;

public:

	/* Bindings */

	/** An open lambda binding for to be used for complex functionality when the button is pressed.
	 * Called when OnPressed is called.
	 */
	FWSButtonLambda OnPressedLambda;

	/** Called when the button is pressed with the left mouse button. */
	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
	FWSButtonDelegate OnPressed;

	/** Called when the button is released.*/
	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
    FWSButtonDelegate OnReleased;

	/** Called when the button is quickly pressed twice with the left mouse button.*/
	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
	FWSButtonDelegate OnDoublePressed;

	/** Executes the OnPressedLambda delegate.
	 * Note: Will only execute if the lambda has been bound.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Event|Advanced")
	void ExecuteOnPressedLambda(UWidgetStudioButtonBase* CallingButton)
	{
		OnPressedLambda.ExecuteIfBound();
	}

	/**
	 * Called when the buttons checked status is toggled.
	 * Only called when the button is set to checkable.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
	FWSButtonToggledDelegate OnToggled;

	UPROPERTY()
	bool bForceCheckable = false;

	/**
	 * Internal Use Only.
	 * Used to make the button non-interactable when checked.
	 */
	UPROPERTY()
	bool bIsInteractable = true;

	/* Helpers */

	/**
	 * Returns the checkable state of the button.
	 * - Will return false if not checkable.
	 * - Will return false if checked state changing is locked.
	 */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|State")
	bool IsCheckable() const;

	/**
	 * Returns the checked state of the button.
	 * - Will return false if not checkable.
	 */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|State")
	bool IsChecked() const;

	/** Returns the checked lock state of the button. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|State")
	bool IsCheckedStateLocked() const;

	/** Returns the pressed state of the button.*/
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper|State")
	bool IsPressed() const;

	/* Modifiers */

	/**
	 * Set the checkable state of the button. When enabled, the button can be checked on or off.
	 * @param bNewCheckableState The checkable state to update the button to.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier|State")
	virtual void SetCheckable(bool bNewCheckableState);

	/**
	 * Set the checked state of the button.
	 * Important: This function does nothing if the buttons checkable state is set to false.
	 * @param bNewCheckedState The checked state to update the button to.
	 * @param bBroadcastEvent Whether or not to broadcast the event.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier|State")
	virtual void SetChecked(bool bNewCheckedState, bool bBroadcast = true);

	/**
	 * Lock or unlock the ability to change the checked state of the button.
	 * @param bNewCheckedState The checked locked state to update to button to.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier|State")
	virtual void SetCheckedLockedState(bool bNewCheckedState);
};
