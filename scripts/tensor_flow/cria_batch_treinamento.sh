#!/bin/bash

# Script pra criar pasta com as imagens para o treinamento
# ..

batch_size=52

# Endereço das classes
path="/home/rodrigo/textures/"

# Criando pasta de treinamento
for i in $(seq 1 5); do 
    mkdir $path"class_"$i"/train" -p -v
done


# Escolhendo imagens aleatorias
# e copiando-as para a pasta de treinamento
for i in $(seq 1 5); do

    cd $path"class_"$i
    
    for folder in $(ls -d */ | egrep '[1-9]'); do 
        
        count=0
        for image in $(ls $folder | sort -R | egrep '*.png' ); do
            if [ "$count" -ne $batch_size ]; then
               cp $folder$image $path"class_"$i"/train/" --verbose
               count=$((count + 1))
            fi
            
        done 

    done

    cd ..

done
