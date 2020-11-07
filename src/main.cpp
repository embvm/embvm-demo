// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPLv3-only

#include <cstdio>
// This header provides access to the logging subsystem and macros (logdebug() below)
#include <logging/log.hpp>
// Each platform defines a standard platform.hpp header, which contains
// the implementation for the platform class. The platform header maps the specific
// platform class to the `VirtualPlatform` type (with a `using` statement). This
// ensures that our application has a common way of accessing the platform object and interface,
// rather than having to redefine the type each time we want to build the application.
// This can be viewed as an abstract interface of sorts!
#include <platform.hpp>

// TODO: put while loop on a timer, instead of a thread?
// Or, alternatively, make a platform delay function?

volatile bool abort_program_ = false;
constexpr auto DEFAULT_TOF_READ_DELAY = std::chrono::milliseconds(500);

int main()
{
	int r = 0;

	logdebug("Entered main()\n");

	// This is how we access our platform class in a portable way -
	// We use a common `VirtualPlatform` alias.
	auto& platform = VirtualPlatform::inst();

	// Start Blink is an API that is required by this application.
	// It is implemented at the platform level. What the platform does
	// is implementation-dependent. Some will use threads, some will use
	// peripheral timers, some will forward this to the hardware platform.
	platform.startBlink();

	// Access the first available I2C master via its abstract interface.
	// Returns an optional<> type.
	auto i2c = platform.findDriver<embvm::i2c::master>();

	// If an I2C master device has been registered, we will run
	// an I2C sweep to see what devices are connected.
	// If not, we will write to the log and continue on.
	if(i2c)
	{
		logdebug("Performing I2C Sweep\n");
		embvm::i2c::master::sweep_list_t found_list;
		// Note that we register a lambda function as a callback for sweep()
		i2c.value().sweep(found_list, [&]() {
			logdebug("Sweep found %d devices\n", found_list.size());

			for(const auto& t : found_list)
			{
				logdebug("Found address: 0x%x\n", t);
			}
		});
	}
	else
	{
		logdebug("I2C bus not found on this system\n");
	}

	// Get the abstract interface for the ToF sensor; returns an optional<> type.
	auto tof = platform.findDriver<embvm::tof::sensor>();
	// Abort the program if the device is not found
	assert(tof);

	while(!abort_program_)
	{
		// Trigger a sensor read
		// Note that no action is taken here - specific responses to this
		// read are handled via callbacks
		tof.value().read();

		// Workaround is in place because our STM32L4 module
		// does not support FreeRTOS yet.
#ifdef ENABLE_STM32L4_WORKAROUND
		for(int i = 0; i < 100000; i++)
			;
#else
		std::this_thread::sleep_for(DEFAULT_TOF_READ_DELAY);
#endif
	}

	printf("Exiting demo application. Log contents:\n");

	// If a _putchar() function is defined, this will dump the contents of the buffer to the
	// associated communication channel.
	// Note that this is a platform API, not a standard logging API.
	platform.echoLogBufferToConsole();

	return r;
}
