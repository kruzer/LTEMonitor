#!/bin/sh
cd lmgui
npm i
./node_modules/@angular/cli/bin/ng build --prod
cd ..
mkdir Release
cd Release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
