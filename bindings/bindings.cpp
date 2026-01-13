#include <pybind11/pybind11.h>
#include "../src/game.h"

namespace py = pybind11;

PYBIND11_MODULE(reversi, m) {
    py::enum_<Cell>(m, "Cell")
        .value("EMPTY", Cell::EMPTY)
        .value("BLACK", Cell::BLACK)
        .value("WHITE", Cell::WHITE);

    py::class_<Board>(m, "Board")
        .def(py::init<>())
        .def("getCell", &Board::getCell)
        .def("isValidMove", &Board::isValidMove);

        py::class_<Game>(m, "Game")
        .def(py::init<int, int, int, int>(), py::arg("mode"), py::arg("difficulty"), py::arg("humanColor") = 1, py::arg("difficulty2") = -1)
        .def("getBoard", &Game::getBoard, py::return_value_policy::reference)
        .def("getCurrentPlayer", &Game::getCurrentPlayer)
        .def("makeMove", &Game::makeMove)
        .def("aiMove", &Game::aiMove)
        .def("isGameOver", &Game::isGameOver)
        .def("isAI", &Game::isAI)
        .def("getScoreBlack", &Game::getScoreBlack)
        .def("getScoreWhite", &Game::getScoreWhite)
        .def("save", &Game::save)
        .def("load", &Game::load);
    
}
