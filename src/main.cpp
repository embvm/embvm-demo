// Copyright 2020 Embedded Artistry LLC
// SPDX-License-Identifier: GPLv3-only

#include <cstdio>
#include <logging/log.hpp>
#include <platform.hpp>

volatile bool abort_program_ = false;

constexpr auto DEFAULT_TOF_READ_DELAY = std::chrono::milliseconds(500);

int main()
{
	int r = 0;

	logdebug("Entered main()\n");

	auto& platform = VirtualPlatform::inst();

	platform.startBlink();

	// Run an i2c sweep for demo purposes
	auto i2c = platform.findDriver<embvm::i2c::master>();
	if(i2c)
	{
		logdebug("Sweeping\n");
		embvm::i2c::master::sweep_list_t found_list;
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

	auto tof = platform.findDriver<embvm::tof::sensor>();
	assert(tof);

	// TODO: put this on a timer, instead of a thread? Make a platform delay function?
	while(!abort_program_)
	{
		tof.value().read();
#ifdef ENABLE_STM32L4_WORKAROUND
		for(int i = 0; i < 100000; i++)
			;
#else
		std::this_thread::sleep_for(DEFAULT_TOF_READ_DELAY);
#endif
	}

	printf("Exiting demo application. Log contents:\n");

	platform.echoLogBufferToConsole();

	return r;
}
