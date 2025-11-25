#include "daisy_seed.h"

// Use the daisy namespace to prevent having to type
// daisy:: before all libdaisy functions
using namespace daisy;
using namespace daisy::seed;

// Declare a DaisySeed object called hardware
DaisySeed hardware;

int main(void)
{
    // Declare a variable to store the state we want to set for the LED.
    hardware.Init();
	hardware.Configure();

	GPIO my_button;

	my_button.Init(D0, GPIO::Mode::INPUT, GPIO::Pull::PULLUP);

    // Loop forever
    for(;;)
    {
		bool led_state = my_button.Read();
        // Set the onboard LED
        hardware.SetLed(!led_state);
    }
}
