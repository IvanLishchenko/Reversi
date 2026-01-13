from PyQt5.QtWidgets import QMainWindow, QStackedWidget
from menu_widget import MenuWidget
from game_screen import GameScreen
import reversi


class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Reversi")
        self.setFixedSize(640, 720)

        self.stack = QStackedWidget()
        self.setCentralWidget(self.stack)

        self.menu = MenuWidget(self)
        self.stack.addWidget(self.menu)

    def startGame(self, mode, difficulty, humanColor=1, difficulty2=-1):
        # mode: 0=Human-Human, 1=Human-AI, 2=AI-AI
        # difficulty: 0=Random, 1=Greedy, 2=Minimax (Black's AI in AI vs AI)
        # humanColor: 1=Black (first), 2=White (second)
        # difficulty2: White's AI difficulty in AI vs AI mode
        self.game = reversi.Game(mode, difficulty, humanColor, difficulty2)
        
        self.gameScreen = GameScreen(self.game, self)
        self.stack.addWidget(self.gameScreen)
        self.stack.setCurrentWidget(self.gameScreen)

    def backToMenu(self):
        self.stack.setCurrentWidget(self.menu)
