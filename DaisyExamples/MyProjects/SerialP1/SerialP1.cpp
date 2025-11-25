#include "daisy_seed.h"

using namespace daisy;

DaisySeed hw;

int main(void)
{
	hw.Init();
	hw.StartLog(true);
	System::Delay(5000);
	hw.PrintLine("Hello World");
	while(1) {}
}
