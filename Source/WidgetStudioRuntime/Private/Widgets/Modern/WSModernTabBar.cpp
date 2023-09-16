/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/


#include "Widgets/Modern/WSModernTabBar.h"

#include "Blueprint/WidgetTree.h"
#include "Components/GridSlot.h"
#include "Components/OverlaySlot.h"
#include "Components/ScaleBoxSlot.h"
#include "Components/SizeBoxSlot.h"
#include "Components/UniformGridSlot.h"

TSharedRef<SWidget> UWidgetStudioModernTabBar::RebuildWidget()
{
	// Establish the root widget
	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
	WidgetTree->RootWidget = RootWidget;

	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{
		// Construct Widgets
		SizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("SizeBox"));
		ScrollBox = WidgetTree->ConstructWidget<UWSScrollBox>(UWSScrollBox::StaticClass(), TEXT("Scroll Box"));
		Overlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Overlay"));
		ButtonGroup = WidgetTree->ConstructWidget<UWidgetStudioButtonGroup>(UWidgetStudioButtonGroup::StaticClass(), TEXT("Button Group"));
		BackgroundScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Background Scale Box"));
		BackgroundOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Background Overlay"));
		BackgroundDropShadow = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Background DropShadow"));
		Background = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Background"));
		SelectionOverlaySize = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Selection Overlay Size"));
		SelectionOverlay = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Selection Overlay"));
		Grid = WidgetTree->ConstructWidget<UUniformGridPanel>(UUniformGridPanel::StaticClass(), TEXT("Grid"));
				
		// Construct Hierarchy
		RootWidget->AddChild(SizeBox);

		/* SizeBox and ScrollBox are configuring in InitializeStyling */
		Overlay->AddChild(ButtonGroup);
		Overlay->AddChild(BackgroundScaleBox);
		Overlay->AddChild(SelectionOverlaySize);
		Overlay->AddChild(Grid);

		BackgroundScaleBox->AddChild(BackgroundOverlay);
		BackgroundOverlay->AddChild(BackgroundDropShadow);
		BackgroundOverlay->AddChild(Background);
		
		SelectionOverlaySize->AddChild(SelectionOverlay);

		InitializeStyling();
	}
	return Widget;
}

