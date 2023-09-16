// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Custom/WSEditableText.h"
#include "Widgets/Input/SEditableText.h"

void UWSEditableText::SetFont(const FSlateFontInfo& InFontInfo) const
{
	if (MyEditableText)
	{
		MyEditableText->SetFont(InFontInfo);
	}
}
