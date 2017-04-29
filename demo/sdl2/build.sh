#!/bin/bash
gcc -I../../src/ ../../src/cmixer.c src/main.c -lSDL2 -Wall -O3
