# PasswordManager
__This project is NOT DONE YET__

<br>

## How to Compile

This program is designed to work with the cross-platform IDE [Code::Blocks](http://www.codeblocks.org/). To compile, open PasswordManager.cbp in Code::Blocks, and it should compile.

When compiling for Windows, you might need to define the constant *\_WIN32*. In the Code::Blocks toolbar, go to Settings -> Compiler Settings -> Global Compiler Settings. Under the tab #defines, add the line *\_Win32*.

<br>

## Platforms Supported
* Windows
* Linux

_This code might run on other platforms, but it has not been configured or tested_

<br>

## How to Use
*This is how it will work in the future, not how it works right now*

PasswordManager is designed to be a full-line command shell. Run it from the terminal, and pass in the name of the file you would like to load. If everything is successful, you should see a command prompt. Type help for a list of commands.

<br>

## TODO's
* Get the Key and Item fields working
* Write the code that run the command shell
* Load and save files
* Validate the loaded fields
* Encryption with AES and SHA-256
