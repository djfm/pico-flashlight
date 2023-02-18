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