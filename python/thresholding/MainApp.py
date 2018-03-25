#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys, os
from util import searchForImages
from ExtensionList import ExtensionList
from ImageFileList import ImageFileList
from ImageProcesingEngine import procesImages
from PyQt5.QtWidgets import QApplication, QWidget, QPushButton, QFileDialog, QVBoxLayout, QListWidgetItem, QLineEdit, QLabel

class MainApp(QWidget):

	def __init__(self):
                
		super().__init__()
		self.title = 'marked surface counter'
		self.left = 50
		self.top = 50
		self.width = 800
		self.height = 600
		self.thresholdParameter = None
		self.imagesList = ImageFileList(self)
		self.extensionList = ExtensionList(self)
		self.initUI()
		
	def initUI(self):
		self.setWindowTitle(self.title)
		self.setGeometry(self.left, self.top, self.width, self.height)
		
		#layout = QVBoxLayout()
		#self.setLayout(layout)
		
		choseFileButton = QPushButton("chose files", self)
		choseFileButton.resize(100,20)
		choseFileButton.move(10, 10)
		choseFileButton.clicked.connect(self.choseFileAction)
		#layout.addWidget(choseFileButton)
		
		choseDirectoryButton = QPushButton("chose directory", self)
		choseDirectoryButton.resize(100, 20)
		choseDirectoryButton.move(10, 40)
		choseDirectoryButton.clicked.connect(self.choseDirectoryAction)
		#layout.addWidget(choseDirectoryButton)
		
		procesImagesButton = QPushButton("proces images", self)
		procesImagesButton.resize(100, 20)
		procesImagesButton.move(10, 70)
		procesImagesButton.clicked.connect(self.procesImagesAction)
		#layout.addWidget(procesImagesButton)
		
		self.imagesList.setGeometry(120, 10, 500, 500)
		#layout.addWidget(self.imagesList)

		self.extensionList.setGeometry(10, 100, 100, 350)
		#layout.addWidget(self.extensionList)
		
		self.thresholdParameterLineEditLabel = QLabel(self)
		self.thresholdParameterLineEditLabel.setText("threshold parameter")
		self.thresholdParameterLineEditLabel.resize(120,20)
		self.thresholdParameterLineEditLabel.move(630,20)
		
		self.thresholdParameterLineEdit = QLineEdit(self)
		#layout.addWidget(self.thresholdParameterLineEdit)
		self.thresholdParameterLineEdit.resize(40,20)
		self.thresholdParameterLineEdit.move(630,40)
		self.thresholdParameter = 60
		self.thresholdParameterLineEdit.setText(str(self.thresholdParameter))
		self.thresholdParameterLineEdit.textChanged.connect(self.textChangedThresholdParameterLineEdit)
		
		self.show()
		
	def textChangedThresholdParameterLineEdit(self,event):
		try:
			self.thresholdParameter = int(self.thresholdParameterLineEdit.text())
		except:
			self.thresholdParameter.setText("")
		
	def choseFileAction(self):   
		options = QFileDialog.Options()
		#options |= QFileDialog.DontUseNativeDialog
		files, _ = QFileDialog.getOpenFileNames(self,"Select files")
		if files:
			self.imagesList.addFilesToList(files)
			
	def choseDirectoryAction(self):   
		options = QFileDialog.Options()
		options |= QFileDialog.DontUseNativeDialog
		directoryPath = str(QFileDialog.getExistingDirectory(self, "Select Directory"))
		files = searchForImages(directoryPath, self.extensionList.getExtensions())
		if files:
			self.imagesList.addFilesToList(files)
			
	def procesImagesAction(self):
		imageFiles = []
		for i in range(self.imagesList.count()):
			imageFiles.append(self.imagesList.item(i).text())
		procesImages(imageFiles, self.thresholdParameter)
		

		
if __name__ == '__main__':
	app = QApplication(sys.argv)
	ex = MainApp()
	sys.exit(app.exec_())
