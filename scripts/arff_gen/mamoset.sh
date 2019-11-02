#!/bin/bash

base_path="/home/rodrigo/mamoset/MAMMOSET-SPLIT/"
exec_path="/home/rodrigo/gdrive/ESTUDOS/faculdade/iniciacao/build-img-graph-gray/"
arff_path="/home/rodrigo/"

exec_name="img-graph-gray"


base_name="mamoset"
mst=1
color=0
codec=".png"



cd $base_path
folders_path=$(ls -d */)


counter=1

# Com mst
for class in $folders_path; do
    number_of_images=$( expr $(ls -l $class$(cat $base_path$class"folders_name.txt")/ | egrep *$codec | wc -l) - 1)
    echo $exec_path$exec_name $base_path $base_name $number_of_images $codec $color $mst $arff_path"class$counter""_mst.arff" 
    sleep 1
    $exec_path$exec_name $base_path$class $base_name $number_of_images $codec $color $mst $arff_path"class$counter""_mst.arff" 

    counter=$((counter + 1))
done 


# Sem mst
counter=1
mst=0

for class in $folders_path; do
    number_of_images=$( expr $(ls -l $class$(cat $base_path$class"folders_name.txt")/ | egrep *$codec | wc -l) - 1)
    echo $exec_path$exec_name $base_path$class $base_name $number_of_images $codec $color $mst $arff_path"class$counter.arff" 
    $exec_path$exec_name $base_path$class $base_name $number_of_images $codec $color $mst $arff_path"class$counter.arff" 
    counter=$((counter + 1))
done 










