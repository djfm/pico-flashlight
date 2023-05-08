# This project is a flashlight powered by a Raspberry Pi Pico

You should have the [PICO SDK](https://github.com/raspberrypi/pico-sdk),
and set its path in the `CMakeLists.txt` file.

I'll make a schematic later.

I'm making this for educational purposes.

The documentation is not meant for others yet.

## General setup

On debian add yourself to the `dialout` group:

```bash
sudo usermod -a -G dialout $USER
```

Then log out and log back in or use `newgrp dialout` to apply the changes in the current shell.

On ubuntu you should be able to communicate with `/dev/ttyACM0` by default.

## VSCode intellisense

Clang doesn't seem to work great or at least I dunno how to make it work.
Use the native Microsoft intellisense C/C++ extensions,
ensure that C++ intellisense is enabled in settings.

## Development commands

### Display the debug output

Install `minicom`:

```bash
sudo apt install minicom
```

Then run:

```bash
minicom -b 115200 -o -D /dev/ttyACM0
```

Or use the convenience script:

```bash
./displayOutput
```

You can exit `minicom` with `Ctrl-A` then `X`.

### Compile and upload

This command will compile and upload the code to the pico,
it will ask you to unplug it and re-plug it while holding the `BOOTSEL` button.

It will then display the debug output.

```bash
./run
```

## Some useful derivations

### Setting up PWM

To set up PWM, we need to set the `pwm_wrap` and `divider` of our PWM PIN.
The SDK understands slice numbers and not pins, but there are functions to convert from `GPIO` to `slice` number.

The frequency of the PWM is governed by the equation


$$divider = \frac{frequency_{clock}}{pwm_{wrap} * frequency_{desired}}$$

where \$frequency_{clock}\$ is the frequency of the pico, which is generally 125MHz.

And we have the constraints that:

$$pwm_{wrap} \in [1, 65535]$$

$$divider \in [1, 256] \cap N$$

To get the most precision possible in setting our duty cycle, we want to set \$pwm_{wrap}\$ to the maximum value possible.

The current code could be optimized, it strikes a good balance for a PWM at 50kHz.
