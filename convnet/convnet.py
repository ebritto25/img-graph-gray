import cv2
import numpy as np
import time
from CNN import *
import glob
import os.path 



num_epochs = 100
batch_size = 100
path = "/home/eduardo/IC/textures/class_1/"
num_Classes = 13
num_Img = 100

<<<<<<< HEAD

=======
'''
def loadimages(path,n_Class,n_Img):
	image = []
	label = []
	for i in range(0,num_Img):
		img = cv2.cvtColor(cv2.imread(path+"/"+str(n_Class)+"/"+"output"+str(n_Class)+"_"+str(i)+".tiff"),cv2.COLOR_RGB2GRAY)
		image.append(img)
		label.append(n_Class-1)
		

	return image[:], label[:]
'''
>>>>>>> 67963418cca3556e0988ac9e4028b68af114f6bd
def loadimages(path,folder,img_codec):
    label = []
    label[:] = []
    file_path = path+str(folder)+'/*.png'
    images = [ tf.image.decode_png(x,channels = 1)  for x in glob.glob(file_path) ]
    label[0:len(images)] = [0] * num_Classes
    label[0:len(images)][folder-1] = folder-1 
    #label = [0]  * len(images) 
    #label[folder-1] = folder-1

    return images,label


with tf.Session() as sess:
    # Initialize all variables
    sess.run(tf.global_variables_initializer())
    
    # Add the model graph to TensorBoard
    writer.add_graph(sess.graph)
    
    # Loop over number of epochs
    for epoch in range(num_epochs):
        
        start_time = time.time()
        train_accuracy = 0
        
        for num_Class in range(1, num_Classes+1):
		    
	    # Get a batch of images and labels
	    x_batch, y_true_batch = loadimages(path,num_Class,'tiff')

	    print(sess.run(tf.shape(x_batch)))

	    # Put the batch into a dict with the proper names for placeholder variables
	    feed_dict_train = {x: x_batch[0:100], y_true: y_true_batch[0:100]}
	    
	    # Run the optimizer using this batch of training data.
	    sess.run(optimizer, feed_dict=feed_dict_train)
	    
	    # Calculate the accuracy on the batch of training data
	    train_accuracy += sess.run(accuracy, feed_dict=feed_dict_train)
	    
		
          
        train_accuracy /= (num_Classes*num_Img)
        

        end_time = time.time()
        
        print("Epoch "+str(epoch+1)+" completed : Time usage "+str(int(end_time-start_time))+" seconds")
        print("\tAccuracy:")
        print("\t- Training Accuracy:\t{}".format(train_accuracy))
