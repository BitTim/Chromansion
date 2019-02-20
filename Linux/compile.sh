#!/bin/bash

echo Started Compiling...
g++ main.cpp -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -o Chromansion -std=c++11

if [ "$1" = "-c" ]; then
	echo Finished compiling! Due to an argument, the program will not run automatically
else
	echo "Finished Compiling! Now running the program..."
	./Chromansion
fi
