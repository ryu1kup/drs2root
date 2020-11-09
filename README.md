# drs2root

A converter from a DRS4 binary dat to a ROOT tree file

## output data structure

The output root file contains a simple `TTree` object.

| branch       | type                 |
| ------------ | -------------------- |
| range_center | unsigned short       |
| trigger_cell | unsigned short       |
| ch1_wf       | unsigned short[1024] |
| ch2_wf       | unsigned short[1024] |
| ch3_wf       | unsigned short[1024] |
| ch4_wf       | unsigned short[1024] |

## requirements

- C++17 + ROOT

## compile

```
mkdir build
cd build
cmake ..
cmake --build . --target install
```

## usage

```
./bin/drs2root [options]
```

options:

- `-i, --input <input.dat>`                    ... specify the input DRS4 binar file `<input.dat>`
- `-o, --output <output.dat>`                  ... specify the output root file `<output.root>`
- `-c, --channel-activation <ch1 ch2 ch3 ch4>` ... activate/deactivate each channel simultaneously
- `-h, --help`                                 ... help command to display the usage

## example

If the input DRS4 file is `/path/to/test.dat` and you want to get the output `./test.root` with channel 1 and channel 4, then

```
./bin/drs2root -i /path/to/test.dat -o ./test.root -c 1 0 0 1
```

NOTE: be careful to activate/deactivate correct channels. If you set incorrect channels, then the output root file must be bugged (or simply `drs2root` would be aborted).
