# Overview
Code allows to read volumetric data (file extension .hgv) produced by the Hilgus software. In the meaning of ultrasonic investigations, volumetric means two independent spatial coordinates and a position synchronized time signal (e.g. an amplitude scan, A-Scan). The code is also able two read the following types of scans (file extension .hgy) : brightness, contour, depth and feature scan. If the input file includes of multiple types of scans, they will read seperatley with their own amplitude resolution. 

# Usage
Some proof of concept code is attached in the *src* directory and is able to read the data files and convert them to MATLAB using the [matio library](https://sourceforge.net/projects/matio/).

