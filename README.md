# flatmessage

A code-generator that started out as a tool to generate network packet code and grew into a full-fledged compiler that lets you generate any code you want.

## Installation

Clone this repository to your local machine.

```git
git clone https://github.com/DennisWG/flatmessage.git
```

Make a new directory called `build` and invoke cmake from that directory. Make sure to provide a valid install-prefix (the location where you want it to be installed).

```
cd flatmessage
mkdir build && cd build
cmake -DCMAKE_INSTALL_PREFIX=<PATH/TO/INSTALL/FOLDER> ..
```

Build and install the project.

```
cmake --build . --config Release
cmake --install
```

## Add to PATH

If you haven't installed the project into a directory that is inside the PATH environment variable, consider adding the directory to PATH so that you can use it globally.

## Documentation

Hopefully some day!
