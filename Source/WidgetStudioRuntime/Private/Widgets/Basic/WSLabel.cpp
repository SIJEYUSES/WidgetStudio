// Fill out your copyright notice in the Description page of Project Settings.

/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/

#include "Widgets/Basic/WSLabel.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/OverlaySlot.h"
#include "Components/VerticalBoxSlot.h"

TSharedRef<SWidget> UWidgetStudioLabel::RebuildWidget()
{
	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
	WidgetTree->RootWidget = RootWidget;

	TSharedRef<SWidget> Widget = Super::RebuildWidget();
	
	if (RootWidget && WidgetTree)
	{
		/* Construct Widgets */
		HBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("HBox"));
		VBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("VBox"));
		TextItem = WidgetTree->ConstructWidget<UWidgetStudioText>(UWidgetStudioText::StaticClass(), TEXT("Text Item"));
			
		/* Construct Hierarchy */
		RootWidget->AddChild(HBox);
		RootWidget->AddChild(VBox);
		HBox->AddChild(TextItem);
		HBox->AddChild(WidgetSlot);
		
		// Additional Construction Parameters
		bCanOverrideDimensions = false;
		bCanOverrideBorderRadius = false;

		InitializeStyling();
		UpdateStyling();
		
	}
	return Widget;
}

int32 UWidgetStudioLabel::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	if (bDisablePainting) { return LayerId; }
		
	return LayerId;
}

void UWidgetStudioLabel::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UpdateStyling();
}

void UWidgetStudioLabel::InitializeStyling()
{
	Super::InitializeStyling();
	if (!HBox) { return; }

	Cast<UOverlaySlot>(HBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(HBox->Slot)->SetVerticalAlignment(VAlign_Fill);
	Cast<UOverlaySlot>(HBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
	Cast<UOverlaySlot>(VBox->Slot)->SetVerticalAlignment(VAlign_Fill);
}

void UWidgetStudioLabel::UpdateStyling()
{
	Super::UpdateStyling();
	if (!HBox || !VBox || !TextItem || !WidgetSlot) { return; }

	if (LabelPlacement == EWSPlacement::Left)
	{
		HBox->AddChild(TextItem);
		HBox->AddChild(WidgetSlot);
		Cast<UHorizontalBoxSlot>(WidgetSlot->Slot)->SetVerticalAlignment(VAlign_Center);
		Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetSize(ESlateSizeRule::Fill);
		Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetVerticalAlignment(VAlign_Center);
		Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetPadding(FMargin(0.0f, 0.0f, LabelPadding, 0.0f));
	}
	if (LabelPlacement == EWSPlacement::Right)
	{
		HBox->AddChild(WidgetSlot);
		HBox->AddChild(TextItem);
		Cast<UHorizontalBoxSlot>(WidgetSlot->Slot)->SetVerticalAlignment(VAlign_Center);
		Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetSize(ESlateSizeRule::Fill);
		Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetVerticalAlignment(VAlign_Center);
		Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetPadding(FMargin(LabelPadding, 0.0f, 0.0f, 0.0f));
	}

	if (LabelPlacement == EWSPlacement::Top)
	{
		VBox->AddChild(TextItem);
		VBox->AddChild(WidgetSlot);
		Cast<UVerticalBoxSlot>(TextItem->Slot)->SetSize(ESlateSizeRule::Fill);
		Cast<UVerticalBoxSlot>(TextItem->Slot)->SetPadding(FMargin(0.0f, 0.0f, 0.0f, LabelPadding));
	}

	if (LabelPlacement == EWSPlacement::Bottom)
	{
		VBox->AddChild(WidgetSlot);
		VBox->AddChild(TextItem);
		Cast<UVerticalBoxSlot>(TextItem->Slot)->SetSize(ESlateSizeRule::Fill);
		Cast<UVerticalBoxSlot>(TextItem->Slot)->SetPadding(FMargin(0.0f, LabelPadding, 0.0f, 0.0f));
	}

	// Update Label
	TextItem->SetText(LabelText);
	TextItem->SetTextStyle(TextStyle);
	TextItem->SetSizeModifier(SizeModifier);
}

FText UWidgetStudioLabel::GetLabelText() const
{
	return LabelText;
}

void UWidgetStudioLabel::SetLabelText(const FText NewText)
{
	LabelText = NewText;
	SynchronizeProperties();
}

FWSTextStyle UWidgetStudioLabel::GetLabelOptions() const
{
	return TextStyle;
}

EWSPlacement UWidgetStudioLabel::GetLabelPlacement() const
{
	return LabelPlacement;
}

float UWidgetStudioLabel::GetLabelPadding() const
{
	return LabelPadding;
}

UWidget* UWidgetStudioLabel::GetSlottedWidget() const
{
	return WidgetSlot->GetChildAt(0);
}

void UWidgetStudioLabel::SetLabelOptions(const FWSTextStyle NewLabelOptions)
{
	TextStyle = NewLabelOptions;
	SynchronizeProperties();
}

void UWidgetStudioLabel::SetLabelPlacement(const EWSPlacement NewPlacement)
{
	LabelPlacement = NewPlacement;
	SynchronizeProperties();
}

void UWidgetStudioLabel::SetLabelPadding(const float NewPadding)
{
	SetPadding(NewPadding);
	SynchronizeProperties();
}
