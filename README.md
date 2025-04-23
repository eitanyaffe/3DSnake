# 3DSnake

This project is an exercise in OpenGL, designed to explore and demonstrate basic graphics programming concepts.

This project was originally developed as an exercise during my MSc in Computer Science at Tel Aviv University in the early 2000s.

## Basic Design

The 3DSnake game is built using OpenGL to render 3D graphics. The game logic is implemented in C++, and it involves controlling a snake that moves around a 3D grid, consuming items to grow longer while avoiding collisions with itself and the boundaries.

## How to Run

1. **Prerequisites**: Ensure you have a C++ compiler and OpenGL installed on your system.
2. **Build the Project**: Use the provided Makefile or CMake configuration to compile the project.
3. **Execute the Game**: Run the compiled binary from the terminal.

## How to Play

- Use the arrow keys to control the direction of the snake.
- The objective is to consume items that appear randomly on the grid to grow the snake.
- Avoid running into the walls or the snake's own body, as this will end the game.