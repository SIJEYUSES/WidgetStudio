/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/

#pragma once

#include "CoreMinimal.h"
#include "Widgets/WSBase.h"
#include "Widgets/Custom/WSTextBlock.h"
#include "WSText.generated.h"

/**
 * The settings for the Widget Studio Text.
 */

USTRUCT(BlueprintType)
struct FWSTextStyle
{
	GENERATED_BODY()

	/** The font type that'll govern the style of the text.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget Studio")
	EWSFontType Type;

	/** A custom font style. Useful for one-off styles. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget Studio", Meta = (
		EditCondition = "Type == EWSFontType::Custom",
		EditConditionHides
	))
	FFontStyle CustomStyle;

	/** The case of the text. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget Studio")
	EFontCase Case;

	/** The growth direction and alignment of the text.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget Studio")
	TEnumAsByte<ETextJustify::Type> Justification;

	/** Automatically wraps the text to the next line when it reaches the end of available horizontal space.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget Studio|Text Wrap")
	bool AutoWrapText;
	
	/** Manually wraps the text at the given distance. Ignored if set to 0.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget Studio|Text Wrap", Meta=(
		ClampMin = "0",
		UIMin = "0",
		ClampMax = "10000",
		UIMax = "10000",
		EditCondition="AutoWrapText == true",
		EditConditionHides
	))
	float WrapTextAt;

	/** How the text should wrap.*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Widget Studio|Text Wrap", Meta = (
		EditCondition="AutoWrapText == true",
		EditConditionHides
	))
	ETextWrappingPolicy WrappingPolicy;
	
	FWSTextStyle(
		const EWSFontType InType = EWSFontType::Body1,
		const FFontStyle& InCustomStyle = FFontStyle(),
		const EFontCase InCase = EFontCase::Sentence,
		const ETextJustify::Type InJustification = ETextJustify::Left,
		const bool InAutoWrapText = true,
		const float InWrapTextAt = 0,
		const ETextWrappingPolicy InWrappingPolicy = ETextWrappingPolicy::DefaultWrapping)
	{
		Type = InType;
		CustomStyle = InCustomStyle;
		Case = InCase;
		Justification = InJustification;
		AutoWrapText = InAutoWrapText;
		WrapTextAt = InWrapTextAt;
		WrappingPolicy = InWrappingPolicy;
	}
};

/**
 * A custom text widget setup to work with the Typography Styling.
 */

UCLASS()
class WIDGETSTUDIORUNTIME_API UWidgetStudioText : public UWidgetStudioBase
{
	GENERATED_BODY()

protected:
	
	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual int32 NativePaint(const FPaintArgs& Args, const FGeometry& AllottedGeometry, const FSlateRect& MyCullingRect, FSlateWindowElementList& OutDrawElements, int32 LayerId, const FWidgetStyle& InWidgetStyle, bool bParentEnabled) const override;
	virtual void SynchronizeProperties() override;
	virtual void InitializeStyling() override;
	virtual void UpdateStyling() override;

	UPROPERTY()
	FSlateFontInfo FontInfo;

	// Widget Components

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	USizeBox* SizeBox = nullptr;

	UPROPERTY(BlueprintReadOnly, Category = "Widgets")
	UWidgetStudioTextBlock* TextItem = nullptr;
	

	// Properties
	
	/** The text that is displayed. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta=(MultiLine=true))
	FText Text = FText::FromString("Placeholder");

	/** The display options for the text. Will eventually replace the base variables. */
	UPROPERTY(Meta = (ShowOnlyInnerProperties))
	FWSTextStyle TextStyle;

	/** The color of the text. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Color")
	EPalette Color = EPalette::PrimaryContent;

	

	/** The font type that'll govern the style of the text.*/
	UPROPERTY(EditAnywhere, Category="Widget Studio")
	EWSFontType Type = EWSFontType::Body1;

	/** A custom font style. Useful for one-off styles. */
	UPROPERTY(EditAnywhere, Category="Widget Studio", Meta = (EditCondition = "Type == EWSFontType::Custom", EditConditionHides))
	FFontStyle CustomStyle;

	/** Set the case style of the text that is displayed.*/
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	EFontCase Case = EFontCase::Sentence;

	/** The growth direction and alignment of the text.*/
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	TEnumAsByte<ETextJustify::Type> Justification = ETextJustify::Left;

	/** Automatically wraps the text to the next line when it reaches the end of available horizontal space.*/
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Text Wrap")
	bool AutoWrapText = false;
	
	/** Manually wraps the text at the given distance. Ignored if set to 0.*/
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Text Wrap",Meta=(ClampMin = "0", UIMin = "0"))
	float WrapTextAt = 0;

	/** How the text should wrap.*/
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Text Wrap")
	ETextWrappingPolicy WrappingPolicy = ETextWrappingPolicy::DefaultWrapping;


public:
	
	// Helpers

	/** Returns all of the text options. */
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	FWSTextStyle GetTextOptions() const;

	/** Returns the displayed text.*/
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	FText GetText() const;

	/** Returns true if the text is empty.*/
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsTextEmpty() const;

	/** Returns the justification of the text.*/
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	ETextJustify::Type GetJustification() const;

	/** Returns how long the text is (in characters).*/
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	int32 Length() const;

	/** Returns the text type used.*/
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EWSFontType GetType() const;

	/** Returns the custom text style.*/
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	FFontStyle GetCustomStyle() const;

	/** Returns true if the text is automatically being wrapping.*/
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	bool IsAutoWrappingText() const;

	/** Returns the set distance to wrap text at.*/
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	float GetWrapTextAt() const;

	/** Returns the set wrapping policy.*/
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	ETextWrappingPolicy GetWrapPolicy() const;

	/** Returns the set case of the text.*/
	UFUNCTION(BlueprintPure, Category="Widget Studio|Helper")
	EFontCase GetCase() const;


	// Modifier Functions

	/**
	 * Set all of the text options
	 * @param NewTextOptions The new text options to set.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetTextStyle(FWSTextStyle NewTextOptions);

	/**
	 * Set the text that will be displayed by the widget.
	 * @param NewText The new text to display on the widget.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetText(FText NewText);

	/**
	 * Set the growth direction and alignment of the text.
	 * @param NewJustification The new text justification to apply to the text.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetJustification(ETextJustify::Type NewJustification);

	/**
	 * Set the color of the text.
	 * @param NewColor The new color to apply to the text.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetColor(EPalette NewColor);

	/**
	 * Set the font type that'll govern the style of the text.
	 * @param NewType The new font type to apply to the text.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetType(EWSFontType NewType);

	/**
	 * Set the custom font style. Will only take effect if the Font Type is set to Custom.
	 * @param NewStyle The new font style to apply to the text.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetCustomStyle(FFontStyle NewStyle);

	/**
	 * Set the text to automatically wrap or not.
	 * @param NewState The new auto wrap state to apply to the text.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetAutoWrapText(bool NewState);

	/**
	 * Set where to wrap the text at. Ignored if set to 0.
	 * @param NewWrapAt The new wrap at location.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetWrapTextAt(float NewWrapAt);

	/**
	 * Set how the text should be wrapped.
	 * @param NewPolicy The new wrap policy to apply to the text.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetWrapPolicy(ETextWrappingPolicy NewPolicy);

	/**
	 * Set the case style of the text.
	 * @param NewCase The new case to apply to the text.
	 */
	UFUNCTION(BlueprintCallable, Category="Widget Studio|Modifier")
	void SetCase(EFontCase NewCase);
};