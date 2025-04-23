# 3DSnake

This was an class exercise during my MSc in Computer Science at Tel Aviv University.

## Basic Design

The 3DSnake game is built using OpenGL. The game is implemented in C++, and it involves controlling a snake that moves around a 3D grid, consuming items to grow longer while avoiding collisions with itself and the boundaries.

## How to Run

1. **Prerequisites**: Ensure you have a C++ compiler and OpenGL installed on your system.

2. **Clone the Repository**:
   ```bash
   git clone https://github.com/yourusername/3DSnake.git
   cd 3DSnake
   ```

3. **Build the Project**: Use the provided Makefile to compile the project.
   ```bash
   make
   ```

4. **Execute the Game**: Run the compiled binary from the terminal.
   ```bash
   ./3DSnake
   ```

5. **Clean Up**: If you need to clean the build files, you can run:
   ```bash
   make clean
   ```

## How to Play

- Use the arrow keys to control the direction of the snake.
- The objective is to consume items that appear randomly on the grid to grow the snake.
- Avoid running into the walls or the snake's own body, as this will end the game.
