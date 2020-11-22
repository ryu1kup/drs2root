# drs2root

A converter from a DRS4 binary dat to a ROOT tree file

## output data structure

The output root file contains a simple `TTree` object.
The `ch<i>_dt/wf` branches are only written when the channel `<i>` is switched on in the binary.

| branch       | unit | type                 | description                               |
| ------------ | ---- | -------------------- | ----------------------------------------- |
| range_center |      | unsigned short       | the center of the dynamic range           |
| trigger_cell |      | unsigned short       | the trigger cell used to calibrate timing |
| ch1_dt       | bin  | float[1024]          | effective bin width in ns for channel 1   |
| ch2_dt       | bin  | float[1024]          | effective bin width in ns for channel 2   |
| ch3_dt       | bin  | float[1024]          | effective bin width in ns for channel 3   |
| ch4_dt       | bin  | float[1024]          | effective bin width in ns for channel 4   |
| ch1_wf       | ADC  | unsigned short[1024] | waveform in ADC count for channel 1       |
| ch2_wf       | ADC  | unsigned short[1024] | waveform in ADC count for channel 2       |
| ch3_wf       | ADC  | unsigned short[1024] | waveform in ADC count for channel 3       |
| ch4_wf       | ADC  | unsigned short[1024] | waveform in ADC count for channel 4       |

NOTE: dt and wf should be calibrated, bin to ns and ADC to V, respectively. See the [manual](https://www.psi.ch/sites/default/files/import/drs/DocumentationEN/manual_rev50.pdf).

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
    -i, --input <input.dat>    specify the input DRS4 binary file <input.dat>
    -o, --output <output.dat>  specify the output root file <output.root> (=./output.root)
    -h, --help`                help command to display the usage
```
