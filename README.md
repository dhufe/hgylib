# HGYLib

## Overview
Code allows to read volumetric data (file extension .hgv) produced by the Hilgus software. In the meaning of ultrasonic investigations, volumetric means two independent spatial coordinates and a position synchronized time signal (e.g. an amplitude scan, A-Scan). The code is also able two read the following types of scans (file extension .hgy) : brightness, contour, depth and feature scan. If the input file includes of multiple types of scans, they will read seperatley with their own amplitude resolution.

The main usage of this code is to post-process recorded ultrasound data using Python.

## Usage
Some proof of concept code is attached in the *src* directory and is able to read the data files and convert them to MATLAB using the [matio library](https://sourceforge.net/projects/matio/).

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
