import PyQt5
from util import supported_image_extensions
from PyQt5.QtWidgets import QApplication, QWidget, QListWidget, QVBoxLayout, QListWidgetItem, QMenu, QAbstractItemView
from PyQt5.QtCore import *
from PyQt5.QtGui import *

class ExtensionList(QListWidget):
 
	def __init__(self, parent=None):
		QListWidget.__init__(self, parent)
		self.setSelectionMode(QAbstractItemView.ExtendedSelection)
		for extension in supported_image_extensions():
			item = QListWidgetItem(self)
			item.setFlags(item.flags() | Qt.ItemIsUserCheckable)
			if extension in ['bmp', 'png', 'jpg']:
				item.setCheckState(Qt.Checked)
			else:
				item.setCheckState(Qt.Unchecked)
			item.setText(extension)
			
	def getExtensions(self):
		extensions = []
		for i in range(self.count()):
			if self.item(i).checkState():
				extensions.append(self.item(i).text())
		return extensions
		
	def contextMenuEvent(self, event):
		menu = QMenu(self)
		printExtensinsAction = menu.addAction("Print extensions")
		selectAllItemsAction = menu.addAction("Select all items")
		unselectAllItemsAction = menu.addAction("Unselect all items")
		action = menu.exec_(self.mapToGlobal(event.pos()))
		if action == printExtensinsAction:
			extensions = self.getExtensions()
			print(extensions)
		elif action == selectAllItemsAction:
			for i in range(self.count()):
				self.item(i).setCheckState(Qt.Checked)
		elif action == unselectAllItemsAction:
			for i in range(self.count()):
				self.item(i).setCheckState(Qt.Unchecked)
			



if __name__ == '__main__':

	app = QApplication([])
 
	extensionListWidget = QWidget()
	extensionListWidget.setWindowTitle('')
	extensionListWidget.setMinimumSize(600, 400)
	
	layout = QVBoxLayout()
	extensionListWidget.setLayout(layout)
 
	extensionList = ExtensionList(extensionListWidget)
	layout.addWidget(extensionList)

	extensionListWidget.show()
	app.exec_()