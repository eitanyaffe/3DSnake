# 3DSnake

This 3D version of the classic [snake game](https://en.wikipedia.org/wiki/Snake_(video_game_genre)) was a course exercise in Computer Graphics during my MSc in Computer Science at Tel Aviv University.

## Basic Design

The game was built using OpenGL in C++. It involves controlling a snake that moves around a 3D grid, consuming apples while avoiding collisions with itself and the walls.

## How to Run

1. **Prerequisites**: A C++ compiler and OpenGL. Tested on MacOS 13.3.1.

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
   ./3dsnake
   ```

## How to Play

- Use the arrow keys to control the direction of the snake.
- The objective is to consume apples on the grid to grow the snake.
- Avoid running into the walls or the snake's own body, as this will end the game.
