# tppSSS

[![Actions Status](https://github.com/trackpadpro/tppsss/workflows/CMake%20MSYS2/badge.svg)](https://github.com/trackpadpro/tppsss/actions)
[![Actions Status](https://github.com/trackpadpro/tppsss/workflows/CMake%20Native/badge.svg)](https://github.com/trackpadpro/tppsss/actions)

## Building & Running

### Prerequisites

Terminal

Git

CMake with a build tool

C/C++ compiler

Curl library 
* If none or not found, use commands in .github/workflows to build/find dependencies

### Build Commands

```
cd \[DesiredDownloadDirectory]\

git clone https://github.com/trackpadpro/tppsss.git

cd .\tppsss\

.\bootstrap.bat   #scripts/bootstrap.ps1
```

### Run Commands

```
.\tripleS.exe

break             #end the program

fetch             #request buy/sell calculations

setup             #input user-specific parameters
```

#

Not financial advice
