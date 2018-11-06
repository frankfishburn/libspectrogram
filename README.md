# libspectrogram

[![Build Status](https://travis-ci.org/frankfishburn/libspectrogram.svg?branch=master)](https://travis-ci.org/frankfishburn/libspectrogram)
[![Open Issues](https://img.shields.io/github/issues/frankfishburn/libspectrogram.svg)](https://github.com/frankfishburn/libspectrogram/issues)
[![LGPLv3 License](https://img.shields.io/github/license/frankfishburn/libspectrogram.svg)](https://github.com/frankfishburn/libspectrogram/blob/master/LICENSE)

## About
**libspectrogram** is a C++ library for computing spectrograms and periodograms, with a C interface and MATLAB bindings

---
## Building
### Library
```bash
mkdir build
cd build
cmake ..
make
make test
```
### Matlab
Specify root directory for the matlab installation (`Matlab_ROOT_DIR`) and optionally the output directory (`MATLAB_INSTALL_PATH`) when you run cmake:
```bash
cmake -DMatlab_ROOT_DIR=/opt/matlab/R2018b -DMATLAB_INSTALL_PATH=$HOME/matlab/libspectrogram ..
make;
make install;
```

---
## Usage
See the **examples** directory
