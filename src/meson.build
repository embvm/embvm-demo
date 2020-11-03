# Experimental App Meson Build File

fwdemo_app = executable('fwdemo',
	sources: ['main.cpp'],
	dependencies: [
		fwdemo_sim_platform_dep
	],
	native: true
)

# We want to build a demo app to test cross compilation on x86_64 with mainline clang

if host_machine.cpu_family() == 'x86_64'
	fwdemo_app_x86 = executable('fwdemo_x86',
		sources: ['main.cpp'],
		dependencies: [
			fwdemo_sim_x86_platform_dep
		],
	)
endif

if host_machine.cpu_family() == 'arm'

	target_objcopy = find_program(meson.get_cross_property('objcopy'))

	#######################
	# NRF52840 DK Targets #
	#######################

	fwdemo_nrf52dk_app = executable('fwdemo_nrf52dk',
		sources: [
			'main.cpp',
		],
		# TODO: these need to be in the platform really...
		# Also need to structure the build so that's a choice that can be made, since it's in the startup files.
		# Maybe these need to be linker-script values rather than macros?
		# Is there another dependency in the startup code that impacts this?
		c_args: [
			'-D__HEAP_SIZE=8192',
			'-D__STACK_SIZE=8192',
		],
		cpp_args: [
			'-D__HEAP_SIZE=8192',
			'-D__STACK_SIZE=8192',
		],
		link_args: [
			'-Wl,-Map=nrf52_dk_fwdemo.map',
		],
		dependencies: [
			fwdemo_nrf52dk_platform_dep
		],
	)

	fwdemo_blinky_hex = custom_target('fwdemo_nrf52dk.hex',
		input: fwdemo_nrf52dk_app,
		output: 'fwdemo_nrf52dk.hex',
		command: [target_objcopy, '-O', 'ihex', '@INPUT@', '@OUTPUT@'],
		build_by_default: true
	)

	fwdemo_blinky_bin = custom_target('fwdemo_nrf52dk.bin',
		input: fwdemo_nrf52dk_app,
		output: 'fwdemo_nrf52dk.bin',
		command: [target_objcopy, '-O', 'binary', '@INPUT@', '@OUTPUT@'],
		build_by_default: true
	)

	#########################
	# Nucleo-L4R5ZI Targets #
	#########################

	fwdemo_nucleo_l4r5_app = executable('fwdemo_nucleo_l4r5',
		sources: [
			'main.cpp',
		],
		# TODO: these need to be in the platform really...
		# Also need to structure the build so that's a choice that can be made, since it's in the startup files.
		# Maybe these need to be linker-script values rather than macros?
		# Is there another dependency in the startup code that impacts this?
		c_args: [
			'-D__HEAP_SIZE=8192',
			'-D__STACK_SIZE=8192',
		],
		cpp_args: [
			'-D__HEAP_SIZE=8192',
			'-D__STACK_SIZE=8192',
		],
		link_args: [
			'-Wl,-Map=fwdemo_nucleo_l4r5.map',
		],
		dependencies: [
			nucleo_l4r5zi_fwdemo_platform_dep
		],
	)

	fwdemo_nucleo_l4r5_hex = custom_target('fwdemo_nucleo_l4r5.hex',
		input: fwdemo_nucleo_l4r5_app,
		output: 'fwdemo_nucleo_l4r5.hex',
		command: [target_objcopy, '-O', 'ihex', '@INPUT@', '@OUTPUT@'],
		build_by_default: true
	)

	fwdemo_nucleo_l4r5_bin = custom_target('fwdemo_nucleo_l4r5.bin',
		input: fwdemo_nucleo_l4r5_app,
		output: 'fwdemo_nucleo_l4r5.bin',
		command: [target_objcopy, '-O', 'binary', '@INPUT@', '@OUTPUT@'],
		build_by_default: true
	)
endif

# Next we will declare a non-native app for an embedded target, which is built
# using the fwdemo_app_dep and the appropriate fwdemo platform

clangtidy_files += files('main.cpp')
