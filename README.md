# gr-ccsds

this is a GNU Radio module for processing data which is encoded according to the [CCSDS][ccsds] 131.0-B standard.
it handles Reed Solomon, interleaving and scrambling/randomization.

it originally was done as part of my master thesis at [NTNU][ntnu] in the spring of 2016.

## installing

to use the blocks, you need to install GNU Radio.

the simplest method on Ubuntu 16.04 is to use 

    sudo apt install git cmake swig gnuradio
    
or with [PyBOMBS][pybombs] if you want the newest version

then clone this repo and follow these instructions

    mkdir build/
    cd build/
    cmake .. -DCMAKE_INSTALL_PREFIX=$(gnuradio-config-info --prefix)
    make
    [sudo] make install

[ccsds]: https://public.ccsds.org/Publications/BlueBooks.aspx
[ntnu]: https://ntnu.edu
[pybombs]: https://github.com/gnuradio/pybombs
