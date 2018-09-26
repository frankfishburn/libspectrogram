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
```Matlab
cd('bindings/matlab');
make;
```

---
## Usage
See the **examples** directory