int32 UWidgetStudioModernTabBar::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect,
	FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle,
	bool bParentEnabled) const
{
	if (bDisablePainting) { return LayerId; }
	
	const float DimX = GetDimensions().X;
	const float DimY = GetDimensions().Y;
	const bool IsCurrentIndexValid = GetCurrentIndex() > -1 && GetCurrentIndex() < ButtonGroup->GetButtonCount();
	
	// Get current button size if valid, otherwise, default to minimal Grid sizing.
	FVector2D CurrentButtonSize;
	if (IsCurrentIndexValid)
	{
		CurrentButtonSize = Grid->GetChildAt(GetCurrentIndex())->GetPaintSpaceGeometry().GetLocalSize();
	}
	else
	{
		CurrentButtonSize = Orientation == Orient_Horizontal ? FVector2D(0, Grid->GetPaintSpaceGeometry().GetLocalSize().Y) : FVector2D(Grid->GetPaintSpaceGeometry().GetLocalSize().X, 0);
	}
	
	// Smoothly update Size Box
	UWidgetStudioFunctionLibrary::InterpSizeBoxMinOverrides(SizeBox, DimX, DimY, 0);

	// Smoothly update background color 
	UWidgetStudioFunctionLibrary::InterpImageColor(Background, BackgroundColor, AnimationTime);

	// Smoothly update selection overlay color 
	UWidgetStudioFunctionLibrary::InterpImageColor(SelectionOverlay, SelectionColor, AnimationTime);

	// Smoothly update DropShadow opacity 
	const float NewDropShadowOpacity = bEnableDropShadow ? IsHovered() ? 0.5f : 0.35f : 0.f;
	UWidgetStudioFunctionLibrary::InterpWidgetOpacity(BackgroundDropShadow, NewDropShadowOpacity, AnimationTime);

	// Smoothly update DropShadow location 
	const float NewDropShadowLoc = IsHovered() ? 5.f : 1.f;
	UWidgetStudioFunctionLibrary::InterpWidgetTranslation(BackgroundDropShadow, FVector2D(0, NewDropShadowLoc), AnimationTime);
	
	
	// Setup Selection Overlay 
	float Width = 0;
	float Height = 0;
	float TargetLocation;
	FVector2D TargetTranslation;

	if (SelectionStyle == ETabBarSelectionStyle::Full)
	{
		Width = CurrentButtonSize.X;
		Height = CurrentButtonSize.Y;
	}

	if (SelectionStyle == ETabBarSelectionStyle::Slim)
	{
		constexpr float SlimSize = 3.f;
		Width = Orientation == Orient_Horizontal ? CurrentButtonSize.X * .5 : SlimSize;
		Height = Orientation == Orient_Horizontal ? SlimSize : CurrentButtonSize.Y * .5;
	}

	if (Orientation == Orient_Horizontal)
	{
		TargetLocation = IsCurrentIndexValid ? Grid->GetChildAt(GetCurrentIndex())->GetPaintSpaceGeometry().GetLocalPositionAtCoordinates(FVector2D(0, 1)).X : 0;
		TargetTranslation = FVector2D(SelectionStyle == ETabBarSelectionStyle::Full ? TargetLocation : TargetLocation + (Width * .5), SelectionStyle == ETabBarSelectionStyle::Full ? 0 : CurrentButtonSize.Y - 3.f);
	}
	else
	{
		TargetLocation = IsCurrentIndexValid ? Grid->GetChildAt(GetCurrentIndex())->GetPaintSpaceGeometry().GetLocalPositionAtCoordinates(FVector2D(0, 0)).Y : 0;
		TargetTranslation = FVector2D(0, SelectionStyle == ETabBarSelectionStyle::Full ? TargetLocation : TargetLocation + (Height * .5) );
	}

	// Smoothly update selection overlay dimension overrides
	UWidgetStudioFunctionLibrary::InterpSizeBoxOverrides(SelectionOverlaySize, Width, Height, AnimationTime * 6);		

	// Smoothly update the selection overlays location.
	UWidgetStudioFunctionLibrary::InterpWidgetTranslation(SelectionOverlaySize, TargetTranslation, AnimationTime * 3);

	// Smoothly update overlay opacity
	const float NewSelectionOpacity = bSelectable == true && IsCurrentIndexValid ? 1.f : 0.f;
	UWidgetStudioFunctionLibrary::InterpWidgetOpacity(SelectionOverlay, NewSelectionOpacity, AnimationTime);
	
	return LayerId;
}

void UWidgetStudioModernTabBar::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	ConstructButtonGroup();
	UpdateStyling();
}

