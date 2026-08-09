#!/bin/sh
cd lmgui
npm i
# Angular 22: the CLI entry point is ng.js, and production is the default
# configuration (the --prod flag was removed in Angular 12).
node ./node_modules/@angular/cli/bin/ng.js build
cd ..
mkdir -p Release
cd Release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
