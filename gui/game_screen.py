from PyQt5.QtWidgets import (
    QWidget, QVBoxLayout, QHBoxLayout, QPushButton, QLabel, QFileDialog, QMessageBox
)
from PyQt5.QtCore import Qt, QTimer
from board_widget import BoardWidget


class GameScreen(QWidget):
    def __init__(self, game, mainWindow):
        super().__init__()
        self.game = game
        self.mainWindow = mainWindow

        layout = QVBoxLayout()
        layout.setContentsMargins(0, 0, 0, 0)
        layout.setSpacing(5)

        # Board
        self.boardWidget = BoardWidget(game, mainWindow)
        layout.addWidget(self.boardWidget, alignment=Qt.AlignCenter)

        # Score label
        self.scoreLabel = QLabel()
        self.scoreLabel.setAlignment(Qt.AlignCenter)
        self.scoreLabel.setStyleSheet("font-size: 14px; font-weight: bold;")
        self.updateScore()
        layout.addWidget(self.scoreLabel)

        # Buttons
        btnLayout = QHBoxLayout()

        saveBtn = QPushButton("Зберегти")
        saveBtn.clicked.connect(self.saveGame)
        btnLayout.addWidget(saveBtn)

        loadBtn = QPushButton("Завантажити")
        loadBtn.clicked.connect(self.loadGame)
        btnLayout.addWidget(loadBtn)

        menuBtn = QPushButton("Меню")
        menuBtn.clicked.connect(self.stopAndGoMenu)
        btnLayout.addWidget(menuBtn)

        layout.addLayout(btnLayout)
        self.setLayout(layout)

        # Connect board updates to score refresh
        self.boardWidget.onUpdate = self.updateScore
        self.boardWidget.onMoveComplete = self.onHumanMove

        # Timer for AI moves
        self.aiTimer = QTimer()
        self.aiTimer.timeout.connect(self.doAIMove)
        
        # Start AI if first player is AI
        self.scheduleAIMoveIfNeeded()

    def scheduleAIMoveIfNeeded(self):
        """Schedule AI move if current player is AI"""
        if not self.game.isGameOver() and self.game.isAI(self.game.getCurrentPlayer()):
            self.aiTimer.start(500)  # 500ms delay between AI moves

    def onHumanMove(self):
        """Called after human makes a move"""
        if self.game.isGameOver():
            self.showGameOver()
        else:
            self.scheduleAIMoveIfNeeded()

    def doAIMove(self):
        """Execute one AI move"""
        self.aiTimer.stop()
        
        if self.game.isGameOver():
            self.showGameOver()
            return

        if self.game.isAI(self.game.getCurrentPlayer()):
            self.game.aiMove()
            self.boardWidget.update()
            self.updateScore()

            if self.game.isGameOver():
                self.showGameOver()
            else:
                self.scheduleAIMoveIfNeeded()

    def showGameOver(self):
        black = self.game.getScoreBlack()
        white = self.game.getScoreWhite()
        
        if black > white:
            result = "Перемогли Чорні! 🏆"
        elif white > black:
            result = "Перемогли Білі! 🏆"
        else:
            result = "Нічия!"
        
        msg = QMessageBox()
        msg.setWindowTitle("Гру завершено")
        msg.setText(f"{result}\n\nЧорні: {black}\nБілі: {white}")
        msg.exec_()
        self.mainWindow.backToMenu()

    def stopAndGoMenu(self):
        self.aiTimer.stop()
        self.mainWindow.backToMenu()

    def updateScore(self):
        black = self.game.getScoreBlack()
        white = self.game.getScoreWhite()
        player = self.game.getCurrentPlayer()
        turn = "Чорні" if player.name == "BLACK" else "Білі"
        self.scoreLabel.setText(f"Чорні: {black}  |  Білі: {white}  |  Хід: {turn}")

    def saveGame(self):
        filename, _ = QFileDialog.getSaveFileName(
            self, "Зберегти гру", "", "Reversi Save (*.rev)"
        )
        if filename:
            if not filename.endswith(".rev"):
                filename += ".rev"
            self.game.save(filename)

    def loadGame(self):
        filename, _ = QFileDialog.getOpenFileName(
            self, "Завантажити гру", "", "Reversi Save (*.rev)"
        )
        if filename:
            self.game.load(filename)
            self.boardWidget.update()
            self.updateScore()

