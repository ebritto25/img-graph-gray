import tensorflow as tf
tf_path = '/home/eduardo/ProjQT/img-graph-gray/convnet/tfrecords/train.tfrecords'

def unpackRecords(tf_path):
	record_iterator = tf.python_io.tf_record_iterator(path = tf_path)
	images = []
	labels = []
	for string_record in record_iterator:

		example = tf.train.Example()
		example.ParseFromString(string_record)

		image_string = (example.features.feature['train/image'].bytes_list.value[0])
		label = int(example.features.feature['train/label'].int64_list.value[0])

		images.append(image_string)
		labels.append(label)

	return images, labels



images, labels = unpackRecords(tf_path)
batch = 32
for i in xrange(0,len(images),batch):
	print(labels[i:i+batch])

print(len(images))
print(len(labels))
