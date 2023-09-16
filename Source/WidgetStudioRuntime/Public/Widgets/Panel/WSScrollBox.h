// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ScrollBox.h"
#include "WSScrollBox.generated.h"

/**
 * A Widget Studio customized version of the Scrollbox
 */
UCLASS(HideCategories="Style")
class WIDGETSTUDIORUNTIME_API UWSScrollBox : public UScrollBox
{
	GENERATED_BODY()
	virtual TSharedRef<SWidget> RebuildWidget() override;

	/** Update the scroll bar styling when the Widget Studio theme changes */
	UFUNCTION()
	void OnStyleChanged();

protected:
	
#if WITH_EDITOR
	virtual const FText GetPaletteCategory() override;
#endif
};