void UWidgetStudioModernTabBar::InitializeStyling()
{
	const float DimX = GetDimensions().X;
	const float DimY = GetDimensions().Y;

	if (SizeBox && ScrollBox && Overlay)
	{
		if (bEnableScrolling)
		{
			SizeBox->AddChild(ScrollBox);
			ScrollBox->AddChild(Overlay);
		}
		else
		{
			SizeBox->AddChild(Overlay);
		}
	}

	if (SizeBox)
	{
		SizeBox->SetMinDesiredWidth(DimX);
		SizeBox->SetMinDesiredHeight(DimY);
		Cast<UOverlaySlot>(SizeBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(SizeBox->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Setup Background Scale Box
	if (BackgroundScaleBox)
	{
		BackgroundScaleBox->SetStretch(EStretch::Fill);
		Cast<UOverlaySlot>(BackgroundScaleBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(BackgroundScaleBox->Slot)->SetVerticalAlignment(VAlign_Fill);
		Cast<UOverlaySlot>(BackgroundScaleBox->Slot)->SetPadding(0);
	}
	
	// Setup Background Overlay 
	if (BackgroundOverlay)
	{
		Cast<UScaleBoxSlot>(BackgroundOverlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UScaleBoxSlot>(BackgroundOverlay->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Setup DropShadow
	if (BackgroundDropShadow)
	{
		Cast<UOverlaySlot>(BackgroundDropShadow->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(BackgroundDropShadow->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Setup Background
	if (Background)
	{
		Cast<UOverlaySlot>(Background->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(Background->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
		
	// Setup Overlay Size Box
	if (SelectionOverlaySize)
	{
		SelectionOverlaySize->SetWidthOverride(0);
		SelectionOverlaySize->SetHeightOverride(0);
	}
	
	// Setup Selection Overlay
	if (SelectionOverlay)
	{
		Cast<USizeBoxSlot>(SelectionOverlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<USizeBoxSlot>(SelectionOverlay->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
		
	// Setup Grid
	if (Grid)
	{
		Cast<UOverlaySlot>(Grid->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(Grid->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
		
	// Setup Colors
	if (Background)
	{
		Background->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(BackgroundColor));
	}
	if (SelectionOverlay)
	{
		SelectionOverlay->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(SelectionColor));
	}
	
	ConstructButtonGroup();
}

void UWidgetStudioModernTabBar::UpdateStyling()
{
	const float DimY = GetDimensions().Y;
	const float RoundedSize = CornerStyle == ECornerStyle::Pill ? DimY : CornerStyle == ECornerStyle::Rounded ? FMath::Clamp(static_cast<float>(GetBorderRadius()), 0.f, GetDimensions().Y) : 0;
	
	// Setup Size Box + Size Constraint 

	if (SizeBox)
	{
		if (bEnableScrolling && SizeConstraint > 0)
		{
			if (Orientation == Orient_Horizontal)
			{
				SizeBox->SetMaxDesiredWidth(SizeConstraint);
				SizeBox->ClearMaxDesiredHeight();
			}
			else
			{
				SizeBox->SetMaxDesiredHeight(SizeConstraint);
				SizeBox->ClearMaxDesiredWidth();
			}
		}
		else
		{
			SizeBox->ClearMaxDesiredWidth();
			SizeBox->ClearMaxDesiredHeight();
		}
	}

	if (SelectionOverlay)
	{
		SelectionOverlaySize->SetWidthOverride(0);
		SelectionOverlaySize->SetHeightOverride(0);
	}
	
	// Setup Scrollbox
	if (ScrollBox)
	{
		ScrollBox->SetOrientation(Orientation);
		ScrollBox->SetScrollBarVisibility(ScrollBarVisibility);
	}

	// Setup BackgroundDropShadow
	if (BackgroundDropShadow)
	{
		BackgroundDropShadow->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedShadowMaterial());
		UWidgetStudioFunctionLibrary::SetBrushImageSize(BackgroundDropShadow, RoundedSize, RoundedSize);
		BackgroundDropShadow->SetColorAndOpacity(FLinearColor::Black);
		BackgroundDropShadow->SetRenderOpacity(bEnableDropShadow ? 0.35f : 0.f);

		FSlateBrush NewBrush = BackgroundDropShadow->GetBrush();
		NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		NewBrush.DrawAs = ESlateBrushDrawType::Box;
		BackgroundDropShadow->SetBrush(NewBrush);
	}

	// Setup Background
	if (Background)
	{
		Background->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
		UWidgetStudioFunctionLibrary::SetBrushImageSize(Background, RoundedSize, RoundedSize);

		FSlateBrush NewBrush = Background->GetBrush();
		NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		NewBrush.DrawAs = ESlateBrushDrawType::Box;
		Background->SetBrush(NewBrush);
	}

	// Setup SelectionOverlay
	if (SelectionOverlay)
	{
		const float NewOverlaySize = SelectionStyle == ETabBarSelectionStyle::Slim ? RoundedSize * 0.3f : RoundedSize;
		SelectionOverlay->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
		UWidgetStudioFunctionLibrary::SetBrushImageSize(SelectionOverlay, NewOverlaySize, NewOverlaySize);

		FSlateBrush NewBrush = SelectionOverlay->GetBrush();
		NewBrush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		NewBrush.DrawAs = ESlateBrushDrawType::Box;
		SelectionOverlay->SetBrush(NewBrush);

		SelectionOverlay->SetRenderOpacity(0);
	}

}

void UWidgetStudioModernTabBar::ConstructOption(const FButtonOptions Option)
{
	if (Grid && ButtonGroup)
	{
		UWidgetStudioModernButton* NewButton = CreateWidget<UWidgetStudioModernButton>(Grid, UWidgetStudioModernButton::StaticClass());

		if (!NewButton) {return ;}
		
		// Add button to Button Group for management
		ButtonGroup->AddButton(NewButton);

		// Add child to grid
		const float GridChildrenCount = Grid->GetChildrenCount();
		const int32 GridRow = Orientation == Orient_Horizontal ? 0 : GridChildrenCount;
		const int32 GridColumn =Orientation == Orient_Horizontal ? GridChildrenCount : 0;
		UUniformGridSlot* GridSlot = Grid->AddChildToUniformGrid(NewButton, GridRow, GridColumn);
		Cast<UUniformGridSlot>(NewButton->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UUniformGridSlot>(NewButton->Slot)->SetVerticalAlignment(VAlign_Fill);

		// Add grid slot styling
		GridSlot->SetHorizontalAlignment(HAlign_Fill);
		GridSlot->SetVerticalAlignment(VAlign_Fill);

		// Apply Defaults
		NewButton->OverrideDimensions = GetDimensions();
		NewButton->SizeModifier = SizeModifier;
		NewButton->SetButtonStyle(EButtonStyle::Content);
		NewButton->SetStandardBackgroundColor(ContentColor);
		NewButton->SetCheckedContentColor(BackgroundColor);
		NewButton->SetCheckedBackgroundColor(SelectionStyle == ETabBarSelectionStyle::Full ? BackgroundColor : SelectionColor);
		NewButton->SetCheckable(bSelectable);
		NewButton->SetChecked(Grid->GetChildrenCount() - 1 == GetCurrentIndex());
		NewButton->SetCheckedLockedState(Option.bIsCheckedStateLocked);
		NewButton->SetTextOptions(TextStyle);
		NewButton->SetIconOptions(IconStyle);
		NewButton->SetAlignment(ContentAlignment);
		NewButton->SetIconPlacement(IconPlacement);

		// Apply from Option
		NewButton->SetOptions(Option);
		
		
		// Apply Bindings
		NewButton->OnHoverStateChanged.AddDynamic(this, &UWidgetStudioModernTabBar::IndividualHoverStateChanged);
	}
}

TEnumAsByte<EHorizontalAlignment> UWidgetStudioModernTabBar::GetContentAlignment() const
{
	return ContentAlignment;
}

FWSTextStyle UWidgetStudioModernTabBar::GetTextOptions() const
{
	return TextStyle;
}

FWSIconStyle UWidgetStudioModernTabBar::GetIconOptions() const
{
	return IconStyle;
}

EWSPlacement UWidgetStudioModernTabBar::GetIconPlacement() const
{
	return IconPlacement;
}

void UWidgetStudioModernTabBar::SetContentAlignment(const TEnumAsByte<EHorizontalAlignment> NewAlignment)
{
	ContentAlignment = NewAlignment;
	SynchronizeProperties();
}

void UWidgetStudioModernTabBar::SetTextOptions(const FWSTextStyle NewTextOptions)
{
	TextStyle = NewTextOptions;
	SynchronizeProperties();
}

void UWidgetStudioModernTabBar::SetIconOptions(FWSIconStyle NewIconOptions)
{
	IconStyle = NewIconOptions;
	SynchronizeProperties();
}

void UWidgetStudioModernTabBar::SetIconPlacement(const EWSPlacement NewIconPlacement)
{
	IconPlacement = NewIconPlacement;
	SynchronizeProperties();
}

TEnumAsByte<EOrientation> UWidgetStudioModernTabBar::GetOrientation() const
{
	return Orientation;
}

ECornerStyle UWidgetStudioModernTabBar::GetCornerStyle() const
{
	return CornerStyle;
}

ETabBarSelectionStyle UWidgetStudioModernTabBar::GetSelectionStyle() const
{
	return SelectionStyle;
}

EPalette UWidgetStudioModernTabBar::GetBackgroundColor() const
{
	return BackgroundColor;
}

EPalette UWidgetStudioModernTabBar::GetContentColor() const
{
	return ContentColor;
}

EPalette UWidgetStudioModernTabBar::GetSelectionColor() const
{
	return SelectionColor;
}

bool UWidgetStudioModernTabBar::IsSelectable() const
{
	return bSelectable;
}

float UWidgetStudioModernTabBar::IsScrollable() const
{
	return bEnableScrolling;
}

void UWidgetStudioModernTabBar::SetScrollable(const bool NewState)
{
	bEnableScrolling = NewState;
	SynchronizeProperties();
}

float UWidgetStudioModernTabBar::GetSizeConstraint() const
{
	return SizeConstraint;
}

ESlateVisibility UWidgetStudioModernTabBar::GetScrollbarVisibility() const
{
	return ScrollBarVisibility;
}

void UWidgetStudioModernTabBar::SetScrollbarVisibility(const ESlateVisibility NewVisibility)
{
	ScrollBarVisibility = NewVisibility;
	SynchronizeProperties();
}

void UWidgetStudioModernTabBar::SetSizeConstraint(const float NewSize)
{
	SizeConstraint = NewSize;
	SynchronizeProperties();
}

void UWidgetStudioModernTabBar::SetOrientation(const TEnumAsByte<EOrientation> NewOrientation)
{
	Orientation = NewOrientation;
	SynchronizeProperties();
}

void UWidgetStudioModernTabBar::SetCornerStyle(const ECornerStyle NewStyle)
{
	CornerStyle = NewStyle;
	SynchronizeProperties();
}

void UWidgetStudioModernTabBar::SetBackgroundColor(const EPalette NewColor)
{
	BackgroundColor = NewColor;
}

void UWidgetStudioModernTabBar::SetContentColor(const EPalette NewColor)
{
	ContentColor = NewColor;
}

void UWidgetStudioModernTabBar::SetSelectionColor(const EPalette NewColor)
{
	SelectionColor = NewColor;
}

void UWidgetStudioModernTabBar::SetSelectable(const bool NewState)
{
	bSelectable = NewState;
}

void UWidgetStudioModernTabBar::SetCurrentIndex(const int32 Index, const bool bBroadcast)
{
	Super::SetCurrentIndex(Index, bBroadcast);
	ButtonGroup->SetCurrentIndex(Index, false);
}

void UWidgetStudioModernTabBar::ClearOptions()
{
	if (Grid)
	{
		Grid->ClearChildren();
	}

	if (ButtonGroup)
	{
		ButtonGroup->RemoveAllButtons();
	}

	Super::ClearOptions();
	SynchronizeProperties();
}

void UWidgetStudioModernTabBar::ConstructButtonGroup()
{
	if (ButtonGroup && Grid)
	{
		// Bind Button Group to Current Index Function
		ButtonGroup->SetSelectionMethod(bSelectable ? EClusivity::Exclusive : EClusivity::Inclusive);
		ButtonGroup->OnCurrentIndexChanged.RemoveAll(this);
		ButtonGroup->OnCurrentIndexChanged.AddDynamic(this, &UWidgetStudioModernTabBar::UpdateIndexFromButtonGroup);

		// Clear the grid of all children to ready it for applying the options without error.
		ButtonGroup->RemoveAllButtons();
		Grid->ClearChildren();

		// Construct options
		for (int32 i = 0; i < GetOptionCount(); i++)
		{
			ConstructOption(GetOptions()[i]);
		}

		ButtonGroup->SetCurrentIndex(GetCurrentIndex(), false);
	}
}

void UWidgetStudioModernTabBar::UpdateIndexFromButtonGroup(const int32 NewIndex)
{
	SetCurrentIndex(NewIndex, true);
}