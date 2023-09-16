/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/


#include "Widgets/Modern/WSModernComboBox.h"

#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/OverlaySlot.h"
#include "Components/ScaleBoxSlot.h"
#include "Components/SizeBoxSlot.h"
#include "Components/VerticalBoxSlot.h"
#include "Widgets/Modern/WSModernTabBar.h"

TSharedRef<SWidget> UWidgetStudioModernComboBox::RebuildWidget()
{
	UPanelWidget* RootWidget = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("RootWidget"));
	WidgetTree->RootWidget = RootWidget;
	
	TSharedRef<SWidget> Widget = Super::RebuildWidget();

	if (RootWidget && WidgetTree)
	{

		/*
		* RootWidget (Overlay)
		* - VBox
		* -- Label Item
		* -- Content Size Box
		* --- Content Scale Box
		* ---- Content Overlay
		* ----- DropShadow
		* ----- Background
		* ---- HBox
		* ----- Icon Item
		* ----- Text Item
		* ----- Arrow Item
		* --- Menu Anchor
		*/
		
		// Construct Widgets 
		VBox = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("VBox"));
		LabelItem = WidgetTree->ConstructWidget<UWidgetStudioText>(UWidgetStudioText::StaticClass(), TEXT("Label Item"));
		ContentSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Content Size Box"));
		ContentScaleBox = WidgetTree->ConstructWidget<UScaleBox>(UScaleBox::StaticClass(), TEXT("Content Scale Box"));
		ContentOverlay = WidgetTree->ConstructWidget<UOverlay>(UOverlay::StaticClass(), TEXT("Content Overlay"));
		DropShadow = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("DropShadow"));
		Background = WidgetTree->ConstructWidget<UImage>(UImage::StaticClass(), TEXT("Background"));
		HBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("HBox"));
		IconItem = WidgetTree->ConstructWidget<UWidgetStudioIcon>(UWidgetStudioIcon::StaticClass(), TEXT("Icon Item"));
		TextItem = WidgetTree->ConstructWidget<UWidgetStudioText>(UWidgetStudioText::StaticClass(), TEXT("Text Item"));
		ArrowItem = WidgetTree->ConstructWidget<UWidgetStudioIcon>(UWidgetStudioIcon::StaticClass(), TEXT("Arrow Item"));
		MenuAnchor = WidgetTree->ConstructWidget<UMenuAnchor>(UMenuAnchor::StaticClass(), TEXT("Menu Anchor"));

				
		// Construct Hierarchy 	
		RootWidget->AddChild(VBox);

		VBox->AddChild(LabelItem);
		VBox->AddChild(ContentSizeBox);

		ContentSizeBox->AddChild(ContentScaleBox);
		ContentScaleBox->AddChild(ContentOverlay);
		ContentOverlay->AddChild(DropShadow);
		ContentOverlay->AddChild(Background);
		ContentOverlay->AddChild(HBox);
		ContentOverlay->AddChild(MenuAnchor);

		HBox->AddChild(IconItem);
		HBox->AddChild(TextItem);
		HBox->AddChild(ArrowItem);
		
		// Additional Construction Parameters
		MinimumDimensions = FVector2D(225, 35);

		InitializeStyling();
	}
	return Widget;
}

int32 UWidgetStudioModernComboBox::NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry,
	const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId,
	const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const
{
	if (bDisablePainting) { return LayerId; }
	
	// Smoothly Update SizeBox 
	UWidgetStudioFunctionLibrary::InterpSizeBoxMinOverrides(ContentSizeBox, GetDimensions().X, GetDimensions().Y, AnimationTime);
	//UWidgetStudioFunctionLibrary::InterpSizeBoxOverrides(ContentSizeBox, 0, GetDimensions().Y, AnimationTime);
	
	// Smoothly update DropShadow opacity 
	const float NewDropShadowOpacity = MenuAnchor->IsOpen() || IsHovered() ? 0.5f : 0.35f;
	UWidgetStudioFunctionLibrary::InterpWidgetOpacity(DropShadow, NewDropShadowOpacity, AnimationTime);
	
	// Smoothly update DropShadow location 
	const float NewDropShadowLoc = MenuAnchor->IsOpen() || IsHovered() ? 5.f : 1.f;
	UWidgetStudioFunctionLibrary::InterpWidgetTranslation(DropShadow, FVector2D(0, NewDropShadowLoc), AnimationTime);
	
	UWidgetStudioFunctionLibrary::InterpImageColor(Background, BackgroundColor, AnimationTime);
	UWidgetStudioFunctionLibrary::InterpWidgetColor(IconItem, ContentColor, AnimationTime);
	UWidgetStudioFunctionLibrary::InterpWidgetColor(ArrowItem, ContentColor, AnimationTime);
	UWidgetStudioFunctionLibrary::InterpWidgetRotation(ArrowItem, MenuAnchor->IsOpen() ? 180 : 0, AnimationTime * 4);
	UWidgetStudioFunctionLibrary::InterpWidgetColor(LabelItem, LabelColor, AnimationTime);
	UWidgetStudioFunctionLibrary::InterpWidgetColor(TextItem, ContentColor, AnimationTime);
	UWidgetStudioFunctionLibrary::InterpWidgetOpacity(TextItem, GetCurrentIndex() != -1 || bIsSelectable ? 1.0f : 0.5f, AnimationTime);
	
	return LayerId;
}

