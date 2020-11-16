# drs2root

A converter from a DRS4 binary dat to a ROOT tree file

## output data structure

The output root file contains a simple `TTree` object.
The `ch<i>_time/wf` branches are only written when the channel `<i>` is switched on in the binary.

| branch       | unit | type                 |
| ------------ | ---- | -------------------- |
| range_center |      | unsigned short       |
| trigger_cell |      | unsigned short       |
| ch1_time     | ns   | float[1024]          |
| ch2_time     | ns   | float[1024]          |
| ch3_time     | ns   | float[1024]          |
| ch4_time     | ns   | float[1024]          |
| ch1_wf       | V    | unsigned short[1024] |
| ch2_wf       | V    | unsigned short[1024] |
| ch3_wf       | V    | unsigned short[1024] |
| ch4_wf       | V    | unsigned short[1024] |

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

options:
    -i, --input <input.dat>    specify the input DRS4 binar file <input.dat>
    -o, --output <output.dat>  specify the output root file <output.root> (=./output.root)
    -h, --help`                help command to display the usage
```
