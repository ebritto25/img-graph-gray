import os
import sys
import glob
from PIL import Image


if len(sys.argv) < 2:
	print 'Caminho Nao Informado!\n Informar Caminho como argumento'
else:
	path = sys.argv[1] 


	for image_path in glob.glob(path+'/*.tiff'):
		png_file = image_path.split('.')[0] + '.png'

		print 'Output: ' + png_file

		try:
			im = Image.open(image_path) 
			im.thumbnail(im.size)
			im.save(png_file,"PNG",quality=100)
		except Exception, e:
			print e 
