// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/EditableText.h"
#include "WSEditableText.generated.h"

/**
 * A customized EditableText widget that exposes required functionality.
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UWSEditableText : public UEditableText
{
	GENERATED_BODY()

public:
	// Remove the UFUNCTION(BlueprintSetter) declaration here
	void SetFont(const FSlateFontInfo& InFontInfo) const; // Remove 'override' keyword
};
