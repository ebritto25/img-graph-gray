#!/bin/bash

base_path="/home/rodrigo/ic/bases/rgbbrodatz/"
exec_path="/home/rodrigo/ic/build-img-graph-gray-Desktop-Debug/"
arff_path="/home/rodrigo/ic/resultados_script/rgbbrodatz/"

exec_name="img-graph-gray"


base_name="rgbbrodatz"
mst=1
color=1
codec=".tif"



cd $base_path
classes=$(ls -d */)


# Com mst RGB
number_of_images=$( expr $(ls -l $class$counter/ | wc -l) - 1)
$exec_path$exec_name $base_path$class $base_name $number_of_images $codec $color $mst $arff_path"rgbbrodatz_mst.arff" 

# Sem mst RGB
mst=0
$exec_path$exec_name $base_path$class $base_name $number_of_images $codec $color $mst $arff_path"rgbbrodatz.arff" 



# Com mst GRAY 
mst=1
color=0
$exec_path$exec_name $base_path$class $base_name $number_of_images $codec $color $mst $arff_path"rgbbrodatz_mst_gray.arff" 




# Sem mst GRAY 
mst=0
$exec_path$exec_name $base_path$class $base_name $number_of_images $codec $color $mst $arff_path"rgbbrodatz_gray.arff" 

