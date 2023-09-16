// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/MultiLineEditableTextBox.h"
#include "CustomMultiLineTextBox.generated.h"

/**
 * 
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UCustomMultiLineTextBox : public UMultiLineEditableTextBox
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintSetter)
	void SetAutoWrap(bool bAutoWrap) const;
	
	UFUNCTION(BlueprintSetter)
	void SetWrapAt(const float InWrapAt) const;

	UFUNCTION(BlueprintSetter)
	void SetWrappingPolicy(const ETextWrappingPolicy InWrappingPolicy) const;
};
