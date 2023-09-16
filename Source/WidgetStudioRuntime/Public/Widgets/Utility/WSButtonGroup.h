/** 
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
#include "Widgets/WSButtonBase.h"
#include "Widgets/Modern/WSModernCheckBox.h"
#include "WSButtonGroup.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FWSButtonGroupChangedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWSButtonGroupDelegate, int32, Index);

/**
 * A utility widget to group buttons and their logic together.
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UWidgetStudioButtonGroup : public UWidgetStudioBase
{
	GENERATED_BODY()

	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void SynchronizeProperties() override;;

protected:
	
	UFUNCTION()
	void OnButtonPressed(UWidgetStudioButtonBase* Button);

	UFUNCTION()
	void UpdateCurrentIndex();

	UFUNCTION()
	void UpdateButtonCheckedStates();

private:
	/* The button selection method.
	 * Inclusive: Multiple buttons can be selected at a time.
	 * Exclusive: Only a single button can be selected at a time.
	 */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	EClusivity SelectionMethod = EClusivity::Exclusive;

	/** The index of the button currently active. If in Inclusive mode, this will be the last toggled button index. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	int32 CurrentIndex = -1;

	/** Add the buttons to be managed by this group. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	TArray<UWidgetStudioButtonBase*> Buttons;

	/** The buttons being managed by the group. */
	UPROPERTY()
	TArray<UWidgetStudioButtonBase*> ManagedButtons = {};

	/** The latest button pressed. */
	UPROPERTY()
	UWidgetStudioButtonBase* CurrentButton = nullptr;

public:
	/* Bindings */

	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
	FWSButtonGroupDelegate OnCurrentIndexChanged;

	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
	FWSButtonGroupChangedDelegate OnButtonsChanged;

	
	/* Helpers */

	/** Returns the index of the last button to be selected. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	int32 GetCurrentIndex() const;

	/** Returns all of the buttons managed by the Button Group */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	TArray<UWidgetStudioButtonBase*> GetButtons() const;

	/**
	 * Returns the button at the given index.
	 * @param Index The index to find the button at.
	 */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	UWidgetStudioButtonBase* GetButtonAtIndex(int32 Index);

	/** Returns the button at the current index. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	UWidgetStudioButtonBase* GetCurrentButton();

	/** Returns the number of buttons contained within the Button Group. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	int32 GetButtonCount() const;

	/** Returns the current selection method. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	EClusivity GetSelectionMethod() const;


	/* Modifiers */

	/**
	 * Add a button to the Button Group.
	 * @param NewButton The button to add to the button group.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void AddButton(UWidgetStudioButtonBase* NewButton);

	/**
	 * Remove a button from the Button Group
	 * @param ButtonToRemove The button to remove from the button group.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	bool RemoveButton(UWidgetStudioButtonBase* ButtonToRemove);

	/**
	 * Remove button at given index.
	 * @param Index The index to remove the button from.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void RemoveButtonAtIndex(int32 Index);

	/** Removes all buttons from Button Group. */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void RemoveAllButtons();

	/**
	 * Set the currently selected button.
	 * @param Index The index to set the button group to.
	 * @param bBroadcast Enable to dispatch the OnCurrentIndexChanged event
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetCurrentIndex(int32 Index, bool bBroadcast = true);

	/**
	 * Set the currently selected button.
	 * @param NewMethod The method to change the button group to.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void SetSelectionMethod(EClusivity NewMethod);

};
