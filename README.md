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

- C++17 and ROOT

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
- `-h, --help`                                 ... help command to display the usage

## example

If the input DRS4 file is `/path/to/test.dat` and you want to get the output `./test.root`

```
./bin/drs2root -i /path/to/test.dat -o ./test.root
```
