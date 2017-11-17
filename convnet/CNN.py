from func_cnn import *

# Placeholder variable for the input images
x = tf.placeholder(tf.float32, shape=[None,32,32], name='X')
# Reshape it into [num_images, img_height, img_width, num_channels]
x_image = tf.reshape(x, [-1, 32, 32, 1])
#x_image = tf.reshape(x, [32, 32, 1])


# Placeholder variable for the true labels associated with the images
y_true = tf.placeholder(tf.int32, name='y_true')
y_true_cls = tf.argmax(y_true)


############## CNN CREATION ############
# Convolutional Layer 1
layer_conv1, weights_conv1 = new_conv_layer(input=x_image, num_input_channels=1, filter_size=5, num_filters=6, name ="conv1")

# RelU layer 1
layer_relu1 = new_relu_layer(layer_conv1, name="relu1")

# Pooling Layer 1
layer_pool1 = new_pool_layer(layer_relu1, name="pool1")

# Convolutional Layer 2
layer_conv2, weights_conv2 = new_conv_layer(input=layer_pool1, num_input_channels=6, filter_size=5, num_filters=16, name= "conv2")

# RelU layer 2
layer_relu2 = new_relu_layer(layer_conv2, name="relu2")

# Pooling Layer 2
layer_pool2 = new_pool_layer(layer_relu2, name="pool2")

# Flatten Layer
num_features = layer_pool2.get_shape()[1:4].num_elements()
layer_flat = tf.reshape(layer_pool2, [-1, num_features])

# Fully-Connected Layer 1
layer_fc1 = new_fc_layer(layer_flat, num_inputs=num_features, num_outputs=128, name="fc1")

# RelU layer 3
layer_relu3 = new_relu_layer(layer_fc1, name="relu3")

# Fully-Connected Layer 2
layer_fc2 = new_fc_layer(input=layer_relu3, num_inputs=128, num_outputs=13, name="fc2")


# Use Softmax function to normalize the output
with tf.variable_scope("Softmax"):
    y_pred = tf.nn.softmax(layer_fc2)
    y_pred_cls = tf.argmax(y_pred, dimension=1)

# Use Cross entropy cost function
with tf.name_scope("cross_ent"):
    cross_entropy = tf.nn.sparse_softmax_cross_entropy_with_logits(logits=layer_fc2, labels=y_true)
    cost = tf.reduce_mean(cross_entropy)

# Use Adam Optimizer
with tf.name_scope("optimizer"):
    optimizer = tf.train.AdamOptimizer(learning_rate=1e-4).minimize(cost)


# Accuracy
with tf.name_scope("accuracy"):
    correct_prediction = tf.equal(y_pred_cls, y_true_cls)
    accuracy = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

writer = tf.summary.FileWriter("Training_FileWriter/")


