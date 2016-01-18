#include "GameManager.h"
#include "SpringController.h"

int main()
{
	SpringController* Controller = new SpringController();
	if (GAMEMANAGER.Initialize(Controller))
	{
		GAMEMANAGER.Run();
	}
}