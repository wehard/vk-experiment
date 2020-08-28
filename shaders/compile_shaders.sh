#!/bin/sh

shader_files=`find -name "*.vert" -o -name "*.frag"`

for f in $shader_files
do
	file=`basename "$f"`
	type="${file##*.}"
	filename="${file%.*}"
	ext=.spv
	glslangValidator -o "$filename"_"$type$ext" -V $f
done
