// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/MultiLineEditableText.h"
#include "WSMultiLineEditableText.generated.h"

/**
 * 
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UWSMultiLineEditableText : public UMultiLineEditableText
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintSetter)
	void SetWrapAt(const float InWrapAt) const;
	
};
