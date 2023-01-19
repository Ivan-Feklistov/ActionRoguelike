// Fill out your copyright notice in the Description page of Project Settings.


#include "SWorldUserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Components/SizeBox.h"

void USWorldUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	FVector2D ScreenPosition;
	if (UGameplayStatics::ProjectWorldToScreen(GetOwningPlayer(), AttachedActor->GetActorLocation() + WorldOffset, ScreenPosition))
	{
		if (!IsValid(AttachedActor))
		{
			RemoveFromParent();
			return;
		}
		float Scale = UWidgetLayoutLibrary::GetViewportScale(this);

		ScreenPosition /= Scale;

		if (!ParentSizeBox)
		{
			GEngine->AddOnScreenDebugMessage(FMath::Rand(), 5.0f, FColor::Red, FString::Printf(TEXT("You must have Size Box named 'ParentSizeBox' and have everythin else as child of it in order to properly attach widget to object in World!")));
		}
		else
		{
			ParentSizeBox->SetRenderTranslation(ScreenPosition);
		}
	}
}
