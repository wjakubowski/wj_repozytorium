import sys, os, re, PyQt5
from util import supported_image_extensions, searchForImages
from glob import glob
from Paint import Paint
from PyQt5.QtWidgets import QApplication, QWidget, QInputDialog, QLineEdit, QFileDialog, QListWidget, QVBoxLayout, QListWidgetItem, QMenu, QAbstractItemView
from PyQt5.QtGui import QImageReader, QIcon
from PyQt5.QtCore import QByteArray

class ImageFileList(QListWidget):
 
	def __init__(self, parent=None):
		QListWidget.__init__(self, parent)
		self.setSelectionMode(QAbstractItemView.ExtendedSelection)
			
	def addFilesToList(self,files):
		self.clear()
		for file in files:
			item = QListWidgetItem(self)
			item.setText(file)
			item.setIcon(QIcon(file))
			
	def contextMenuEvent(self, event):
		menu = QMenu(self)
		deleteSelectedItemsAction = menu.addAction("Delete Selected Items")
		markExcludedRegionAction = menu.addAction("Mark Excluded Region")
		action = menu.exec_(self.mapToGlobal(event.pos()))
		if action == deleteSelectedItemsAction:
			for selectedItem in self.selectedItems():
				self.takeItem(self.row(selectedItem))
		if action == markExcludedRegionAction:
			for selectedItem in self.selectedItems():
				self.markExcludedRegion(
				selectedItem.text())
				
	def markExcludedRegion(self, imagePath):
		Paint(imagePath)


if __name__ == '__main__':

	app = QApplication([])
 
	imageListWidget = QWidget()
	imageListWidget.setWindowTitle('Image List')
	imageListWidget.setMinimumSize(600, 400)
	
	layout = QVBoxLayout()
	imageListWidget.setLayout(layout)
 
	imageList = ImageFileList(imageListWidget)
	layout.addWidget(imageList)
	
	"""entry = QLineEdit(imageListWidget)
	layout.addWidget(entry)
	def on_item_changed(curr, prev):
		entry.setText(curr.text())
	imageList.currentItemChanged.connect(on_item_changed)"""
	
	imagesFromDirectory = searchForImages(sys.argv[1], supported_image_extensions())
	imageList.addFilesToList(imagesFromDirectory)

	imageListWidget.show()
	app.exec_()