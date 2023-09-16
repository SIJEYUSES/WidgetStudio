/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/


#include "Widgets/Basic/WSIcon.h"
#include "Blueprint/WidgetTree.h"
#include "Components/Overlay.h"
#include "Components/ScaleBoxSlot.h"
#include "Components/SizeBox.h"
#include "Components/SizeBoxSlot.h"
#include "Widgets/Images/SImage.h"

TSharedRef<SWidget> UWidgetStudioIcon::RebuildWidget()
{
	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
	WidgetTree->RootWidget = RootWidget;
	
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{
		
		/* Construct Widgets */
		SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("SizeBox"));
		ScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("ScaleBox"));
		IconItem = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("IconItem"));

			
		/* Construct Hierarchy */
		RootWidget->AddChild(SizeBox);
		SizeBox->AddChild(ScaleBox);
		ScaleBox->AddChild(IconItem);

		// Make the icon not hit testable by default.
		RootWidget->SetVisibility(ESlateVisibility::HitTestInvisible);

		ScaleBox->SetStretch(EStretch::ScaleToFit);
		Cast<USizeBoxSlot>(ScaleBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<USizeBoxSlot>(ScaleBox->Slot)->SetVerticalAlignment(VAlign_Fill);
		Cast<UScaleBoxSlot>(IconItem->Slot)->SetHorizontalAlignment(HAlign_Center);
		Cast<UScaleBoxSlot>(IconItem->Slot)->SetVerticalAlignment(VAlign_Center);
		
		// Additional Construction Parameters
		bCanOverrideDimensions = false;
		bCanOverrideBorderRadius = false;

		IconStyle = FWSIconStyle(UWidgetStudioFunctionLibrary::GetSizeByModifier(SizeModifier, Size), bUseNativeColor);

		InitializeStyling();
		UpdateStyling();
	}
	return Widget;
}

int32 UWidgetStudioIcon::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	if (bDisablePainting) { return LayerId; }
	
	// Adjust SizeBox dimensions
	const float ModifiedSize = UWidgetStudioFunctionLibrary::GetSizeByModifier(SizeModifier, IconStyle.Size);
	UWidgetStudioFunctionLibrary::InterpSizeBoxOverrides(SizeBox, ModifiedSize, ModifiedSize, AnimationTime);

	// Adjusts icon color
	if (!IconStyle.bUseNativeColor)
	{
		UWidgetStudioFunctionLibrary::InterpImageColor(IconItem, Color, AnimationTime);
	}
	
	return LayerId;
}

void UWidgetStudioIcon::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	IconStyle.Size = Size;
	UpdateStyling();
}

void UWidgetStudioIcon::InitializeStyling()
{
	Super::InitializeStyling();

	// Initialize the SizeBox widget styling
    if (SizeBox)
    {
	    const float ModifiedSize = UWidgetStudioFunctionLibrary::GetSizeByModifier(SizeModifier, IconStyle.Size);
        SizeBox->SetWidthOverride(ModifiedSize);
        SizeBox->SetHeightOverride(ModifiedSize);
    }

	// Initialize the IconItem widget styling
	if (IconItem && !IconStyle.bUseNativeColor)
	{
		IconItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(Color));
	}
}

void UWidgetStudioIcon::UpdateStyling()
{
	Super::UpdateStyling();

	// Update the IconItem widget styling
	if (IconItem)
	{
		if (CustomIcon)
		{
			IconItem->SetBrushFromTexture(CustomIcon, true);
		}
		else
		{
			IconItem->SetBrushFromTexture(UWidgetStudioFunctionLibrary::GetTextureFromIconLibrary(Icon), true);
		}

		if (bDisablePainting)
		{
			if (!IconStyle.bUseNativeColor)
			{
				IconItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(Color));
			}
		}
	}

	// Update the SizeBox widget styling
	if (SizeBox)
	{
		if (bDisablePainting)
		{
			const float ModifiedSize = UWidgetStudioFunctionLibrary::GetSizeByModifier(SizeModifier, IconStyle.Size);
			SizeBox->SetWidthOverride(ModifiedSize);
			SizeBox->SetHeightOverride(ModifiedSize);
		}
	}
}

EIconItem UWidgetStudioIcon::GetIcon() const
{
	return Icon;
}

EPalette UWidgetStudioIcon::GetColor() const
{
	return Color;
}

bool UWidgetStudioIcon::IsUsingCustomIcon() const
{
	return CustomIcon != nullptr;
}

UTexture2D* UWidgetStudioIcon::GetCustomIcon() const
{
	return CustomIcon;
}

float UWidgetStudioIcon::GetSize() const
{
	return IconStyle.Size;
}


float UWidgetStudioIcon::IsUsingNativeColor() const
{
	return IconStyle.bUseNativeColor;
}

void UWidgetStudioIcon::SetUseNativeColor(const bool NewState)
{
	IconStyle.bUseNativeColor = NewState;
	UpdateStyling();
}

void UWidgetStudioIcon::SetIconStyle(const FWSIconStyle& NewOptions)
{
	IconStyle = NewOptions;
	UpdateStyling();
}

void UWidgetStudioIcon::SetSize(const float NewSize)
{
	IconStyle.Size = NewSize;
	UpdateStyling();
}

void UWidgetStudioIcon::SetIcon(const EIconItem NewIcon)
{
	Icon = NewIcon;
	UpdateStyling();
	
}

void UWidgetStudioIcon::SetColor(const EPalette NewColor)
{
	Color = NewColor;
	UpdateStyling();
}

void UWidgetStudioIcon::SetCustomIcon(UTexture2D* NewCustomIcon)
{
	CustomIcon = NewCustomIcon;
	UpdateStyling();
}

void UWidgetStudioIcon::ClearCustomIcon()
{
	IconItem->SetBrushFromTexture(UWidgetStudioFunctionLibrary::GetTextureFromIconLibrary(Icon));
	CustomIcon = nullptr;
	UpdateStyling();
}

