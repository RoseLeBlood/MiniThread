#!/bin/zsh

pio package pack
mv miniThread-2.10.*.tar.gz release/miniThread.tar.gz
cd examples/empty/
doas rm -r .pio/libdeps/

pio run &> fail.txt
