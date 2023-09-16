/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/

#include "WSEditorFunctionLibrary.h"
#include "EditorUtilityWidget.h"
#include "UnrealEd.h"
#include "LevelEditor.h"

/**
* Spawns an Editor Tab if the given Widget Blueprint is an Editor Utility Widget
* @param FSpawnTabArgs& - const
* @param Blueprint - UWidgetBlueprint to spawn tab for
* @return SDockTab shared reference
*/
TSharedRef<SDockTab> SpawnEditorUITab(const FSpawnTabArgs&, UWidgetBlueprint* Blueprint) {
	TSharedRef<SDockTab> SpawnedTab = SNew(SDockTab);
	UWorld* World = GEditor->GetEditorWorldContext().World(); 
	check(World);
	UEditorUtilityWidget* CreatedUMGWidget = CreateWidget<UEditorUtilityWidget>(World, TSubclassOf<UEditorUtilityWidget>(Blueprint->GeneratedClass));
	if (CreatedUMGWidget)
	{
		SpawnedTab->SetContent(TSharedRef<SWidget>(CreatedUMGWidget->TakeWidget()));
	}

	return SpawnedTab;
}

void UWidgetStudioEditorFunctionLibrary::OpenEditorUtilityWidget(UWidgetBlueprint* WidgetBlueprint)
{
	if (IsValid(WidgetBlueprint))
	{
		if (WidgetBlueprint->GeneratedClass->IsChildOf(UEditorUtilityWidget::StaticClass()))
		{
			const UEditorUtilityWidget* CDO = WidgetBlueprint->GeneratedClass->GetDefaultObject<UEditorUtilityWidget>();
			if (CDO->ShouldAutoRunDefaultAction())
			{
				// This is an instant-run blueprint, just execute it
				UEditorUtilityWidget* Instance = NewObject<UEditorUtilityWidget>(GetTransientPackage(), WidgetBlueprint->GeneratedClass);
				Instance->ExecuteDefaultAction();
			}
			else
			{
				const FName RegistrationName = FName(*(WidgetBlueprint->GetPathName() + TEXT("_ActiveTab")));
				const FText DisplayName = FText::FromString(WidgetBlueprint->GetName());
				FLevelEditorModule& LevelEditorModule = FModuleManager::GetModuleChecked<FLevelEditorModule>(TEXT("LevelEditor"));
				TSharedPtr<FTabManager> LevelEditorTabManager = LevelEditorModule.GetLevelEditorTabManager();
				if (!LevelEditorTabManager->HasTabSpawner(RegistrationName))
				{
					LevelEditorTabManager->RegisterTabSpawner(RegistrationName, FOnSpawnTab::CreateStatic(&SpawnEditorUITab, WidgetBlueprint))
                        		.SetDisplayName(DisplayName)
                        		.SetMenuType(ETabSpawnerMenuType::Hidden);
				}

				LevelEditorTabManager->TryInvokeTab(RegistrationName);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to Open Widget due to the given object <%s> being invalid."), *GetNameSafe(WidgetBlueprint));
	}
	// Original code derived from https://forums.unrealengine.com/t/how-to-open-an-editor-widget-utility-tab-from-code-python-or-c/126831/2
}
