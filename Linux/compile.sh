#!/bin/bash

echo Started Compiling...
g++ main.cpp -lSDL2 -lSDL2_ttf -lSDL2_image -lSDL2_mixer -o Chromansion

if [ "$1" = "-c" ]
then echo Finished compiling! Due to an argument, the program will not run automatically
	return 0
fi

echo "Finished Compiling! Now running the program..."
./Chromansion
