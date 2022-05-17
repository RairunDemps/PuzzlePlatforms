// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PPServerRowWidget.generated.h"

class UTextBlock;

UCLASS()
class PUZZLEPLATFORMS_API UPPServerRowWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	void SetServerName(const FText& ServerName);

protected:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* ServerNameTextBlock;
};
