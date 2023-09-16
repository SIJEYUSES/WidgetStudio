/// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/Panel/WSScrollBox.h"
#include "WSFunctionLibrary.h"
#include "WSSubsystem.h"

#define LOCTEXT_NAMESPACE "Widget Studio"

TSharedRef<SWidget> UWSScrollBox::RebuildWidget()
{
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	// Apply scroll box styling
	this->SetWidgetBarStyle(UWidgetStudioFunctionLibrary::GetScrollBarStyle()); // Use setter method
	this->SetAnimateWheelScrolling(true);

	// Update the scroll box style to match the current Widget Studio theme settings
	if (GEngine->IsValidLowLevel())
	{
		GEngine->GetEngineSubsystem<UWidgetStudioSubsystem>()->OnStyleChanged.AddUniqueDynamic(this, &UWSScrollBox::OnStyleChanged);
	}

	return Widget;
}

void UWSScrollBox::OnStyleChanged()
{
	this->SetWidgetBarStyle(UWidgetStudioFunctionLibrary::GetScrollBarStyle()); // Use setter method
}


#if WITH_EDITOR

const FText UWSScrollBox::GetPaletteCategory()
{
	return LOCTEXT("*Widget Studio - Panel", "*Widget Studio - Panel");
}
#endif

#undef LOCTEXT_NAMESPACE
