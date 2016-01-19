#include "GameManager.h"
#include "FluidController.h"

int main()
{
	FluidController* Controller = new FluidController();
	if (GAMEMANAGER.Initialize(Controller))
	{
		GAMEMANAGER.Run();
	}
}