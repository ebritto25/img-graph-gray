#!/bin/bash

base_path="/home/rodrigo/ic/bases/ucm/Images/"
exec_path="/home/rodrigo/ic/build-img-graph-gray-Desktop-Debug/"
arff_path="/home/rodrigo/ic/resultados_script/ucm/"

exec_name="img-graph-gray"


base_name="ucm"
mst=1
color=0 # 0 -> GRAY | 1 -> RGB
codec=".tiff"
number_of_images=100



# Com mst
$exec_path$exec_name $base_path$base_name $number_of_images $codec $color $mst $arff_path$base_name".arff" 
echo $exec_path$exec_name $base_path$base_name $number_of_images $codec $color $mst $arff_path$base_name".arff" 


# Sem mst
mst=0
$exec_path$exec_name $base_path$class $base_name $number_of_images $codec $color $mst $arff_path$base_name".arff" 
echo $exec_path$exec_name $base_path$base_name $number_of_images $codec $color $mst $arff_path$base_name".arff" 










