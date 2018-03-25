import sys, os , cv2
from util import loadPixmapWithExcludedRegion
from PyQt5.QtWidgets import QApplication, QWidget, QLabel, QShortcut, QLineEdit, QButtonGroup, QCheckBox, QPushButton
from PyQt5.QtGui import QPainter, QColor, QPixmap, QBrush, QKeySequence
from PyQt5.QtCore import Qt
 
class Paint(QWidget):
 
	def __init__(self, imageName):
		super().__init__()
		self.title = 'PyQt5 Paint: {}'.format(imageName)
		self.imageName = imageName
		self.left = 100
		self.top = 100
		self.width = 600
		self.height = 500
		self.shortcut = QShortcut(QKeySequence("Ctrl+Z"), self)
		self.shortcut.activated.connect(self.backEdit)
		self.initUI()
 
	def initUI(self):
		self.setWindowTitle(self.title)
		self.setGeometry(self.left, self.top, self.width, self.height)
		
		#main image window/widget definition
		self.label = QLabel(self)
		self.label.mouseDrowingState = False
		self.label.setMouseTracking(True)
		self.label.lastPos = None
		
		self.label.orginalImage = QPixmap(self.imageName)
		#self.label.orginalImage = loadPixmapWithExcludedRegion(self.imageName)
		self.label.orginalImageSize = self.label.orginalImage.size()
		self.label.orginalImage = self.label.orginalImage.scaledToWidth(self.width-100)
		self.label.orginalImage = self.label.orginalImage.scaledToHeight(self.height)
		
		"""self.label.selctedRegionImage = None
		head, tail = os.path.split(self.imageName)
		if os.path.isfile("{}/ExcludedRegion_{}".format(head,tail)):
			self.label.selctedRegionImage = QPixmap("{}/ExcludedRegion_{}".format(head,tail))
			self.label.selctedRegionImage = self.label.selctedRegionImage.scaledToWidth(self.width-100)
			self.label.selctedRegionImage = self.label.selctedRegionImage.scaledToHeight(self.height)
		else:
			self.label.selctedRegionImage = QPixmap(self.label.orginalImage.size())"""
		self.label.selctedRegionImage = None
		head, tail = os.path.split(self.imageName)
		if os.path.isfile("{}/ExcludedRegion_{}".format(head,tail)):
			self.label.selctedRegionImage = QPixmap("{}/ExcludedRegion_{}".format(head,tail))
		else:
			self.label.selctedRegionImage = QPixmap(self.label.orginalImage.size())
		self.label.selctedRegionImage = self.label.selctedRegionImage.scaledToWidth(self.width-100)
		self.label.selctedRegionImage = self.label.selctedRegionImage.scaledToHeight(self.height)

		#self.label.orginalImage = cv2.bitwise_and(self.label.orginalImage, self.label.selctedRegionImage, mask = self.label.selctedRegionImage)
		
		self.label.setPixmap(loadPixmapWithExcludedRegion(self.imageName))
		self.label.setPixmap(self.label.pixmap().scaledToWidth(self.width-100))
		self.label.setPixmap(self.label.pixmap().scaledToHeight(self.height))
		
		#mouse event interception
		self.label.mouseMoveEvent = self.mouseMoveEventOnLabel
		self.label.mousePressEvent = self.mousePressEventOnLabel
		self.label.mouseReleaseEvent = self.mouseReleaseEventOnLabel
		
		#editing history
		self.label.editHistory = []
		self.label.editHistory.append(QPixmap(self.label.pixmap()))
		
		#brush size line edit control
		brushSizeTextEditLabel = QLabel(self)
		brushSizeTextEditLabel.setText("brush size")
		brushSizeTextEditLabel.move(self.width-90,85)
		
		self.brushSizeTextEdit = QLineEdit(self)
		self.brushSizeTextEdit.resize(60,20)
		self.brushSizeTextEdit.move(self.width-90,100)
		self.label.brushSize = 10
		self.brushSizeTextEdit.setText(str(self.label.brushSize))
		
		self.brushSizeTextEdit.textChanged.connect(self.textChangedBrushSizeLineEdit)
		
		#print mode checkbox (print/erase)
		printModeButtonGroup = QButtonGroup(self)
		printModeButtonGroup.setExclusive(True)
		
		self.printCheckBoxList = QCheckBox("PRINT",self)
		printModeButtonGroup.addButton(self.printCheckBoxList,1)
		self.printCheckBoxList.move(self.width-90,200)
		self.printCheckBoxList.setCheckState(Qt.Checked)
		
		self.eraseCheckBoxList = QCheckBox("ERASE",self)
		printModeButtonGroup.addButton(self.eraseCheckBoxList,2)
		self.eraseCheckBoxList.move(self.width-90,230)
		self.eraseCheckBoxList.setCheckState(Qt.Unchecked)
		
		#clear all button
		self.clearAllButton = QPushButton("CLEAR ALL", self)
		self.clearAllButton.mousePressEvent = self.clearAll
		self.clearAllButton.resize(60,20)
		self.clearAllButton.move(self.width-90,450)
		
		#save selected region button
		self.saveSelectedRegionButton = QPushButton("SAVE\nEXCLUDED\nREGIN", self)
		self.saveSelectedRegionButton.mousePressEvent = self.saveSelectedRegion
		self.saveSelectedRegionButton.resize(60,20)
		self.saveSelectedRegionButton.move(self.width-90,400)
		
		self.show()
		
	def drow(self, pos, state, drawLine):
		painter = QPainter(self.label.pixmap())
		if state:
			painter.setBrush(QBrush(QColor(255,0,0), Qt.SolidPattern))
			painter.setPen(QColor(255,0,0))
		else:
			brush = QBrush()
			brush.setTexture(self.label.orginalImage)
			painter.setBrush(brush)
			painter.setPen(QColor(Qt.transparent))
			painter.pen().setWidth(self.label.brushSize)
		painter.drawEllipse(pos.x()-self.label.brushSize/2, pos.y()-self.label.brushSize/2,self.label.brushSize,self.label.brushSize)
		if drawLine:
			painter.drawLine(pos, self.label.lastPos)
		
		painter2 = QPainter(self.label.selctedRegionImage)
		if state:
			painter2.setBrush(QBrush(QColor(255,0,0), Qt.SolidPattern))
			painter2.setPen(QColor(255,0,0))
		else:
			brush = QBrush()
			brush.setTexture(QPixmap(self.label.orginalImage.size()))
			painter2.setBrush(brush)
			painter2.setPen(QColor(Qt.transparent))
			painter2.pen().setWidth(self.label.brushSize)
		painter2.drawEllipse(pos.x()-self.label.brushSize/2, pos.y()-self.label.brushSize/2,self.label.brushSize,self.label.brushSize)
		if drawLine:
			painter2.drawLine(pos, self.label.lastPos)
		self.label.update()
		
		
	def mouseMoveEventOnLabel(self, event):
		print(event.pos())
		if self.label.mouseDrowingState:
			self.drow(event.pos(), self.printCheckBoxList.checkState(), True)
			self.label.lastPos = event.pos()
		
	def mousePressEventOnLabel(self, event):
		self.label.mouseDrowingState = not self.label.mouseDrowingState
		self.drow(event.pos(), self.printCheckBoxList.checkState(), False)
		self.label.lastPos = event.pos()
		
	def mouseReleaseEventOnLabel(self, event):
		self.label.mouseDrowingState = not self.label.mouseDrowingState
		self.label.editHistory.append(QPixmap(self.label.pixmap()))
		if len(self.label.editHistory) > 10:
			self.label.editHistory.pop(0)
		#print("Release", len(self.label.editHistory))
		
	def backEdit(self):
		if len(self.label.editHistory) > 1:
			self.label.setPixmap(self.label.editHistory[-2])
			self.label.editHistory.pop(-1)
			self.label.update()
			#print("Ctrl+Z", len(self.label.editHistory))
			
	def textChangedBrushSizeLineEdit(self,event):
		#print("inputTextEdit")
		try:
			self.label.brushSize = int(self.brushSizeTextEdit.text())
		except:
			self.brushSizeTextEdit.setText("")
	
	def clearAll(self, event = None):
		self.label.setPixmap(self.label.orginalImage)
		self.label.selctedRegionImage  = QPixmap()
		self.label.editHistory.append(QPixmap(self.label.pixmap()))
		if len(self.label.editHistory) > 10:
			self.label.editHistory.pop(0)
		
	def saveSelectedRegion(self, event = None):
		head, tail = os.path.split(self.imageName)
		self.label.selctedRegionImage.\
			scaled(self.label.orginalImageSize, Qt.IgnoreAspectRatio, Qt.FastTransformation).\
			save("{}/ExcludedRegion_{}".format(head,tail))
		print(os.path.split(self.imageName))
		
		
 
if __name__ == '__main__':
	app = QApplication(sys.argv)
	ex = Paint('C:\\Users\\Wojciech\\Dropbox\\analiza_obrazu\\GUI\\pare_zdjec\\NaT_1.lif_Series036Snapshot1_ch00.jpg')
	sys.exit(app.exec_())