# XNanoReadLine

#### Introduction
XNanoReadLine is an embedded line editing library that is similar to a simple GNU readline and can achieve the following functions. Based on XNanoReadLine, a terminal software based on serial port, TCP and other character stream interfaces can be quickly implemented.
1. Cursor movement
2. Character addition and deletion in lines
3. Shortcut key detection
4. History recording
5. Environment variable storage
6. Line string variable replacement, implementing ${var1} $var2
7. Line string split calculation, implementing "max 1 2", output parma[0]="max", parma[1]="1", parma[2]="2"

#### Compilation
```shell
sudo apt-get install cppcheck
make
make check
```
#### Usage
See example.c

#### Documentation
See detailed documentation in the doc directory.