# Board Games Collection 2025

**CS213: Object-Oriented Programming - Assignment 3**  
**Cairo University - Faculty of Computers and Artificial Intelligence**

## 📋 Project Overview

A comprehensive collection of 14 board game variants implemented in C++ using Object-Oriented Programming principles. This project demonstrates advanced OOP concepts including inheritance, polymorphism, templates, and the Open/Closed Principle.

## 🎮 Available Games

### Core Games (Individual)
1. **Standard Tic-Tac-Toe** - Classic 3x3 grid game
2. **SUS** - Form S-U-S sequences on a 3x3 grid
3. **Four-in-a-Row** - Connect 4 on a 6x7 grid
4. **5x5 Tic-Tac-Toe** - Score with most 3-in-a-rows
5. **Word Tic-Tac-Toe** - Form valid 3-letter words
6. **Misère Tic-Tac-Toe** - Avoid getting 3-in-a-row
7. **Diamond Tic-Tac-Toe** - Complete 2 lines simultaneously
8. **4x4 Tic-Tac-Toe** - Align 3 tokens on 4x4 grid
9. **Pyramid Tic-Tac-Toe** - Play on pyramid-shaped board

### Team Games (Group)
10. **Numerical Tic-Tac-Toe** - Sum to 15 with numbers
11. **Obstacles Tic-Tac-Toe** - 4-in-a-row with random obstacles
12. **Infinity Tic-Tac-Toe** - Marks disappear after 3 moves

### Bonus Games
13. **Ultimate Tic-Tac-Toe** - Meta-game with 9 sub-boards
14. **Memory Tic-Tac-Toe** - Hidden marks challenge

## 🏗️ Architecture

### Framework Classes

The project uses a generic, template-based framework:

- **`Board<T>`** - Abstract base class for all game boards
- **`Move<T>`** - Represents a single move in any game
- **`Player<T>`** - Defines human and computer players
- **`UI<T>`** - Abstract user interface handler
- **`GameManager<T>`** - Controls game flow and turns

### Design Principles Applied

✅ **Abstraction** - Abstract base classes for Board and UI  
✅ **Polymorphism** - Virtual methods for game-specific behavior  
✅ **Inheritance** - Game-specific classes extend framework  
✅ **Encapsulation** - Private/protected members with public interfaces  
✅ **Open/Closed Principle** - Framework closed for modification, open for extension  
✅ **Templates** - Generic implementation works with any symbol type

## 🚀 Getting Started

### Prerequisites

- C++ Compiler (GCC 7.0+ or MSVC 2017+)
- Windows OS (for Windows.h API features)
- Doxygen (for documentation generation)

### Building the Project

```bash
# Clone the repository
git clone https://github.com/yourusername/board-games-2025.git
cd board-games-2025

# Compile
g++ -std=c++17 "Game ASS 3.cpp" -o BoardGames.exe

# Run
./BoardGames.exe
```

### Generating Documentation

```bash
# Install Doxygen (if not installed)
# Download from: https://www.doxygen.nl/download.html

# Generate documentation
doxygen Doxyfile

# Open documentation
cd docs/html
start index.html
```

## 📁 Project Structure

```
board-games-2025/
│
├── BoardGame_Classes.h           # Core framework classes
├── Game ASS 3.cpp                # Main application with menu
│
├── Game0_StandardTicTacToe_Board.h
├── Game1_SUS_Board.h
├── Game2_FourInARow_Board.h
├── Game3_5x5TicTacToe_Board.h
├── Game4_WordTicTacToe_Board.h
├── Game5_MisereTicTacToe_Board.h
├── Game6_DiamondTicTacToe_Board.h
├── Game7_4x4TicTacToe_Board.h
├── Game8_PyramidTicTacToe_Board.h
├── Game9_NumericalTicTacToe_Board.h
├── Game10_ObstaclesTicTacToe_Board.h
├── Game11_InfinityTicTacToe_Board.h
├── Game12_UltimateTicTacToe_Board.h
├── Game13_MemoryTicTacToe_Board.h
│
├── dic.txt                       # Dictionary for Word Tic-Tac-Toe
├── Doxyfile                      # Doxygen configuration
├── README.md                     # This file
└── docs/                         # Generated documentation
    └── html/
        └── index.html
```

