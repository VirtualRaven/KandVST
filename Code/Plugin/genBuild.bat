rmdir /Q /S build
mkdir build
cd build
cmake .. -DAudioPlugin_jucer_FILE=..\AudioPlugin.jucer
cd ..