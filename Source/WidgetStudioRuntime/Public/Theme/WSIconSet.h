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
#include "Engine/DataAsset.h"

/* Do Not Remove */
#include "Engine/Texture2D.h"

#include "WSIconSet.generated.h"

/**
 * Data asset that contains the current theme icon set
 */
UCLASS(BlueprintType)
class WIDGETSTUDIORUNTIME_API UWidgetStudioIconSet : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ChevronLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ChevronRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ChevronUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ChevronDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Check;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ArrowLeft;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ArrowRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ArrowUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ArrowDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Close;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Plus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Minus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Hamburger;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* GridView;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Heart;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Envelope;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Trash;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Photo;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Document;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* DocumentCopy;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* LockClose;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* LockOpen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ArrowInUp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ArrowInDown;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Download;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Upload;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* CloudDownload;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* CloudUpload;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Cloud;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Home;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Pen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Power;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Phone;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* User;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Print;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Video;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Clipboard;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Magnifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* MagnifierPlus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* MagnifierMinus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Mobile;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Desktop;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Attachment;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Tag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Globe;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Link;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Question;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Exclamation;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Info;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Share;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Settings;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Cogwheel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Volume;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* VolumePlus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* VolumeMinus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* VolumeMute;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ControlPlay;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ControlStop;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ControlPause;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ControlRewind;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ControlFastForward;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ControlStart;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ControlEnd;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ControlFullScreen;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* SpeechBubble;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Bookmark;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Wifi;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Microphone;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Calendar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Users;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Clock;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Refresh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Pin;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Map;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Navigate;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Compass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Expand;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ArrowLeftRight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ShoppingCart;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Folder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* File;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* FilePlus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* FileMinus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* StatisticPie;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* StatisticGraph;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* ListView;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Sun;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IconSet")
		UTexture2D* Moon;
};
