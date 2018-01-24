import tensorflow as tf

filenames = "/home/eduardo/ProjQT/img-graph-gray/convnet/tfrecords/train.tfrecords"
dataset = tf.data.TFRecordDataset(filenames);
iterator = dataset.make_initializable_iterator();
next_element = iterator.get_next();


with tf.Session() as sess:
    # Compute for 100 epochs.
    for _ in range(100):
      sess.run(iterator.initializer)
      while True:
        try:
            sess.run(next_element)
        except tf.errors.OutOfRangeError:
            break

  # [Perform end-of-epoch calculations here.]
