#!/bin/bash

echo '
phoenix = .
cflags = -Ofast -g -fPIC -march=native
cppflags = $cflags -std=c++11
includes = -I $phoenix/include
libs = -L $phoenix/target -lphoenix -lSDL2

rule cpp
    command = clang++ $cppflags $includes -c $in -o $out
    description = compile $in

rule cc
    command = clang $cflags $includes -c $in -o $out
    description = compile $in

rule link
    command = clang++ $in $libs -o $out
    description = link $out

rule archive
    command = ar -cr $out $in
    description = archive $out
' > build.ninja

destfiles=""

for srcfile in src/*.c; do
    destfile=$(echo $srcfile | sed 's/src/target/;s/.c$/.o/') 
    echo "build $destfile: cc $srcfile" >> build.ninja
    destfiles="$destfiles $destfile"
done

#for srcfile in src/*.cpp; do
#    destfile=$(echo $srcfile | sed 's/src/target/;s/.cpp$/.o/') 
#    echo "build $destfile: cpp $srcfile" >> build.ninja
#done

echo "build target/libphoenix.a: archive $destfiles" >> build.ninja



