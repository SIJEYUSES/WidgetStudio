/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/


#include "Widgets/Utility/WSButtonGroup.h"

void UWidgetStudioButtonGroup::NativePreConstruct()
{
	Super::NativePreConstruct();

	// Additional Construction Parameters
	bCanOverrideDimensions = false;
	bCanOverrideBorderRadius = false;

	ManagedButtons = Buttons;

	SynchronizeProperties();
}

void UWidgetStudioButtonGroup::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Clears out managed buttons.
	ManagedButtons.Empty();
	
	for (int i = 0; i < Buttons.Num(); i++)
	{
		AddButton(Buttons[i]);
	}

	SynchronizeProperties();
}

void UWidgetStudioButtonGroup::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	for (int i = 0; i < ManagedButtons.Num(); i++)
	{
		if (Cast<UWidgetStudioModernCheckBox>(ManagedButtons[i]))
		{
			Cast<UWidgetStudioModernCheckBox>(ManagedButtons[i])->SetSelectionMethod(SelectionMethod);
		}
	}

	UpdateButtonCheckedStates();
}

void UWidgetStudioButtonGroup::OnButtonPressed(UWidgetStudioButtonBase* Button)
{
	CurrentButton = Button;
	UpdateCurrentIndex();
	UpdateButtonCheckedStates();
}

void UWidgetStudioButtonGroup::UpdateCurrentIndex()
{
	if (IsValid(CurrentButton))
	{
		CurrentIndex = ManagedButtons.Find(CurrentButton);
		
	}
	else
	{
		CurrentIndex = -1;
	}
	OnCurrentIndexChanged.Broadcast(CurrentIndex);

	SynchronizeProperties();
}

void UWidgetStudioButtonGroup::UpdateButtonCheckedStates()
{
	if (SelectionMethod != EClusivity::Exclusive) { return; }
	if (ManagedButtons.Num() == 0) { return; }
	
	for (int i = 0; i < ManagedButtons.Num(); i++)
	{
		UWidgetStudioButtonBase* Button = ManagedButtons[i];

		if (!IsValid(Button))
		{
			break;
		}

		// Force uncheckable button to be checkable.
		if (!Button->IsCheckable())
		{
			Button->SetCheckable(true);
		}
		
		if (i != CurrentIndex)
		{
			if (Button->IsChecked() != false)
			{
				Button->SetChecked(false);
				Button->bIsInteractable = true;
			}
		}
		else
		{
			// Checked fail safe. This is in case that the button is suppose to be checked, but for some reason wasn't.
			if (Button->IsChecked() != true)
			{
				Button->SetChecked(true);
			}

			Button->bIsInteractable = false;
		}
	}
}

int32 UWidgetStudioButtonGroup::GetCurrentIndex() const
{
	return CurrentIndex;
}

TArray<UWidgetStudioButtonBase*> UWidgetStudioButtonGroup::GetButtons() const
{
	return ManagedButtons;
}

UWidgetStudioButtonBase* UWidgetStudioButtonGroup::GetButtonAtIndex(const int32 Index)
{
	if (ManagedButtons.IsValidIndex(Index))
	{
		return ManagedButtons[Index];
	}

	return nullptr;
}

UWidgetStudioButtonBase* UWidgetStudioButtonGroup::GetCurrentButton()
{
	return ManagedButtons[CurrentIndex];
}

int32 UWidgetStudioButtonGroup::GetButtonCount() const
{
	return ManagedButtons.Num();
}

EClusivity UWidgetStudioButtonGroup::GetSelectionMethod() const
{
	return SelectionMethod;
}

void UWidgetStudioButtonGroup::SetSelectionMethod(const EClusivity NewMethod)
{
	SelectionMethod = NewMethod;
	for (int i = 0; i < ManagedButtons.Num(); i++)
	{
		ManagedButtons[i]->SetCheckable(SelectionMethod == EClusivity::Exclusive);
	}

	SynchronizeProperties();
}

void UWidgetStudioButtonGroup::AddButton(UWidgetStudioButtonBase* NewButton)
{
	if (IsValid(NewButton))
	{
		// Add button to managed button group
		ManagedButtons.AddUnique(NewButton);

		// Bind button on pressed event to internal function to process event
		NewButton->OnPressed.AddDynamic(this, &UWidgetStudioButtonGroup::OnButtonPressed);
	
		// Broadcast that the buttons have been changed
		OnButtonsChanged.Broadcast();

		// Sets the selection method on the checkbox.
		if (Cast<UWidgetStudioModernCheckBox>(NewButton))
		{
			Cast<UWidgetStudioModernCheckBox>(NewButton)->SetSelectionMethod(SelectionMethod);
		}
	}
}

bool UWidgetStudioButtonGroup::RemoveButton(UWidgetStudioButtonBase* ButtonToRemove)
{
	if (IsValid(ButtonToRemove))
	{
		// Makes the button interactable.
		ButtonToRemove->bIsInteractable = true;
		
		// Unbind
		ButtonToRemove->OnPressed.RemoveAll(this);
		
		// Remove button from managed button group
		const int ButtonRemoved = ManagedButtons.Remove(ButtonToRemove);

		if (ButtonRemoved > 0)
		{
			// Broadcast that the buttons have been changed
			OnButtonsChanged.Broadcast();

			UpdateCurrentIndex();
		}
	
		return ButtonRemoved > 0;
	}
	return false;
}

void UWidgetStudioButtonGroup::RemoveButtonAtIndex(const int32 Index)
{
	if (Index > -1 && Index < ManagedButtons.Num())
	{
		ManagedButtons.RemoveAt(Index);
		OnButtonsChanged.Broadcast();
		UpdateCurrentIndex();
	}
}

void UWidgetStudioButtonGroup::RemoveAllButtons()
{
	// Empty the managed buttons
	ManagedButtons.Empty();

	// Reset current index to -1
	SetCurrentIndex(-1, false);
}

void UWidgetStudioButtonGroup::SetCurrentIndex(const int32 Index, const bool bBroadcast)
{
	// Force set the index to -1 if invalid index.
	if (!ManagedButtons.IsValidIndex(Index) || Index <= -1)
	{
		CurrentIndex = -1;
		CurrentButton = nullptr;
	}

	// If the incoming index is equal to or greater than the number of managed buttons, force set the current index to the last valid index.
	if (Index >= ManagedButtons.Num())
	{
		const int LastValidIndex = ManagedButtons.Num() -1;
		if (GetButtonAtIndex(LastValidIndex)->IsCheckedStateLocked())
		{
			UE_LOG(LogWidgetStudio, Warning, TEXT("%s: Checked state NOT changed due to buttons checked state being locked."), *FString(__func__));
			return;
		}
		
		CurrentIndex = LastValidIndex;
		CurrentButton = GetButtonAtIndex(LastValidIndex);
		if (bBroadcast)
		{
			OnCurrentIndexChanged.Broadcast(LastValidIndex);
		}
	}

	// If the incoming index is valid.
	if (ManagedButtons.IsValidIndex(Index))
	{
		if (GetButtonAtIndex(Index)->IsCheckedStateLocked())
		{
			UE_LOG(LogWidgetStudio, Warning, TEXT("%s: Checked state NOT changed due to buttons checked state being locked."), *FString(__func__));
			return;
		}
		
		CurrentIndex = Index;
		CurrentButton = GetButtonAtIndex(CurrentIndex);
		if (bBroadcast)
		{
			OnCurrentIndexChanged.Broadcast(CurrentIndex);
		}
	}

	SynchronizeProperties();
}
