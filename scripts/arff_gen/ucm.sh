#!/bin/bash 
base_path="/home/rodrigo/ic/bases/ucm/Images/"
exec_path="/home/rodrigo/ic/build-img-graph-gray-Desktop-Debug/"
arff_path="/home/rodrigo/ic/resultados_script/ucm/"

exec_name="img-graph-gray"


base_name="ucm"
mst=1
color=0 # 0 -> GRAY | 1 -> RGB
codec=".tif"
number_of_images=100



# Com mst GRAY
echo $exec_path$exec_name $base_path$base_name $number_of_images $codec $color $mst $arff_path$base_name"_mst.arff" 
$exec_path$exec_name $base_path $base_name $number_of_images $codec $color $mst $arff_path$base_name"_mst.arff" &


# Sem mst GRAY
mst=0
echo $exec_path$exec_name $base_path $base_name $number_of_images $codec $color $mst $arff_path$base_name".arff" 
$exec_path$exec_name $base_path $base_name $number_of_images $codec $color $mst $arff_path$base_name".arff" & 

wait

color=1

# Com mst RGB
echo $exec_path$exec_name $base_path$base_name $number_of_images $codec $color $mst $arff_path$base_name"_mst_rgb.arff" 
$exec_path$exec_name $base_path $base_name $number_of_images $codec $color $mst $arff_path$base_name"_mst_rgb.arff" &


# Sem mst RGB
mst=0
echo $exec_path$exec_name $base_path $base_name $number_of_images $codec $color $mst $arff_path$base_name"_rgb.arff" 
$exec_path$exec_name $base_path $base_name $number_of_images $codec $color $mst $arff_path$base_name"_rgb.arff" &

wait








