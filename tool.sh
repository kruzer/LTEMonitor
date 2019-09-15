#!/bin/sh
rm -rf Release/ Debug/ build/

cd lmgui
npm install
ng build --prod
cd ..

mkdir Release
cd Release
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release
make
strip LTEMonitor

cd ..

mkdir Debug
cd Debug
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug
make
