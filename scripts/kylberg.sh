#!/bin/bash 
base_path="/home/rodrigo/ic/bases/kylberg/"
exec_path="/home/rodrigo/ic/build-img-graph-gray-Desktop-Debug/"
arff_path="/home/rodrigo/ic/resultados_script/kylberg/"

exec_name="img-graph-gray"


##Config
base_name="kylberg"
mst=1
color=0 # 0 -> GRAY | 1 -> RGB
codec=".pgn"
number_of_images=160
##

# Com mst GRAY
echo $exec_path$exec_name $base_path$base_name $number_of_images $codec $color $mst $arff_path$base_name"_mst.arff" 
$exec_path$exec_name $base_path $base_name $number_of_images $codec $color $mst $arff_path$base_name"_mst.arff" 


# Sem mst GRAY
mst=0
echo $exec_path$exec_name $base_path $base_name $number_of_images $codec $color $mst $arff_path$base_name".arff" 
$exec_path$exec_name $base_path $base_name $number_of_images $codec $color $mst $arff_path$base_name".arff" 




