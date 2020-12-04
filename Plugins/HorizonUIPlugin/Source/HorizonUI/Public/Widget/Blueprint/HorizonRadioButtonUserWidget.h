// Created by dorgon, All Rights Reserved.
// Date of intended publication: 2019/08/01
// email: dorgonman@hotmail.com
// blog: dorgon.horizon-studio.net

#pragma once

#include "CoreMinimal.h"
#include "Widget/Blueprint/HorizonDesignableUserWidget.h"
#include "HorizonRadioButtonUserWidget.generated.h"

class UCheckBox;
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class HORIZONUI_API UHorizonRadioButtonUserWidget : public UHorizonDesignableUserWidget
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHorizonRadioButtonEvent);
	DECLARE_MULTICAST_DELEGATE(FOnHorizonRadioButtonEventNative);
public:
	UHorizonRadioButtonUserWidget();
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	virtual void OnSynchronizeProperties_Implementation() override;

public:
	UFUNCTION(BlueprintCallable, Category = "HorizonPlugin|UI|RadioButton")
	virtual void SetChecked();
	virtual bool IsChecked();
private:
	UFUNCTION()
	virtual void NativeOnCheckStateChanged(bool bIsChecked);

	void TryChecked();
public:
	UPROPERTY(BlueprintAssignable, Category = "HorizonPlugin|UI|Delegate")
	FOnHorizonRadioButtonEvent OnCheckedDelegate;

	UPROPERTY(BlueprintAssignable, Category = "HorizonPlugin|UI|Delegate")
	FOnHorizonRadioButtonEvent OnUnCheckedDelegate;

	// for c++ callback binding
	FOnHorizonRadioButtonEventNative OnCheckedDelegateNative;
	FOnHorizonRadioButtonEventNative OnUnCheckedDelegateNative;
public:
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget), Category = "UI")
	UCheckBox* CheckBox_Main = nullptr;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidgetOptional), Category = "UI")
	UTextBlock* TextBlock_Main = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Content", meta = (MultiLine = true))
	FText Text_Main;

};
