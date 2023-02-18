# Display the output

```bash
sudo apt install minicom
```

then run:

```bash
minicom -b 115200 -o -D /dev/ttyACM0
```

or use the script:

```bash
./displayOutput
```

exit `minicom` with `Ctrl-A` then `X`

## Compile and upload

This command will compile and upload the code to the pico,
it will ask you to unplug it and re-plug it while holding the `BOOTSEL` button.

```bash
./run
```
