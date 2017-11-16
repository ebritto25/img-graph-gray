#!/bin/bash

base_path="/home/rodrigo/textures/"
exec_path="/home/rodrigo/gdrive/ESTUDOS/iniciacao/build-img-graph-gray-Desktop_Qt_5_9_1_GCC_64bit-Debug/"
arff_path="/home/rodrigo/gdrive/ESTUDOS/iniciacao/resultados/brodatz/"

exec_name="img-graph-gray"


base_name="brodatz"
mst=1
codec=".tiff"



cd $base_path
folders_path=$(ls -d */)


counter=1

# Com mst
for class in $folders_path; do
    number_of_images=$( expr $(ls -l $class$counter/ | wc -l) - 1)
    $exec_path$exec_name $base_path$class $base_name $number_of_images $codec $mst $arff_path"class$counter""_mst.arff" 

    counter=$((counter + 1))
done 

# Sem mst
counter=1
mst=0

for class in $folders_path; do
    number_of_images=$( expr $(ls -l $class$counter/ | wc -l) - 1)
    $exec_path$exec_name $base_path$class $base_name $number_of_images $codec $mst $arff_path"class$counter.arff" 

    counter=$((counter + 1))
done 










