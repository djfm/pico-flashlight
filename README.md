# This project is a flashlight powered by a Raspberry Pi Pico

You should have the [PICO SDK](https://github.com/raspberrypi/pico-sdk),
and set its path in the `CMakeLists.txt` file.

I'll make a schematic later.

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


$$divider = \frac{clock_frequency}{pwm_wrap * desired_frequency}$$

where `clock_frequency` is the frequency of the clock used by the PWM, which is 125MHz.

And we have the constraints that:

$$
pwm_wrap \in \intv{1, 65535}
divider \in \Iintv{1, 65535}
$$

