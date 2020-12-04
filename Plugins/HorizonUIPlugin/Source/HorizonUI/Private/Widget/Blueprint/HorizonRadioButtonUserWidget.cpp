// Created by dorgon, All Rights Reserved.
// Date of intended publication: 2019/08/01
// email: dorgonman@hotmail.com
// blog: dorgon.horizon-studio.net

#include "Widget/Blueprint/HorizonRadioButtonUserWidget.h"
#include "HorizonUIPrivate.h"
#include "Widget/Components/HorizonRadioButton.h"

UHorizonRadioButtonUserWidget::UHorizonRadioButtonUserWidget()
{
	Text_Main = FText::FromString(TEXT("Text Block"));
}

void UHorizonRadioButtonUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (CheckBox_Main)
	{
		CheckBox_Main->OnCheckStateChanged.AddDynamic(this, &ThisClass::NativeOnCheckStateChanged);
		TryChecked();
	
	}	

}


void UHorizonRadioButtonUserWidget::NativeDestruct()
{
	Super::NativeDestruct();

}

void UHorizonRadioButtonUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UHorizonRadioButtonUserWidget::OnSynchronizeProperties_Implementation()
{
	if (TextBlock_Main)
	{
		TextBlock_Main->SetText(FText::AsCultureInvariant(Text_Main));
	}
}



void UHorizonRadioButtonUserWidget::SetChecked()
{
	if (CheckBox_Main)
	{
		bool _bIsChecked = CheckBox_Main->IsChecked();
		NativeOnCheckStateChanged(true);
		if (!_bIsChecked)
		{
			OnCheckedDelegate.Broadcast();
			OnCheckedDelegateNative.Broadcast();
		}
	}
}

bool UHorizonRadioButtonUserWidget::IsChecked()
{
	if (CheckBox_Main)
	{
		return CheckBox_Main->IsChecked();
	}
	return false;
}

void UHorizonRadioButtonUserWidget::NativeOnCheckStateChanged(bool bIsChecked)
{

	auto pParentPanel = GetParent();
	if (pParentPanel)
	{
		TArray<UWidget*> allChridren = pParentPanel->GetAllChildren();
		for (auto& it : allChridren)
		{
			UHorizonRadioButtonUserWidget* pRadioButtonWidget = Cast<UHorizonRadioButtonUserWidget>(it);
			if (nullptr == pRadioButtonWidget || nullptr == pRadioButtonWidget->CheckBox_Main) { continue; }
			if (pRadioButtonWidget != this)
			{
				// uncheck others if the widget is checked
				if (pRadioButtonWidget->CheckBox_Main->GetCheckedState() == ECheckBoxState::Checked)
				{
					pRadioButtonWidget->CheckBox_Main->SetIsChecked(false);
					pRadioButtonWidget->OnUnCheckedDelegate.Broadcast();
					pRadioButtonWidget->OnUnCheckedDelegateNative.Broadcast();

				}
			}
			else
			{
				if (CheckBox_Main->GetCheckedState() != ECheckBoxState::Checked)
				{
					// click same widget, force checked
					CheckBox_Main->SetIsChecked(true);
				}
				else
				{
					OnCheckedDelegate.Broadcast();
					OnCheckedDelegateNative.Broadcast();
				}
			}
		}
	}

}


void UHorizonRadioButtonUserWidget::TryChecked()
{
	bool bHasChecked = false;
	auto pParentPanel = GetParent();
	if (pParentPanel)
	{
		TArray<UWidget*> allChridren = pParentPanel->GetAllChildren();
		for (auto& it : allChridren)
		{
			UHorizonRadioButtonUserWidget* pRadioButtonWidget = Cast<UHorizonRadioButtonUserWidget>(it);
			if (pRadioButtonWidget && pRadioButtonWidget->CheckBox_Main)
			{
				if (pRadioButtonWidget->CheckBox_Main->IsChecked())
				{
					bHasChecked = true;
					break;
				}
			}
		}
	}
	if (false == bHasChecked)
	{
		NativeOnCheckStateChanged(true);
	}
}