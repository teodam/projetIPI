#!/bin/bash
cd Images/

if [ -d .minies ] 
	then
		rm -R .minies
fi
if [ -d .micros ]
	then
		rm -R .micros
fi

mkdir .minies
mkdir .micros
mogrify -resize 1200x600 -path .minies *.ppm
mogrify -resize 200x100 -path .micros *.ppm
