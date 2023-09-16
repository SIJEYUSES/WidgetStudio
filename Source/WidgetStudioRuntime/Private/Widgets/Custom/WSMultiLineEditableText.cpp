// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Custom/WSMultiLineEditableText.h"

#include "Widgets/Text/SMultiLineEditableText.h"

void UWSMultiLineEditableText::SetWrapAt(const float InWrapAt) const
{
	if (MyMultiLineEditableText != nullptr)
	{
		MyMultiLineEditableText->SetWrapTextAt(InWrapAt);
		
	}
}