## 🎯 Features

### User Experience
- ✨ Colorful console interface using Windows API
- 🔊 Sound effects for moves and game events
- 🎨 Professional menu system
- ⚡ Input validation and error handling
- 👤 Custom player names

### Gameplay Options
- 👥 Human vs Human
- 🤖 Human vs Computer (Random)
- 🧠 Human vs Computer (Smart AI - Bonus)

### Technical Features
- Template-based generic framework
- Smart computer players using minimax/backtracking
- Comprehensive error handling
- Input validation for all games
- Extensible architecture

## 👥 Team Members

**Section: [Your Section]**

| ID | Name | Email | Games Assigned |
|----|------|-------|----------------|
| [ID1] | [Name1] | [Email1] | Games 1, 2 |
| [ID2] | [Name2] | [Email2] | Games 3, 4 |
| [ID3] | [Name3] | [Email3] | Games 5, 6 |
| [ID4] | [Name4] | [Email4] | Games 7, 8 |
| **Team** | **All** | - | Games 9-13 |

## 🏆 Work Breakdown

### Individual Responsibilities
- **Player 1** (Smallest ID): Games 1, 2
- **Player 2**: Games 3, 4
- **Player 3**: Games 5, 6
- **Player 4** (Largest ID): Games 7, 8

### Team Responsibilities
- **All Members**: Games 9, 10, 11 (+ 12, 13 for bonus)
- **Integration**: Combining all games into single application
- **Documentation**: Doxygen comments and HTML generation
- **Code Review**: Each member reviews another's code

## 📊 Code Quality

### Standards Applied
- ✅ Consistent naming conventions
- ✅ Comprehensive error handling
- ✅ Input validation for all user inputs
- ✅ Clear and informative error messages
- ✅ Code comments and documentation
- ✅ DRY principle (Don't Repeat Yourself)
- ✅ SOLID principles

### Code Review Checklist
- [ ] No magic numbers
- [ ] Proper memory management
- [ ] No code duplication
- [ ] Consistent formatting
- [ ] Meaningful variable names
- [ ] Error handling present
- [ ] Comments for complex logic

## 🎓 Learning Outcomes

This project demonstrates proficiency in:

1. **OOP Concepts**
   - Inheritance hierarchies
   - Virtual functions and polymorphism
   - Abstract classes and interfaces
   - Template programming

2. **C++ Features**
   - STL containers (vector, set, queue)
   - Smart pointers (optional)
   - File I/O operations
   - Exception handling

3. **Software Engineering**
   - Design patterns
   - Code reusability
   - Documentation practices
   - Version control (Git)
   - Code review process

## 📝 Academic Integrity

This project was completed in accordance with Cairo University's academic integrity policy. All code is original work by the team members, with proper attribution for any external resources used.

## 📄 License

This project is submitted as coursework for CS213 at Cairo University FCAI.  
**Academic Year**: 2025-2026, First Semester

## 🔗 Resources

- [Doxygen Download](https://www.doxygen.nl/download.html)
- [C++ Reference](https://en.cppreference.com/)
- [Code Review Checklist](https://github.com/mgreiler/code-review-checklist)
- [12 Best Code Review Tools](https://kinsta.com/blog/code-review-tools/)

## 📞 Contact

For questions or issues, please contact the course instructor:  
**Dr. Mohammad El-Ramly**  
Cairo University, Faculty of Computers and Artificial Intelligence

---

**Version**: 5.0  
**Last Updated**: December 2025  
**Course**: CS213 - Object Oriented Programming