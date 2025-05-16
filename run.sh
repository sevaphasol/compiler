#!/bin/bash


if [ "$#" -ne 3 ]; then
  source_path="source.txt"
  frontend_path="frontend.txt"
  backend_path="backend.s"
else
  source_path="$1"
  frontend_path="$2"
  backend_path="$3"
fi

cd commons
make clean
make

cd ../front-end
make clean
make

cd ../back-end
make clean
make

cd ..
front-end/build/frontend $source_path $frontend_path
back-end/build/backend  $frontend_path $backend_path
