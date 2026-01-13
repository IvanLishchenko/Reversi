from PyQt5.QtWidgets import QWidget
from PyQt5.QtGui import QPainter, QColor
from PyQt5.QtCore import Qt
import reversi

CELL_SIZE = 80
HIGHLIGHT_COLOR = QColor(255, 255, 0, 120)


class BoardWidget(QWidget):
    def __init__(self, game, mainWindow):
        super().__init__()
        self.game = game
        self.mainWindow = mainWindow
        self.setFixedSize(640, 640)
        self.onUpdate = None  # Callback for score updates
        self.onMoveComplete = None  # Callback after human move

    def getValidMoves(self):
        moves = []
        board = self.game.getBoard()
        player = self.game.getCurrentPlayer()

        if self.game.isAI(player):
            return moves

        for x in range(8):
            for y in range(8):
                if board.isValidMove(x, y, player):
                    moves.append((x, y))
        return moves

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)

        board = self.game.getBoard()

        # Фон
        painter.setBrush(QColor(0, 130, 0))
        painter.drawRect(self.rect())

        # Сітка
        painter.setPen(Qt.black)
        for i in range(9):
            painter.drawLine(0, i * CELL_SIZE, 640, i * CELL_SIZE)
            painter.drawLine(i * CELL_SIZE, 0, i * CELL_SIZE, 640)

        # Підсвітка можливих ходів
        painter.setBrush(HIGHLIGHT_COLOR)
        painter.setPen(Qt.NoPen)
        for (x, y) in self.getValidMoves():
            painter.drawEllipse(
                x * CELL_SIZE + CELL_SIZE // 2 - 8,
                y * CELL_SIZE + CELL_SIZE // 2 - 8,
                16, 16
            )

        # Фішки
        for x in range(8):
            for y in range(8):
                cell = board.getCell(x, y)
                if cell != reversi.Cell.EMPTY:
                    painter.setBrush(
                        Qt.black if cell == reversi.Cell.BLACK else Qt.white
                    )
                    painter.drawEllipse(
                        x * CELL_SIZE + 5,
                        y * CELL_SIZE + 5,
                        CELL_SIZE - 10,
                        CELL_SIZE - 10
                    )

    def mousePressEvent(self, event):
        if self.game.isGameOver():
            return

        # Don't allow clicks if current player is AI
        if self.game.isAI(self.game.getCurrentPlayer()):
            return

        x = event.x() // CELL_SIZE
        y = event.y() // CELL_SIZE

        if self.game.makeMove(x, y):
            self.update()
            if self.onUpdate:
                self.onUpdate()
            # Notify parent to schedule AI move if needed
            if self.onMoveComplete:
                self.onMoveComplete()
