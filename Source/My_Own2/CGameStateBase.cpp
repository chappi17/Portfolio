#include "CGameStateBase.h"
#include "Global.h"

ACGameStateBase::ACGameStateBase()
{
	CurrentScore = 0;
}
void ACGameStateBase::SetScore(int32 NewScore)
{
	CurrentScore = NewScore;
}

int32 ACGameStateBase::getScore()
{
	return CurrentScore;
}
