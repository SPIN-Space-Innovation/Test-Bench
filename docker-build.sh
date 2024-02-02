#!/bin/sh

# Build the Docker image
docker build -t space-innovation/embedded-toolchain:0.1 -f Dockerfile .

# Build project
docker run -it --rm \
 --mount type=bind,source=${PWD},target=/src \
 space-innovation/embedded-toolchain:0.1 \
 bash -c "cd src && cmake -B build -DCMAKE_BUILD_TYPE=Release -G 'Unix Makefiles' && cmake --build build -j 4"

