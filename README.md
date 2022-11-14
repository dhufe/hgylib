# HGYLib

[![DOI](https://zenodo.org/badge/184125155.svg)](https://zenodo.org/badge/latestdoi/184125155)

## Overview
Code allows to read volumetric data (file extension .hgv) produced by the Hilgus software. In the meaning of ultrasonic investigations, volumetric means two independent spatial coordinates and a position synchronized time signal (e.g. an amplitude scan, A-Scan). The code is also able two read the following types of scans (file extension .hgy) : brightness, contour, depth and feature scan. If the input file includes of multiple types of scans, they will read seperatley with their own amplitude resolution.

The main usage of this code is to post-process recorded ultrasound data using Python.

## Compiling

Get a fresh copy using git and cloning the repository.

``` bash
git clone https://github.com/dkotscha/hgylib.git
```

The build process uses CMake to produce a binary. The setup is tested under macOS, Linux (amd64 and x86) and Microsoft Windows 7.

``` bash
mkdir build
cd build
cmake ../ -DCMAKE_BUILD_TYPE=Release
make
```

## Usage

Some proof of concept code is attached in the *src* directory and is able to read the data files and convert them to MATLAB using the [matio library](https://sourceforge.net/projects/matio/).

| Parameter      | Description                                                                       |
| :------------- | --------------------------------------------------------------------------------: |
| -i, --input    | Source/Input HILGUS files                                                         |
| -o, --output   | Output MATLAB file                                                                |
| --verbose      | Some additional output                                                            |
| --export       | Creates a Markdown formated table with a minor set of reconstruction parameters   |


## Future ideas

### Python extensions

Playing around with extensions for Python. This will enable Python scripts to access the data directly.

### Handling of large files

Handling large files requires various changes and development steps

- [x] Fragmented read support for large input files
- [x] Appending chunks of data to existing MATLAB variables by using libmatio (tested for 1D vectors and 2D matrices)
- [x] Test sequencial read of large files with MATLAB (or at least h5py)
- [ ] Rewrite the code in a parallel woring producer/consumer scheme using threads
- [x] Compiling against recent versions of hdf5 and libmatio

### Reconstruction parameters

Currently, a minor set of reconstruction parameters is support during the conversion process. May be, there is a way to get out all parameters.
