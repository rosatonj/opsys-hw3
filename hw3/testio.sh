#!/bin/sh
# sapc.script used for hw1 grading
# Usage: testio.sh 5   to use system 5, etc.
# shell file to test hw1 through mtip
#  ~S for longer wait, ~s for shorter wait
#  use 20 secs for reset, then CR, then 1 sec, then ~d, 10 secs for download

mtip -b $1 -f testio.lnx << MRK
~r~S~S
~s~S~d~S~SGO 100100
~S~s0~s~s1~s~s2~s~s3~s~s4~s~s5~s~s6~s~s7~s~s8~s~s9~s~s0~s~s1~s~s2~s~s3~s~s4~s~s5~s~s6~s~s7~s~s8~s~s9~s~s0~s~s1~s~s2~s~s3~s~s4~s~s5~s~s6~s~s7~s~s8~s~s9~s~s0~s~s1~s~s2~s~s3~s~s4~s~s5~s~s6~s~s7~s~s8~s~s9~s~q
MRK






