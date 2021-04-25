import json, sys
from PyQt5 import QtWidgets, QtGui, uic
import os.path

class StringFile:
    def __init__(self):
        self._entries = dict()
        pass
    
    def load(self, jsonFileName):
        with open(jsonFileName, "rb") as fp:
            self._entries = json.loads(fp.read())
 
    def save(self, jsonFileName):
        with open(jsonFileName, "wb") as fp:
            fp.write(json.dumps(self._entries, indent=4).encode("utf8"))

    def getIdList(self):
        return list(self._entries)

    def getEntry(self, id):
        return self._entries[id]

    def newEntry(self):
        id = 1
        while str(id) in self._entries:
            id = id + 1
        self._entries[str(id)] = dict(en = "", de = "")
        return str(id)

    def setEntry(self, id, entry):
        self._entries[id] = entry

    def renameEntry(self, oldId, newId):
        entry = self._entries[oldId]
        del self._entries[oldId]
        self._entries[newId] = entry

class Ui(QtWidgets.QMainWindow):
    def __init__(self):
        super(Ui, self).__init__() # Call the inherited classes __init__ method
        self._appName = "Translation Editor"
        self._selectedId = None
        self._filename = None
        self._strings = StringFile()
        uic.loadUi('text_editor.ui', self) # Load the .ui file
        self.show() # Show the GUI
        self.idList.clear()

        self.actionAbout.triggered.connect(self.displayAboutMessage)
        self.actionQuit.triggered.connect(self.quit)
        self.actionOpen.triggered.connect(self.openFile)
        self.actionSave_As.triggered.connect(self.saveAs)
        self.actionSave.triggered.connect(self.save)
        self.actionNew.triggered.connect(self.new)
        self.addIdButton.clicked.connect(self.addNewEntry)
        self.englishTextEdit.textChanged.connect(self.contentChanged)
        self.germanTextEdit.textChanged.connect(self.contentChanged)
        self.idList.currentTextChanged.connect(self.idSelected)
        self.renameIdButton.clicked.connect(self.renameIdClicked)

        self.updateUi()

    def renameIdClicked(self):
        newId = self.idLineEdit.text()
        if len(newId) > 0:
            self._strings.renameEntry(self._selectedId, newId)
            self._selectedId = newId
            self.updateUi()

    def idSelected(self, id):
        if len(id) > 0:
            self._selectedId = id
            self.updateUi()

    def addNewEntry(self):
        id = self._strings.newEntry()
        self._selectedId = id
        self.updateUi()

    def quit(self):
        self.close()

    def openFile(self):
        filename = QtWidgets.QFileDialog.getOpenFileName(self,'Open Translation File','./','JSON Files(*.json)')
        if len(filename[0]) > 0:
            self._filename = filename[0]
            self.load(self._filename)
            self.updateUi()

    def saveAs(self):
        filename = QtWidgets.QFileDialog.getSaveFileName(self,'Save Translation File','./','JSON Files(*.json)')
        if len(filename[0]) > 0:
            self._strings.save(filename[0])
            self._filename = filename[0]
            self.updateUi()
    
    def save(self):
        if len(self._filename) > 0:
            self._strings.save(self._filename)

    def new(self):
        self._strings = StringFile()
        self._selectedId = None
        self._filename = None
        self.updateUi()

    def displayAboutMessage(self):
        QtWidgets.QMessageBox.about(self, self._appName, "{} - An editor for translations for TandyGame".format(self._appName))
        pass

    def load(self, jsonFileName):
        
        self._strings.load(jsonFileName)
        try:
            self._selectedId = self._strings.getIdList()[0]
        except KeyError:
            self._selectedId = None

        self.updateUi()
    
    def contentChanged(self):
        if self._selectedId is not None:
            entry = dict(en = self.englishTextEdit.toPlainText(), de = self.germanTextEdit.toPlainText())
            self._strings.setEntry(self._selectedId, entry)

    def updateUi(self):
        if self._filename is None:
            self.actionSave.setEnabled(False)
            self.setWindowTitle(self._appName)
        else:
            self.setWindowTitle("{} - {}".format(os.path.basename(self._filename), self._appName))
            self.actionSave.setEnabled(True)

        if self._selectedId is None:
            self.idLineEdit.clear()
            self.englishTextEdit.clear()
            self.germanTextEdit.clear()
            self.idLineEdit.setEnabled(False)
            self.englishTextEdit.setEnabled(False)
            self.germanTextEdit.setEnabled(False)
        else:
            entry = self._strings.getEntry(self._selectedId)
            self.idLineEdit.setText(self._selectedId)
            self.englishTextEdit.setPlainText(entry["en"])
            self.germanTextEdit.setPlainText(entry["de"])
            self.idLineEdit.setEnabled(True)
            self.englishTextEdit.setEnabled(True)
            self.germanTextEdit.setEnabled(True)

        self.idList.clear()
        self.idList.addItems(self._strings.getIdList())
        
    


if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv) # Create an instance of QtWidgets.QApplication
    app.setWindowIcon(QtGui.QIcon('text_editor_icon.png'))
    window = Ui() # Create an instance of our class
    app.exec_() # Start the application
