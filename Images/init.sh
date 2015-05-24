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

for file in *
do
	nom=`echo $file | rev | cut -d "." -f 2- | rev`
	ext=`echo $file | rev | cut -d "." -f 1 | rev`
	if [ $ext = JPG ]
		then
			convert $nom.JPG $nom.ppm
	fi
	if [ $ext = jpg ]
		then
			convert $nom.jpg $nom.ppm
	fi
	if [ $ext = JPEG ]
		then
			convert $nom.JPEG $nom.ppm
	fi
	if [ $ext = jpeg ]
		then
			convert $nom.jpeg $nom.ppm
	fi
done

mogrify -resize 1200x600 -path .minies *.ppm
mogrify -resize 200x100 -path .micros *.ppm

for file in *.ppm
do
	rm $file
done