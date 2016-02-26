#include "GameManager.h"
#include "FluidController.h"
#include "IBoidsController.h"

int main()
{
	IBoidsController* Controller = new IBoidsController();
	if (GAMEMANAGER.Initialize(Controller))
	{
		GAMEMANAGER.Run();
	}

	/*FluidController* Controller = new FluidController();
	if (GAMEMANAGER.Initialize(Controller))
	{
		GAMEMANAGER.Run();
	}*/
}