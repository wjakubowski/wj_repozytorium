import os, re, PyQt5, cv2
from glob import glob
from PyQt5.QtGui import QImageReader, QPixmap, QImage
from matplotlib.image  import imread



def supported_image_extensions():
	formats = QImageReader().supportedImageFormats()
	return [re.search('\'.*\'',str(fmt)).group(0)[1:-1] for fmt in formats]
	
def searchForImages(dirpath,extensions = ['jpg','png','bmp']):
	images = []
	for extension in extensions:
		pattern = os.path.join(dirpath, '*.%s' % extension)
		images.extend(glob(pattern))
		
	images = list(filter(isntCachedFile, images))
		
	return images;

	
def isntCachedFile(file):
	head, tile = os.path.split(file)
	pattern = re.compile("ExcludedRegion_.*")
	return not len(re.findall(pattern, file))
	
def loadPixmapWithExcludedRegion(file):
	selctedRegionImage = None
	head, tail = os.path.split(file)
	if os.path.isfile("{}/ExcludedRegion_{}".format(head,tail)):
	
		orginalImage = imread(file)
		selctedRegionImage = imread("{}/ExcludedRegion_{}".format(head,tail))
	
		selctedRegionGrayImage = cv2.cvtColor(selctedRegionImage,cv2.COLOR_BGR2GRAY)
		ret, mask = cv2.threshold(selctedRegionGrayImage, 10, 255, cv2.THRESH_BINARY)
		mask_inv = cv2.bitwise_not(mask)
		img1_bg = cv2.bitwise_and(orginalImage,orginalImage,mask = mask_inv)
		img2_fg = cv2.bitwise_and(selctedRegionImage,selctedRegionImage,mask = mask)

		completeImage = cv2.add(img1_bg,img2_fg)
		image = QImage(completeImage, completeImage.shape[1], completeImage.shape[0], completeImage.shape[1] * 3, QImage.Format_RGB888)
	
		return QPixmap(image)
	else:
		return QPixmap(file)

