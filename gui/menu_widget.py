from PyQt5.QtWidgets import (
    QWidget, QVBoxLayout, QPushButton,
    QComboBox, QLabel
)
from PyQt5.QtCore import Qt


class MenuWidget(QWidget):
    def __init__(self, mainWindow):
        super().__init__()
        self.mainWindow = mainWindow

        layout = QVBoxLayout()
        layout.setAlignment(Qt.AlignCenter)

        title = QLabel("REVERSI")
        title.setStyleSheet("font-size: 28px; font-weight: bold;")
        title.setAlignment(Qt.AlignCenter)

        self.modeBox = QComboBox()
        self.modeBox.addItems([
            "Людина – Людина",
            "Людина – Комп'ютер",
            "Комп'ютер – Комп'ютер"
        ])
        self.modeBox.currentIndexChanged.connect(self.onModeChanged)

        self.colorLabel = QLabel("Ваш колір:")
        self.colorBox = QComboBox()
        self.colorBox.addItems([
            "Чорні (перший хід)",
            "Білі (другий хід)"
        ])

        # Single AI difficulty (for Human vs AI)
        self.aiLabel = QLabel("Складність AI:")
        self.aiBox = QComboBox()
        self.aiBox.addItems(["Random", "Greedy", "Minimax"])

        # Two AI difficulties (for AI vs AI)
        self.aiBlackLabel = QLabel("AI Чорних:")
        self.aiBlackBox = QComboBox()
        self.aiBlackBox.addItems(["Random", "Greedy", "Minimax"])

        self.aiWhiteLabel = QLabel("AI Білих:")
        self.aiWhiteBox = QComboBox()
        self.aiWhiteBox.addItems(["Random", "Greedy", "Minimax"])

        startBtn = QPushButton("Почати гру")
        startBtn.clicked.connect(self.startGame)

        exitBtn = QPushButton("Вихід")
        exitBtn.clicked.connect(self.mainWindow.close)

        layout.addWidget(title)
        layout.addSpacing(20)
        layout.addWidget(QLabel("Режим гри:"))
        layout.addWidget(self.modeBox)
        layout.addWidget(self.colorLabel)
        layout.addWidget(self.colorBox)
        layout.addWidget(self.aiLabel)
        layout.addWidget(self.aiBox)
        layout.addWidget(self.aiBlackLabel)
        layout.addWidget(self.aiBlackBox)
        layout.addWidget(self.aiWhiteLabel)
        layout.addWidget(self.aiWhiteBox)
        layout.addSpacing(20)
        layout.addWidget(startBtn)
        layout.addWidget(exitBtn)

        # Initial state
        self.onModeChanged()

        self.setLayout(layout)

    def onModeChanged(self):
        mode = self.modeBox.currentIndex()
        isHumanVsHuman = mode == 0
        isHumanVsAI = mode == 1
        isAIVsAI = mode == 2
        
        # Color choice only for Human vs AI
        self.colorLabel.setVisible(isHumanVsAI)
        self.colorBox.setVisible(isHumanVsAI)
        
        # Single AI difficulty for Human vs AI
        self.aiLabel.setVisible(isHumanVsAI)
        self.aiBox.setVisible(isHumanVsAI)
        
        # Two AI difficulties for AI vs AI
        self.aiBlackLabel.setVisible(isAIVsAI)
        self.aiBlackBox.setVisible(isAIVsAI)
        self.aiWhiteLabel.setVisible(isAIVsAI)
        self.aiWhiteBox.setVisible(isAIVsAI)

    def startGame(self):
        mode = self.modeBox.currentIndex()
        humanColor = self.colorBox.currentIndex() + 1
        
        if mode == 2:  # AI vs AI
            diffBlack = self.aiBlackBox.currentIndex()
            diffWhite = self.aiWhiteBox.currentIndex()
            self.mainWindow.startGame(mode, diffBlack, humanColor, diffWhite)
        else:
            difficulty = self.aiBox.currentIndex()
            self.mainWindow.startGame(mode, difficulty, humanColor)
