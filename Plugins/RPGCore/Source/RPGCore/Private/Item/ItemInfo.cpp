// Fill out your copyright notice in the Description page of Project Settings.


#include "RPGCore/Public/Item/ItemInfo.h"

UPaperFlipbook* FItemInfo::GetIdleAnimation(EDirection Direction)const
{
	switch (Direction)
	{
	case EDirection::ED_Up:
		return IdleAnimationUp;
		break;

	case EDirection::ED_Down:
		return IdleAnimationDown;
		break;

	case EDirection::ED_Right:
		return IdleAnimationRight;
		break;

	case EDirection::ED_Left:
		return IdleAnimationLeft;
		break;
	default:
		return IdleAnimationUp;
	}
}

UPaperFlipbook* FItemInfo::GetAttackAnimation(EDirection Direction)const
{
	switch (Direction)
	{
	case EDirection::ED_Up:
		return RunningAnimationUp;
		break;

	case EDirection::ED_Down:
		return RunningAnimationDown;
		break;

	case EDirection::ED_Right:
		return RunningAnimationRight;
		break;

	case EDirection::ED_Left:
		return RunningAnimationLeft;
		break;
	default:
		return RunningAnimationUp;
	}
}

UPaperFlipbook* FItemInfo::GetShieldDrawnAnimation(EDirection Direction)const
{
	switch (Direction)
	{
	case EDirection::ED_Up:
		return ShieldDrawnAnimationUp;
		break;

	case EDirection::ED_Down:
		return ShieldDrawnAnimationDown;
		break;

	case EDirection::ED_Right:
		return ShieldDrawnAnimationRight;
		break;

	case EDirection::ED_Left:
		return ShieldDrawnAnimationLeft;
		break;
	default:
		return ShieldDrawnAnimationUp;
	}
}

UPaperFlipbook* FItemInfo::GetDodgeRollAnimation(EDirection Direction)const
{
	switch (Direction)
	{
	case EDirection::ED_Up:
		return DodgeRollAnimationUp;
		break;

	case EDirection::ED_Down:
		return DodgeRollAnimationDown;
		break;

	case EDirection::ED_Right:
		return DodgeRollAnimationRight;
		break;

	case EDirection::ED_Left:
		return DodgeRollAnimationLeft;
		break;
	default:
		return DodgeRollAnimationUp;
	}
}

UPaperFlipbook* FItemInfo::GetRunningAnimation(EDirection Direction) const
{
	switch (Direction)
	{
	case EDirection::ED_Up:
		return RunningAnimationUp;
		break;

	case EDirection::ED_Down:
		return RunningAnimationDown;
		break;

	case EDirection::ED_Right:
		return RunningAnimationRight;
		break;

	case EDirection::ED_Left:
		return RunningAnimationLeft;
		break;
	default:
		return RunningAnimationUp;
	}
}

UPaperFlipbook* FItemInfo::GetSpecialAnimation(FString Name, EDirection Direction) const
{
	if (SpecialAnimations.Num() > 0)
	{
		for (int i = 0; i < SpecialAnimations.Num(); i++)
		{
			if(SpecialAnimations[i].SpecialAnimationName == Name)
			{
				switch (Direction)
				{
				case EDirection::ED_Up:
					return SpecialAnimations[i].DirectionUp;

				case EDirection::ED_Down:
					return SpecialAnimations[i].DirectionDown;

				case EDirection::ED_Right:
					return SpecialAnimations[i].DirectionRight;

				case EDirection::ED_Left:
					return SpecialAnimations[i].DirectionLeft;
				default:
					return nullptr;
				}
			}
		}
	}
	
	return nullptr;
}
