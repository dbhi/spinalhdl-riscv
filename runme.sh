#!/bin/sh

# Command to execute this script in a docker container:
# docker run --rm -u $(id -u) -itv $(pwd):/src -w /src aptman/vhdl:spinal sh -c "./runme.sh"

set -e

cd $(dirname $0)
rootDir=$(pwd)

cd soft/murax/demo/
make clean all

cd "$rootDir/VexRiscv"
sbt "run-main vexriscv.demo.MuraxWithRamInit"
cp Murax.vhd ../
