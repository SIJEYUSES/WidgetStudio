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
#include "Widgets/Modern/WSModernButton.h"
#include "WSContainer.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWSContainerDelegate, int32, CurrentIndex, FButtonOptions, Option);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWSContainerHoverDelegate, UWidgetStudioBase*, CallingWidget, bool, bIsHovering);

/**
 * A base class for building a option container such as a tab bar or combo box
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UWidgetStudioContainer : public UWidgetStudioBase
{
	GENERATED_BODY()

protected:
	virtual void ConstructOption(FButtonOptions Option);
	virtual void SynchronizeProperties() override;

	UFUNCTION()
	virtual void IndividualHoverStateChanged(UWidgetStudioBase* CallingWidget, const bool bIsHovering);

	// Properties

	/** The current option selected. A value of -1 will indicate that no option is selected. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta = (MinClamp = "-1"))
	int32 CurrentIndex = -1;
	
	/** The options that will be available. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta = (TitleProperty = "Text"))
	TArray<FButtonOptions> Options;

public:
	// Bindings
	
	/** Called when the current index has been changed. */
	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
	FWSContainerDelegate OnCurrentIndexChanged;

	/** Called when an individual widget contained in the container's hover state has changed. */
	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
	FWSContainerHoverDelegate OnIndividualHoverStateChanged;
	
	// Helpers

	/** Return the index of the current selected button. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	int32 GetCurrentIndex() const;

	/** Returns the number of options */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	int32 GetOptionCount() const;

	/** Returns the options. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	TArray<FButtonOptions> GetOptions() const;

	/**Return the option at the given index. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	FButtonOptions GetOptionAtIndex(int32 Index);

	/** Get the options of the current button. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	FButtonOptions GetCurrentOption() const;

	/**
	 * Returns the Option Index based on given text.
	 * @param InText The text to find the option by.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Helper")
	int32 GetOptionIndexViaText(FText InText);

	/**
	 * Increases the current index by 1.
	 * @param bBroadcast When true, OnCurrentIndexChanged will be broadcasted.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Helper")
	void IncrementCurrentIndex(bool bBroadcast = true);

	/**
	 * Decreases the current index by 1.
	 * Can only go as low as index 0.
	 * @param bBroadcast When true, OnCurrentIndexChanged will be broadcasted.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Helper")
	void DecrementCurrentIndex(bool bBroadcast = true);

	/**
	 * Sets the current index to -1.
	 * No option will be labeled as selected.
	 * @param bBroadcast When true, OnCurrentIndexChanged will be broadcasted.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Helper")
	void ClearCurrentIndex(bool bBroadcast = true);


	// Modifiers

	/**
	 * Override the options in this container.
	 * @param NewOptions The array of button options to use in the container.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	virtual void SetOptions(TArray<FButtonOptions> NewOptions);

	/** Set the current index.
	 * @param Index The index to set the current index to.
	 * @param bBroadcast When true, OnCurrentIndexChanged will be broadcasted.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	virtual void SetCurrentIndex(int32 Index, bool bBroadcast = true);

	/**
	 * Add an option to the container.
	 * @param NewOption The option to add to the container.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	void AddOption(FButtonOptions NewOption);

	/** Clear all options from the container.*/
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	virtual void ClearOptions();

	/** Finds and sets the current index based on option text.
	 * Returns false if it could not find the option.
	 * @param InText The option text to set the current index to.
	 * @param bBroadcast When true, OnCurrentIndexChanged will be broadcasted.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	bool SetCurrentIndexViaOptionText(FText InText, bool bBroadcast = true);
};

