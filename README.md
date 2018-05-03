# KandVST
[![Build status](https://build.rahmn.net/guestAuth/app/rest/builds/buildType:id:KandVST_Build/statusIcon.svg)]()
## Download ##
Prebuilt binaries for windows/linux are available at [releases](https://github.com/VirtualRaven/KandVST/releases)
## Cloning ##
Please note that git lfs is required and should be installed before cloning, see [lfs](https://git-lfs.github.com/).
After git lfs is installed clone the repo using:  
`git clone https://github.com/VirtualRaven/KandVST.git`

## Building ##

The project is tested on linux and windows and can be built using cmake.
On linux the project has been tested with both gcc and clang and both the makefile and ninja generators. On windows
it has been confirmed to work with Visual Studio 2017.

To build open a terminal in folder `KandVst/Code/Plugin` and execute the following sequence of 
commands 

`mkdir build`  
`cd build`  
`cmake ..`  

This will configure the project, to then build the project run  
`cmake --build .`

To test the build run (Windows only)   
`ctest -C Debug`

### Visual Studio 2017 ###
If you are using Visual Studio 2017 with cmake support one can open the project directly in VS. 
Select File/Open/CMake and select the file `KandVST/Code/Plugin/CMakeLists.txt`.

## Testing ##
The project includes an automatic tester tool aptly named tester.
Tests are defined in `Code/Plugin/Tester/Tests/TestName/` and made from
the following components
   * __TestName.h__ - This class contains an implementation of `TestHost::Test` named TestName
   * __params.txt__ - Contains a list of default values for parameters. The synth parameters are set to these before 
                      running the test, any parameter omitted is initialized to it's default value.
   * __verify.py__  - A python script runs post test. As an argument it is given the path to the test output folder.
 Only TestName.h is required, the others are optional. If the scripts considers the test a success, it should return 42.
 
 As long as the test is placed in the correct folder, it is automatically registerd by cmake at generation time, 
 no changes are needed to the testers code.
 
 Unfortunately the tester tool is not supported on linux as it uses the VST3 api, which JUCE (project dependency)
 does not support on linux platforms.
 
 #### Test output ####
 All tests output is found in `BUILD_FOLDER/Tests/TestName/`. If the test writes anything to the msg buffer, 
 a file named `log.txt` will be created in the folder containing this data.
 If the test method `exportTestData()` returns true, then the files `data_1.txt` and `data_2.txt`. These
 files are CSV files contaning raw sound data for the left and right channels. 
 
 #### params.txt ####
  Each line of this file either contains a comment, marked by a #, or
  it contains a parameter assigment:  
  `PARAM_NAME = 0.1`
  The right-hand side must be a valid value of the parameter.
  If the parameter is of continuous type the value must a floating point value.
  If the parameter is of toggle type it must be one of On,Off,True,False.
  If the parameter is discrete the value must be an integer inside the parameters range.
  
  The left hand side must be a name of parameter that the plugin export.
  For example:
  ```
   #Enable all osc
   Enable Oscillator 0 = On
   Enable Oscillator 1 = On
   Enable Oscillator 2 = On
   Enable Oscillator 3 = On
  ```
  To list available parameters, run the tester with the argument `params` or `Params` for a more detailed output. Or 
  use the list parameters command in the synth terminal window.
  
  Placing the comment `#NORMALIZED`turns off type checking, instead all values are interpreted as normalized floating point   values. This only effects rows after the line which the comment appeared on. 
  Similarly `#UNNORMALIZED` reenables type checking for the rows following it.

## Build dependencies
The project uses cmake as the build system, so cmake is required

### Linux packages 
  * libasound2-dev
  * libfreetype6-dev
  * libcurl-dev
  * libxinerama-dev
  * libxrandr-dev
  * libxcursor-dev
  * freeglut3-dev  
  * libxcomposite-dev
  * mesa-common-dev
  * libxrender-dev
  * PkgConfig
### Windows 
   Visual studio 2017

### Additional dependencies ###
  To be able to run all tests and/or to rebuild binary resources python3 is required together with the following python modules
  * numpy
  * matplotlib
  * peakutils
