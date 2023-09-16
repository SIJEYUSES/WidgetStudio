/* 
* Copyright (c) 2021 THEIA INTERACTIVE.  All rights reserved.
*
* Website: https://widgetstudio.design
* Documentation: https://docs.widgetstudio.design
* Support: marketplace@theia.io
* Marketplace FAQ: https://marketplacehelp.epicgames.com
*/

#pragma once

#include "Containers/UnrealString.h"
#include "Misc/Paths.h"
// ��ȡ��Ŀ�ĸ�Ŀ¼
FString ProjectDir = FPaths::ProjectDir();

// ����Ŀ¼�������ļ������·�������һ�𣬵õ������ļ�������·��
FString DefaultGame = FPaths::Combine(ProjectDir, TEXT("Config/DefaultGame.ini"));

// ��׼�������ļ���·��
FString DefaultGameIni = FConfigCacheIni::NormalizeConfigIniPath(DefaultGame);


FString WSDefaultGameSection = TEXT("/Script/WidgetStudioEditor.WidgetStudioSettings");

const FString PackagingSettingsSection = TEXT("/Script/UnrealEd.ProjectPackagingSettings");
const FString AddCookDirs = TEXT("+DirectoriesToAlwaysCook");
const FString SubtractCookDirs = TEXT("-DirectoriesToAlwaysCook");
const FString WSPathIni = TEXT("(Path=\"/WidgetStudio\")");
FString WSShortPath = TEXT("/WidgetStudio");

FString StringTheme = TEXT("Theme");
FString StringIconSet = TEXT("IconSet");
FString StringTypography = TEXT("Typography");
FString StringBorderRadius = TEXT("BorderRadius");
FString StringIdealWidth = TEXT("IdealWidth");
FString StringIdealHeight = TEXT("IdealHeight");
FString StringControlDimensions = TEXT("ControlDimensions");

FString MintThemePath = TEXT("/WidgetStudio/Style/Themes/Theme_Mint.Theme_Mint");
FString DefaultThemePath = MintThemePath;
FString NordDarkThemePath = TEXT("/WidgetStudio/Style/Themes/Theme_Nord_Dark.Theme_Nord_Dark");
FString NordLightThemePath = TEXT("/WidgetStudio/Style/Themes/Theme_Nord_Light.Theme_Nord_Light");
FString DefaultIconSetPath = TEXT("/WidgetStudio/Style/IconSets/IconSet_Default.IconSet_Default");
FString TestIconSetPath = TEXT("/WidgetStudio/Style/IconSets/TestIconSet/IconSet_Test.IconSet_Test");
FString OpenSansTypographyPath = TEXT("/WidgetStudio/Style/Typography/Typography_OpenSans.Typography_OpenSans");
FString TestTypographyPath = TEXT("/WidgetStudio/Style/Typography/TestTypography/Typography_Test.Typography_Test");
FString DefaultTypographyPath = OpenSansTypographyPath;

int32 DefaultBorderRadius = 15;
float DefaultIdealWidth = 75.f;
float DefaultIdealHeight = 35.f;