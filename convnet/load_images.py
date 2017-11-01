import cv2

def loadimages(path,n_Classes,n_Img):
	#path/textures/class_1/1
	image = []
	label = []
	for i in range(1,n_Classes+1):
		for j in range(n_Img):
			image.append(cv2.imread(path+"/"+str(i)+"/"+"output"+str(i)+"_"+str(j)+".tiff"))
			label.append("class_"+str(i))
			
	return image[:], label[:]



x,y = loadimages("/home/eduardo/ProjQt/textures/class_1",2,10)

#cv2.imshow("teste",cv2.imread("/home/eduardo/ProjQt/textures/class_1/1/output1_45.tiff"))
#cv2.waitKey(0)

#print(cv2.imread("/home/eduardo/ProjQt/textures/class_1/1/output1_45.tiff"))
#print(x,y)
	
