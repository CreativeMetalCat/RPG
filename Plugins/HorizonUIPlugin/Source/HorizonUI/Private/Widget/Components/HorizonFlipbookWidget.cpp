// Created by dorgon, All Rights Reserved.
// Date of intended publication: 2019/08/01
// email: dorgonman@hotmail.com
// blog: dorgon.horizon-studio.net


#include "Widget/Components/HorizonFlipbookWidget.h"
#include "Widget/HorizonWidgetFunctionLibrary.h"

#include "PaperFlipbook.h"
#include "PaperFlipbookComponent.h"
#include "PaperSprite.h"


//UMG
#include "UMGStyle.h"
#include "Components/CanvasPanelSlot.h"


#if 1 // override

void UHorizonFlipbookWidget::OnWidgetRebuilt()
{
	Super::OnWidgetRebuilt();
	ResetAnimation();

}


void UHorizonFlipbookWidget::Tick(float DeltaTime) 
{
	if (PaperFlipbook) 
	{
		if (NumOfLoop <= 0 || CurrentNumOfLoop < NumOfLoop) 
		{   //if NumOfLoop <= 0, then it will loop forever
			CurrentDuration += DeltaTime;

			SetCurrentImage(CurrentDuration);
			if (CurrentDuration > PaperFlipbook->GetTotalDuration()) 
			{
				CurrentDuration = 0;
				//prevent overflow crash
				if (CurrentNumOfLoop < TNumericLimits<int32>::Max()) 
				{
					CurrentNumOfLoop += 1;
				}

			}
		}

	}


}
#endif // override

#if 1 // Functions


void UHorizonFlipbookWidget::SetFlipbook(UPaperFlipbook* InFlipbook)
{
	PaperFlipbook = InFlipbook;
	ResetAnimation();
}


void UHorizonFlipbookWidget::ResetAnimation() 
{
	CurrentDuration = 0;
	CurrentNumOfLoop = 0;
	CurrentSpriteIndex = -1;
	SetCurrentImage(CurrentDuration);
}


void UHorizonFlipbookWidget::PlayAnimation()
{
	ResetAnimation();
	bIsStartTick = true;
}

void UHorizonFlipbookWidget::StopAnimation()
{
	ResetAnimation();
	bIsStartTick = false;
}


void UHorizonFlipbookWidget::PauseAnimation()
{
	bIsStartTick = false;
}

void UHorizonFlipbookWidget::ResumeAnimation()
{
	bIsStartTick = true;
}


float UHorizonFlipbookWidget::GetCurrentAnimationDuration() 
{
	return CurrentDuration;
}
void UHorizonFlipbookWidget::SetCurrentAnimationDuration(float InDuration)
{
	CurrentDuration = InDuration;
}


void UHorizonFlipbookWidget::SetCurrentImage(float currentDuration) {
	if (PaperFlipbook) 
	{
		int32 spriterIndex = PaperFlipbook->GetKeyFrameIndexAtTime(currentDuration);
		if (CurrentSpriteIndex != spriterIndex)
		{
			CurrentSpriteIndex = spriterIndex;
			UPaperSprite* pSprite = PaperFlipbook->GetSpriteAtTime(currentDuration);
			if (pSprite) 
			{
				FSlateAtlasData&& atlasData = pSprite->GetSlateAtlasData();
				SetBrushFromTexture(Cast<UTexture2D>(atlasData.AtlasTexture));
				FBox2D UVCoordinates(atlasData.StartUV, atlasData.StartUV + atlasData.SizeUV);
				UVCoordinates.bIsValid = true;
				Brush.SetUVRegion(UVCoordinates);
				
			}
		}
		
	}
	
}

#endif // Functions












