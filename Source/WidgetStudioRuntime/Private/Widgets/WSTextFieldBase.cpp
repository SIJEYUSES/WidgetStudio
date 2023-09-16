/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/


#include "Widgets/WSTextFieldBase.h"

UWidgetStudioTextFieldBase::UWidgetStudioTextFieldBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	TextStyle.Type = EWSFontType::Body2;
	TextStyle.WrappingPolicy = ETextWrappingPolicy::AllowPerCharacterWrapping;
	TextStyle.AutoWrapText = true;
}

void UWidgetStudioTextFieldBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();
}

void UWidgetStudioTextFieldBase::SetLabel(const FText NewLabel)
{
	Label = NewLabel;
	SynchronizeProperties();
}

void UWidgetStudioTextFieldBase::OnTextInputTextChanged(const FText& InText)
{
	SetText(InText);
}

void UWidgetStudioTextFieldBase::OnTextInputTextCommitted(const FText& InText, const ETextCommit::Type InCommitMethod)
{
	SetText(InText);
	OnTextCommitted.Broadcast(Text, InCommitMethod);
}

FText UWidgetStudioTextFieldBase::GetLabel() const
{
	return Label;
}

FText UWidgetStudioTextFieldBase::GetText() const
{
	return Text;
}

FText UWidgetStudioTextFieldBase::GetPlaceholderText() const
{
	return PlaceholderText;
}

ETextFilter UWidgetStudioTextFieldBase::GetFilter() const
{
	return Filter;
}

int32 UWidgetStudioTextFieldBase::GetCharacterLimit() const
{
	return CharacterLimit;
}

void UWidgetStudioTextFieldBase::SetCharacterLimit(const int32 NewLimit)
{
	CharacterLimit = NewLimit;
	SetText(Text);
}

ETextFieldState UWidgetStudioTextFieldBase::GetState() const
{
	return State;
}

bool UWidgetStudioTextFieldBase::IsPasswordField() const
{
	return bIsPasswordField;
}

void UWidgetStudioTextFieldBase::SetText(const FText NewText)
{
	FText TempText = NewText;

	if (Filter == ETextFilter::NumbersOnly && !TempText.IsNumeric() && !TempText.IsEmpty())
	{
		TempText = Text;
	}

	if (Filter == ETextFilter::TextOnly && UWidgetStudioFunctionLibrary::ContainsNumbers(TempText))
	{
		TempText = Text;
	}

	// Trim by character limit
	TempText = UWidgetStudioFunctionLibrary::FilterByCharacterLimit(TempText, CharacterLimit);

	// Trim by whitespace only if the user doesn't currently have focus
	if (!this->HasFocusedDescendants())
	{
		TempText = FText::FromString(TempText.ToString().TrimStartAndEnd());
	}
	
	// Set the text
	Text = TempText;

	// Sets the text to the text box
	SynchronizeProperties();

	// Broadcast Change
	OnTextChanged.Broadcast(Text);
}

void UWidgetStudioTextFieldBase::SetPlaceholderText(const FText NewText)
{
	PlaceholderText = NewText;
	SynchronizeProperties();
}

void UWidgetStudioTextFieldBase::SetFilter(const ETextFilter NewFilter)
{
	Filter = NewFilter;
	SetText(Text);
}

void UWidgetStudioTextFieldBase::SetState(const ETextFieldState NewState)
{
	State = NewState;
}
