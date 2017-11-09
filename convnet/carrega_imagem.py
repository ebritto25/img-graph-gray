import glob
import cv2
import os.path 


def loadimages(path,folder,img_codec):
    file_path = path+str(folder)+'/*.'+img_codec
    images = [ cv2.cvtColor(cv2.imread(x),cv2.COLOR_RGB2GRAY)  for x in glob.glob(file_path) ]
    label = [folder]  * len(images) 
    
    return images, label