void UWidgetStudioModernComboBox::SynchronizeProperties()
{
	Super::SynchronizeProperties();
	UpdateStyling();
}

void UWidgetStudioModernComboBox::InitializeStyling()
{
	Super::InitializeStyling();

	// Cache the size properties
	const float SizeY = GetDimensions().Y;
	const float RoundedSize = CornerStyle == ECornerStyle::Pill ? SizeY : CornerStyle == ECornerStyle::Rounded ? FMath::Clamp(static_cast<float>(GetBorderRadius()), 0.f, GetDimensions().Y) : 0;

	// Initialize the VBox widget styling
	if (VBox)
	{
		Cast<UOverlaySlot>(VBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(VBox->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Initialize the LabelItem widget styling
	if (LabelItem)
	{
		LabelItem->bDisablePainting = true;
		LabelItem->SetColor(EPalette::White);
		LabelItem->SetType(EWSFontType::Caption);
		LabelItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(LabelColor));
		
		Cast<UVerticalBoxSlot>(LabelItem->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UVerticalBoxSlot>(LabelItem->Slot)->SetVerticalAlignment(VAlign_Fill);
		Cast<UVerticalBoxSlot>(LabelItem->Slot)->SetPadding(FMargin(0, 0, 0, 3));
	}
	
	// Initialize the ContentSizeBox widget styling
	if (ContentSizeBox)
	{
		ContentSizeBox->SetMinDesiredWidth(GetDimensions().X);
		ContentSizeBox->SetMinDesiredHeight(SizeY);

		Cast<UVerticalBoxSlot>(ContentSizeBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UVerticalBoxSlot>(ContentSizeBox->Slot)->SetVerticalAlignment(VAlign_Fill);
		//Cast<UVerticalBoxSlot>(ContentSizeBox->Slot)->SetSize(ESlateSizeRule::Fill);
	}
	
	// Initialize the ContentScaleBox widget styling
	if (ContentScaleBox)
	{
		ContentScaleBox->SetStretch(CornerStyle == ECornerStyle::Rounded || CornerStyle == ECornerStyle::Pill ? EStretch::Fill : EStretch::ScaleToFill);
		
		Cast<USizeBoxSlot>(ContentScaleBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<USizeBoxSlot>(ContentScaleBox->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Initialize the ContentOverlay widget styling
	if (ContentOverlay)
	{
		Cast<UScaleBoxSlot>(ContentOverlay->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UScaleBoxSlot>(ContentOverlay->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Initialize the DropShadow widget styling
	if (DropShadow)
	{
		DropShadow->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedShadowMaterial());
		UWidgetStudioFunctionLibrary::SetBrushImageSize(DropShadow, RoundedSize, RoundedSize);
		DropShadow->SetColorAndOpacity(FLinearColor::Black);
		DropShadow->SetRenderOpacity(0.35f);
		FSlateBrush Brush = DropShadow->GetBrush();
		Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		Brush.DrawAs = ESlateBrushDrawType::Box;
		DropShadow->SetBrush(Brush);
		
		Cast<UOverlaySlot>(DropShadow->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(DropShadow->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Initialize the Background widget styling
	if (Background)
	{
		Background->SetBrushFromMaterial(UWidgetStudioFunctionLibrary::GetRoundedBackgroundMaterial());
		UWidgetStudioFunctionLibrary::SetBrushImageSize(Background, RoundedSize, RoundedSize);
		Background->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(BackgroundColor));
		FSlateBrush Brush = Background->GetBrush();
		Brush.Margin = FVector4(0.5f, 0.5f, 0.5f, 0.5f);
		Brush.DrawAs = ESlateBrushDrawType::Box;
		Background->SetBrush(Brush);
		Background->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(BackgroundColor));

		
		Cast<UOverlaySlot>(Background->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(Background->Slot)->SetVerticalAlignment(VAlign_Fill);
	}
	
	// Initialize the HBox widget styling
	if (HBox)
	{
		Cast<UOverlaySlot>(HBox->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(HBox->Slot)->SetVerticalAlignment(VAlign_Center);
		Cast<UOverlaySlot>(HBox->Slot)->SetPadding(FMargin(15, 0, 15, 0));
	}
	
	// Initialize the IconItem widget styling
	if (IconItem)
	{
		IconItem->bDisablePainting = true;
		IconItem->SetColor(EPalette::White);
		IconItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(ContentColor));
		
		Cast<UHorizontalBoxSlot>(IconItem->Slot)->SetHorizontalAlignment(HAlign_Left);
		Cast<UHorizontalBoxSlot>(IconItem->Slot)->SetVerticalAlignment(VAlign_Center);
		Cast<UHorizontalBoxSlot>(IconItem->Slot)->SetPadding(FMargin(0, 0, 15, 0));
	}
	
	// Initialize the ArrowItem widget styling
	if (ArrowItem)
	{
		ArrowItem->bDisablePainting = true;
		ArrowItem->SetColor(EPalette::White);
		ArrowItem->SetIcon(EIconItem::ChevronDown);
		ArrowItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(ContentColor));
		
		Cast<UHorizontalBoxSlot>(ArrowItem->Slot)->SetHorizontalAlignment(HAlign_Right);
		Cast<UHorizontalBoxSlot>(ArrowItem->Slot)->SetVerticalAlignment(VAlign_Center);
	}
	
	// Initialize the TextItem widget styling
	if (TextItem)
	{
		TextItem->bDisablePainting = true;
		TextItem->SetColor(EPalette::White);
		TextItem->SetColorAndOpacity(UWidgetStudioFunctionLibrary::GetColorFromPalette(ContentColor));
		
		Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetVerticalAlignment(VAlign_Center);
		Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetSize(ESlateSizeRule::Fill);
		Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetPadding(FMargin(0, 0, 15, 0));
	}
	
	// Initialize the MenuAnchor widget styling
	if (MenuAnchor)
	{
		MenuAnchor->SetPlacement(MenuPlacement_ComboBox);
		MenuAnchor->FitInWindow(true);
		
		Cast<UOverlaySlot>(MenuAnchor->Slot)->SetHorizontalAlignment(HAlign_Fill);
		Cast<UOverlaySlot>(MenuAnchor->Slot)->SetVerticalAlignment(VAlign_Bottom);

		MenuAnchor->OnGetUserMenuContentEvent.BindUFunction(this, "ConstructMenu");
	}
}

void UWidgetStudioModernComboBox::UpdateStyling()
{
	Super::UpdateStyling();

	// Update the ContentSizeBox widget styling
	if (ContentSizeBox)
	{
		ContentSizeBox->SetMinDesiredWidth(GetDimensions().X);
		ContentSizeBox->SetMinDesiredHeight(GetDimensions().Y);
	}
	
	// Update the LabelItem widget styling
	if (LabelItem)
	{
		LabelItem->SetText(Label);
		LabelItem->SetVisibility(Label.IsEmpty() ? ESlateVisibility::Collapsed : ESlateVisibility::HitTestInvisible);

		LabelItem->SizeModifier = SizeModifier;
	}
	
	// Update the TextItem widget styling
	if (TextItem && IconItem)
	{
		TextItem->SizeModifier = SizeModifier;
		TextItem->SetTextStyle(TextStyle);
		Cast<UHorizontalBoxSlot>(TextItem->Slot)->SetHorizontalAlignment(ContentAlignment);
		
		IconItem->SizeModifier = SizeModifier;
		IconItem->SetSize(IconStyle.Size);
		IconItem->SetUseNativeColor(IconStyle.bUseNativeColor);
		Cast<UHorizontalBoxSlot>(IconItem->Slot)->SetHorizontalAlignment(ContentAlignment);
		
		if (GetCurrentIndex() > -1 && bIsSelectable)
		{
			const auto Option = GetCurrentOption();
			TextItem->SetText(Option.Text);
			TextItem->SetVisibility(Option.ContentStyle == EButtonContentStyle::Label || Option.ContentStyle == EButtonContentStyle::IconWithLabel ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Hidden);

			IconItem->SetCustomIcon(UWidgetStudioFunctionLibrary::GetTextureFromIconLibrary(Option.Icon));
			IconItem->SetVisibility(Option.ContentStyle == EButtonContentStyle::Icon || Option.ContentStyle == EButtonContentStyle::IconWithLabel ? ESlateVisibility::SelfHitTestInvisible : ESlateVisibility::Collapsed);
		}
		else
		{
			TextItem->SetVisibility(ESlateVisibility::HitTestInvisible);
			TextItem->SetText(PlaceholderText);

			IconItem->SetCustomIcon(nullptr);
			IconItem->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	
	// Update the ArrowItem widget styling
	if (ArrowItem)
	{
		ArrowItem->SizeModifier = SizeModifier;
		ArrowItem->SetSize(IconStyle.Size);
	}
}

void UWidgetStudioModernComboBox::SetCurrentIndex(const int32 Index, const bool bBroadcast)
{
	Super::SetCurrentIndex(Index, bBroadcast);
	SynchronizeProperties();
	UpdateStyling();
}

FReply UWidgetStudioModernComboBox::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.IsMouseButtonDown(FKey("LeftMouseButton")))
	{
		MenuAnchor->Open(true);

		return FReply::Handled();
	}
	return FReply::Unhandled();
}

void UWidgetStudioModernComboBox::OnMenuButtonSelected(const int32 Index, FButtonOptions Option)
{
	SetCurrentIndex(Index, true);
	SynchronizeProperties();
	MenuAnchor->Close();
}

UUserWidget* UWidgetStudioModernComboBox::ConstructMenu() const
{
	UWidgetStudioModernTabBar* MenuBar = CreateWidget<UWidgetStudioModernTabBar>(MenuAnchor);
	MenuBar->SetScrollable(true);
	MenuBar->SetSizeConstraint(MenuHeight);
	MenuBar->SizeModifier = SizeModifier;
	MenuBar->OverrideDimensions = OverrideDimensions;
	MenuBar->SetOrientation(Orient_Vertical);
	MenuBar->SetScrollbarVisibility(ESlateVisibility::Collapsed);
	MenuBar->SetSelectable(bIsSelectable);
	MenuBar->SetBackgroundColor(BackgroundColor);
	MenuBar->SetContentColor(ContentColor);
	MenuBar->SetSelectionColor(SelectionColor);
	MenuBar->SetTextOptions(TextStyle);
	MenuBar->SetIconOptions(IconStyle);
	MenuBar->SetContentAlignment(ContentAlignment);
	MenuBar->SetIconPlacement(IconPlacement);
	MenuBar->SetOptions(GetOptions());
	MenuBar->ForceStyleUpdate();
	
	MenuBar->OnCurrentIndexChanged.AddDynamic(this, &UWidgetStudioModernComboBox::OnMenuButtonSelected);
	MenuBar->OnHoverStateChanged.AddDynamic(this, &UWidgetStudioModernComboBox::IndividualHoverStateChanged);

	return MenuBar;
}

FText UWidgetStudioModernComboBox::GetLabel() const
{
	return Label;
}

FText UWidgetStudioModernComboBox::GetPlaceholderText() const
{
	return PlaceholderText;
}

ECornerStyle UWidgetStudioModernComboBox::GetCornerStyle() const
{
	return CornerStyle;
}

EPalette UWidgetStudioModernComboBox::GetBackgroundColor() const
{
	return BackgroundColor;
}

EPalette UWidgetStudioModernComboBox::GetContentColor() const
{
	return ContentColor;
}

EPalette UWidgetStudioModernComboBox::GetSelectionColor() const
{
	return SelectionColor;
}

EPalette UWidgetStudioModernComboBox::GetLabelColor() const
{
	return LabelColor;
}

bool UWidgetStudioModernComboBox::IsSelectable() const
{
	return bIsSelectable;
}

void UWidgetStudioModernComboBox::SetLabel(const FText NewLabel)
{
	Label = NewLabel;
	SynchronizeProperties();
}

void UWidgetStudioModernComboBox::SetPlaceholderText(const FText NewText)
{
	PlaceholderText = NewText;
}

void UWidgetStudioModernComboBox::SetCornerStyle(const ECornerStyle NewStyle)
{
	CornerStyle = NewStyle;
}

void UWidgetStudioModernComboBox::SetBackgroundColor(const EPalette NewColor)
{
	BackgroundColor = NewColor;
}

void UWidgetStudioModernComboBox::SetContentColor(const EPalette NewColor)
{
	ContentColor = NewColor;
}

void UWidgetStudioModernComboBox::SetSelectionColor(const EPalette NewColor)
{
	SelectionColor = NewColor;
}

void UWidgetStudioModernComboBox::SetLabelColor(const EPalette NewColor)
{
	LabelColor = NewColor;
}

void UWidgetStudioModernComboBox::SetSelectable(const bool NewState)
{
	bIsSelectable = NewState;
}
