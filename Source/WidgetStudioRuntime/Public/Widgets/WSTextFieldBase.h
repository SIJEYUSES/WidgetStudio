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
#include "Basic/WSText.h"
#include "Widgets/WSBase.h"
#include "WSTextFieldBase.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWSTextFieldTextChangedDelegate, const FText&, Text);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FWSTextFieldTextCommittedDelegate, const FText&, Text, ETextCommit::Type, CommitMethod);

UENUM(BlueprintType)
enum class ETextFilter : uint8
{
	None			UMETA(DisplayName="None"),
	TextOnly		UMETA(DisplayName="Text Only"),
	NumbersOnly		UMETA(DisplayName="Numbers Only"),

	TextFilter_Max	UMETA(Hidden),
};

UENUM(BlueprintType)
enum class ETextFieldIconVisibility : uint8
{
	Hidden					UMETA(DisplayName="Hidden"),
	Leading					UMETA(DisplayName="Leading"),
	Trailing				UMETA(DisplayName="Trailing"),
	LeadingAndTrailing		UMETA(DisplayName="Leading & Trailing"),

	TextFieldStyle_Max		UMETA(Hidden),
};

UENUM(BlueprintType)
enum class ETextFieldState : uint8
{
	Normal		UMETA(DisplayName="Normal"),
	Focused		UMETA(DisplayName="Focused"),
	Accepted	UMETA(DisplayName="Accepted"),
	Warning		UMETA(DisplayName="Warning"),
	Error		UMETA(DisplayName="Error"),

	TextFieldState_Max	UMETA(Hidden),
};

/**
 * A Widget Studio abstract text field without any pre-defined styling or widgets.
 * See WSModernTextField on how to setup a new custom text field.
 */
UCLASS()
class WIDGETSTUDIORUNTIME_API UWidgetStudioTextFieldBase : public UWidgetStudioBase
{
	GENERATED_BODY()

protected:
	UWidgetStudioTextFieldBase(const FObjectInitializer& ObjectInitializer);
	
	/* Overrides */
	virtual void SynchronizeProperties() override;

	/* Delegates */

	/**
	 * Called when the text within the field has changed.
	 * @param InText The new text within the text field.
	 */
	UFUNCTION()
	virtual void OnTextInputTextChanged(const FText& InText);
	
	/**
	 * Called when the text field has been committed.
	 * @param InText The new text within the text field.
	 * @param InCommitMethod The method used to commit the text.
	 */
	UFUNCTION()
	virtual void OnTextInputTextCommitted(const FText& InText, ETextCommit::Type InCommitMethod);

	
	/* Properties */

	/**
	 * Enable/disable multi-line support.
	 * Note, not compatible with password mode.
	 */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", meta=(DesignerRebuild="True", ForceRebuildProperty="Text"))
	bool bMultiLine = false;

	/** The label of the text field. If empty, the label will be hidden. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	FText Label;
	
	/** The text displayed in the Text Field when no text has been inputted. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	FText PlaceholderText = FText().FromString("Enter text here!");

	/** The text displayed in the Text Field. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta = (MultiLine = bMultiLine))
	FText Text = FText().FromString("");

	/** The text styling to apply to the text within the text field. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	FWSTextStyle TextStyle = FWSTextStyle(
		EWSFontType::Body2,
		FFontStyle(),
		EFontCase::Sentence,
		ETextJustify::Left,
		true,
		0,
		ETextWrappingPolicy::DefaultWrapping);
	
	/** The current state of the text field. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	ETextFieldState State = ETextFieldState::Normal;

	/** How the text should be filtered. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio")
	ETextFilter Filter = ETextFilter::None;

	/** The limit on how many characters should allowed. 0 = Uncapped */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta = (ClampMin = "0", UIMin = "0"))
	int32 CharacterLimit = 0;

	/** Set the Text Field to password mode. */
	UPROPERTY(EditAnywhere, Category = "Widget Studio", Meta=(EditCondition = "!bMultiLine", EditConditionHides))
	bool bIsPasswordField = false;

public:

	/* Bindings */
	
	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
	FWSTextFieldTextChangedDelegate OnTextChanged;

	UPROPERTY(BlueprintAssignable, Category = "Widget Studio|Event")
	FWSTextFieldTextCommittedDelegate OnTextCommitted;

	
	/* Helpers */

	/** Returns the label. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	FText GetLabel() const;

	/** Returns the text in the text field. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	FText GetText() const;

	/** Returns the placeholder text. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	FText GetPlaceholderText() const;

	/** Returns the current set text filter. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	ETextFilter GetFilter() const;

	/** Returns the character limit. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	int32 GetCharacterLimit() const;

	/** Returns the current Text Field State. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	ETextFieldState GetState() const;

	/** Returns true if the text field is set to be a password field. */
	UFUNCTION(BlueprintPure, Category = "Widget Studio|Helper")
	bool IsPasswordField() const;

	
	/* Modifiers */

	/**
	 * Sets the label that appears above the text field.
	 * @param NewLabel The new text to apply to the label.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Deprecated",Meta=(DeprecatedProperty, DeprecationMessage="Per-widget labeling is no longer supported. Place this widget in the Label Widget instead."))
	virtual void SetLabel(FText NewLabel);

	/**
	 * Sets the text in the Text Field.
	 * @param NewText The new text to display in the Text Field.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	virtual void SetText(FText NewText);

	/**
	 * Sets the placeholder text in the Text Field.
	 * @param NewText The new placeholder text to display in the Text Field.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	virtual void SetPlaceholderText(FText NewText);

	/**
	 * Sets and applies a new filter for the text in the text field.
	 * @param NewFilter The new filter to apply to the Text Field.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	virtual void SetFilter(ETextFilter NewFilter);

	/**
	 * Sets and applies the character limit. If set to 0, then no limit is imposed.
	 * @param NewLimit The new character limit to apply to the Text Field.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	virtual void SetCharacterLimit(int32 NewLimit);

	/**
	 * Manually set the state of the Text Field.
	 * @param NewState The new state to apply to the Text Field.
	 */
	UFUNCTION(BlueprintCallable, Category = "Widget Studio|Modifier")
	virtual void SetState(ETextFieldState NewState);
	
};
