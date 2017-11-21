import os
import glob
from PIL import Image


path = os.getcwd()

for image_path in glob.glob(path+'/*.tiff'):
	png_file = image_path.split('.')[0] + '.png'

	print png_file
	
	try:
		im = Image.open(image_path) 
		im.thumbnail(im.size)
		im.save(png_file,"PNG",quality=100)

	except Exception, e:
		print e 
