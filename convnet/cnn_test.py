import cv2
from func_cnn import *

image = cv2.imread("/home/eduardo/ProjQt/img-graph-gray/convnet/imgtest.tiff")
img = cv2.cvtColor(image,cv2.COLOR_RGB2GRAY)

x = tf.placeholder(tf.int32)
x_image = tf.reshape(x,[-1,32,32,1])

cat = tf.placeholder(tf.int32)

layer_conv,weights_conv = new_conv_layer(input = x_image,num_input_channels = 1,filter_size = 5,num_filters = 2, name = "conv")

layer_relu = new_relu_layer(input = layer_conv,name = "RELU")

layer_pool = new_pool_layer(input = layer_relu,name = "pool")

num_features = layer_pool.get_shape()[1:4].num_elements()
layer_flat = tf.reshape(layer_pool,[-1,num_features])

layer_fc1 = new_fc_layer(input = layer_flat,num_inputs = num_features,num_outputs = 128,name = "fc1")

layer_relu2 = new_relu_layer(input = layer_fc1,name = "RELU2")

layer_fc2 = new_fc_layer(input = layer_relu2,num_inputs = 128,num_outputs = 2,name = "fc1")

with tf.variable_scope("Softmax"):
	cat_pred = tf.nn.softmax(layer_fc2)
	cat_pred2 = tf.argmax(cat_pred,axis = 1)

with tf.name_scope("cross_ent"):
	cross_entropy = 
