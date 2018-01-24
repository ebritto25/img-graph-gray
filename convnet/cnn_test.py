#import cv2
from func_cnn import *
from PIL import Image
path = ['/home/eduardo/ProjQT/img-graph-gray/convnet/imgtest.png']
#image = cv2.imread("/home/eduardo/ProjQT/img-graph-gray/convnet/imgtest.png")
#img = cv2.cvtColor(image,cv2.COLOR_RGB2GRAY)

queue = tf.train.string_input_producer(path)
ok = [1]
image_reader = tf.WholeFileReader()
filename, image = image_reader.read(queue)

img = tf.image.decode_image(image,1)

x = tf.placeholder(tf.float32)
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
	cat_pred2 = tf.argmax(cat_pred,axis = 1,output_type=tf.int32)

with tf.name_scope("cross_ent"):
	cross_entropy = tf.nn.softmax_cross_entropy_with_logits(logits=layer_fc2, labels=cat)
	cost = tf.reduce_mean(cross_entropy)

with tf.name_scope("optimizer"):
	optimizer = tf.train.AdamOptimizer(learning_rate=1e-4).minimize(cost)

with tf.name_scope("accuracy"):
	correct_prediction = tf.equal(cat_pred2,cat)
	accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))


with tf.Session() as sess:
	sess.run(tf.global_variables_initializer())

	
	for epoch in range(10):
		
		train_accuracy = 0

		inp = sess.run(img)

		feed_dict_train = {x:inp,cat:ok}

		sess.run(optimizer, feed_dict=feed_dict_train)

		train_accuracy += sess.run(accuracy, feed_dict=feed_dict_train)

		print('Epoch: '+str(epoch)+' Accuracy: ' +str(train_accuracy))
