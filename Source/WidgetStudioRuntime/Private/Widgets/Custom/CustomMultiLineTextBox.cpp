// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Custom/CustomMultiLineTextBox.h"

#include "Widgets/Input/SMultiLineEditableTextBox.h"

void UCustomMultiLineTextBox::SetAutoWrap(const bool bAutoWrap) const
{
	if (MyEditableTextBlock.IsValid())
	{
		MyEditableTextBlock->SetAutoWrapText(bAutoWrap);
	}
}

void UCustomMultiLineTextBox::SetWrapAt(const float InWrapAt) const
{
	if (MyEditableTextBlock.IsValid())
	{
		MyEditableTextBlock->SetWrapTextAt(InWrapAt);
	}
}

void UCustomMultiLineTextBox::SetWrappingPolicy(const ETextWrappingPolicy InWrappingPolicy) const
{
	if (MyEditableTextBlock.IsValid())
	{
		MyEditableTextBlock->SetWrappingPolicy(InWrappingPolicy);
	}
}
