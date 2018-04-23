# Password Manager
__This project is NOT DONE YET__

<br>

## Platforms Supported
* Windows
* Linux

_This code might run on other platforms, but it has not been configured or tested_

<br>

## Compiling

This program is designed to work with the cross-platform IDE [Code::Blocks](http://www.codeblocks.org/). For Windows platforms, be sure to download and install Code::Blocks with MinGW included.

Before compiling, download and install the [GNU Realine Library](https://tiswww.case.edu/php/chet/readline/rltop.html):
* __Debian Linux:__ sudo apt-get install libreadline6 libreadline6-dev
* __Fedora Linux:__ sudo yum install libreadline6 libreadline6-dev
* __Windows:__ Download [Readline for Windows](http://gnuwin32.sourceforge.net/packages/readline.htm).
  1. Copy the contents of the "include" folder into _\<CodeBlocks\>\MinGW\include_
  2. Copy the contents of the "lib" folder into _\<CodeBlocks\>\MinGW\lib_
  3. Copy the runtime binaries (history5.dll and readline5.dll) into the same folder as the target application (found in _PasswordManager\bin\Debug_ or _PasswordManager\bin\Release_)

When compiling on Windows, you might need to define the constant *\_WIN32*. In the Code::Blocks toolbar, go to Settings -> Compiler Settings -> Global Compiler Settings. Under the tab #defines, add the line *\_Win32*.

Open PasswordManager.cbp in Code::Blocks to edit the project and compile the code.

<br>

## How to Use
*This is how it will work in the future, not how it works right now*

PasswordManager is designed to be a full-line command shell. Run it from the terminal, and pass in the name of the file you would like to load. If everything is successful, you should see a command prompt. Type help for a list of commands.

<br>

## TODO's
* Get the Key and Item fields working
* Load and save files
* Validate the loaded fields
* Encryption with AES and SHA-256

<br>

## Credits
* AES implementation is from the repository [tiny-AES-c](https://github.com/kokke/tiny-AES-c) by kokke.
* SHA-256 is from the repository [crypto-algorithms](https://github.com/B-Con/crypto-algorithms/) by Brad Conte (B-Con).
