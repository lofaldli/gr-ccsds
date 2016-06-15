# gr-ccsds

this is part a GNU Radio module for processing data which is encoded with the CCSDS Recommended Standard for TM Synchronization and Channel Coding using Reed Solomon.

it was done as part of my master thesis at NTNU in the spring of 2016.

## install instructions

to use the blocks, you need to install GNU Radio (I recommend using [pybombs](https://github.com/gnuradio/pybombs))

then clone this repo and follow these instructions

    mkdir build/
    cd build/
    cmake .. -DCMAKE_INSTALL_PREFIX=/path/to/your/prefix
    make
    make install
