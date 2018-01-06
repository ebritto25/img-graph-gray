#!/bin/bash

class_path=$1

python_script="tiff_to_png.py"


folders="$class_path*/"

for class in $folders; do

    echo python $python_script $class
    python $python_script $class


done 
