# gr-ccsds ![][badge1] ![][badge2] ![][badge3] 

this is a GNU Radio module for processing data which is encoded according to the CCSDS Recommended Standard for [TM Synchronization and Channel Coding][ccsds].
it handles Reed Solomon, interleaving and scrambling/randomization.

it was done as part of my master thesis at [NTNU][ntnu] in the spring of 2016.

## installing

to use the blocks, you need to install GNU Radio.

the simplest method on Ubuntu 16.04 is to use 

    sudo apt install git cmake swig gnuradio
    
or with [PyBOMBS][pybombs] if you want the newest version

then clone this repo and follow these instructions

    mkdir build/
    cd build/
    cmake .. -DCMAKE_INSTALL_PREFIX=$(gnuradio-conifg-info --prefix)
    make
    [sudo] make install

[ccsds]: https://public.ccsds.org/Pubs/131x0b2ec1.pdf
[ntnu]: https://ntnu.edu
[pybombs]: https://github.com/gnuradio/pybombs
[badge1]: https://img.shields.io/badge/Ubuntu-14.04-brightgreen.svg
[badge2]: https://img.shields.io/badge/Ubuntu-16.04-brightgreen.svg
[badge3]: https://img.shields.io/badge/Ubuntu-17.04-red.svg
