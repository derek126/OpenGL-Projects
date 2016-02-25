#include "GameManager.h"
#include "LagrangianController.h"

int main()
{
	LagrangianController* Controller = new LagrangianController();
	if (GAMEMANAGER.Initialize(Controller))
	{
		GAMEMANAGER.Run();
	}
}