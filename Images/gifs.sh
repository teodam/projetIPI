#!/bin/bash
if [ -d .gifs ] 
	then
		rm -R .gifs
fi

mkdir .gifs
mogrify -resize 200x100 -path .gifs *.ppm
cd .gifs
for file in ../*.ppm
	do
	mkdir tmp
	cd tmp
	cp ../../testf ./
	./testf $file 2
	rm testf
	rm $file
	files=""
	for fileno in $(ls | cut -d'.' -f 1,2 | sort -n); do
		files=$files" "$fileno".ppm"
	done
	convert -delay 5 -loop 0 $files $file.gif
	rm *.ppm
	cd ..
	rm -R tmp
done
