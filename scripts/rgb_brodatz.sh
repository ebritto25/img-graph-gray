#!/bin/bash

base_path="/home/rodrigo/ic/bases/rgbbrodatz/"
exec_path="/home/rodrigo/ic/build-img-graph-gray-Desktop-Debug/"
arff_path="/home/rodrigo/ic/resultados_script/rgbbrodatz/"

exec_name="img-graph-gray"


base_name="rgbbrodatz"
mst=1
color=1
codec=".tif"
counter=1



cd $base_path
classes=$(ls -d */)


# Com mst RGB
for class in $classes; do
    number_of_images=$( expr $(ls -l $class$counter/ | wc -l) - 1)
    $exec_path$exec_name $base_path$class $base_name $number_of_images $codec $color $mst $arff_path"class$counter""_mst.arff" 
    counter=$((counter + 1))
done 

# Sem mst RGB
counter=1
mst=0

for class in $classes; do
    number_of_images=$( expr $(ls -l $class$counter/ | wc -l) - 1)
    $exec_path$exec_name $base_path$class $base_name $number_of_images $codec $color $mst $arff_path"class$counter.arff" 
    counter=$((counter + 1))
done 



# Com mst GRAY 
counter=1
mst=1
color=0

for class in $classes; do
    number_of_images=$( expr $(ls -l $class$counter/ | wc -l) - 1)
    $exec_path$exec_name $base_path$class $base_name $number_of_images $codec $color $mst $arff_path"class$counter""_mst_gray.arff" 

    counter=$((counter + 1))
done 



# Sem mst GRAY 
counter=1
mst=0

for class in $classes; do
    number_of_images=$( expr $(ls -l $class$counter/ | wc -l) - 1)
    $exec_path$exec_name $base_path$class $base_name $number_of_images $codec $color $mst $arff_path"class$counter""_gray.arff" 

    counter=$((counter + 1))
done 
