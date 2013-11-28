#!/bin/sh
# sapc.script used for hw3 testing (no input case)
# Usage: uprog.sh 5   to use system 5, etc.
# shell file to test hw3 through mtip
#  ~S for longer wait, ~s for shorter wait
#  use 20 secs for reset, then CR, then 1 sec, then ~d, 10 secs for download

mtip -b $1 -f uprog.lnx << MRK
~r~S~S
~s~S~d~S~SGO 100100
~S~S
MRK






