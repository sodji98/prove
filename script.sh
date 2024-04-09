#!/bin/bash

pvs-studio-analyzer trace -- make -j8
pvs-studio-analyzer analyze -j8 -l PVS-Studio.lic -o PVS-Studio.log
plog-converter PVS-Studio.log -t sarif -o results.sarif
