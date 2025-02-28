#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MIN_SIZE 5 // Minimum size for the maze
#define MAX_SIZE 29 // Maximum size for the maze

// Directions for moving up, down, left, right
int dirX[] = {-1, 1, 0, 0};
int dirY[] = {0, 0, -1, 1};

// Function to initialize a random maze
void generateMaze(char **maze, int rows, int cols) {
    // Randomly fill the maze with walls (#) and paths (.)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            maze[i][j] = (rand() % 2) == 0 ? '#' : '.'; // Random wall or open space
        }
    }

    // Ensure the start (P) and exit (E) are open paths
    maze[0][0] = '.'; // Start position (top-left)
    maze[rows - 1][cols - 1] = 'E'; // Exit position (bottom-right)
}

// Function to check if a position is valid (within bounds and open space)
int isValid(int x, int y, char **maze, int visited[][100], int rows, int cols) {
    return x >= 0 && x < rows && y >= 0 && y < cols && maze[x][y] != '#' && !visited[x][y];
}

// Depth First Search to check if there is a path from start to exit
int dfs(int x, int y, char **maze, int visited[][100], int rows, int cols) {
    // Base condition: if we reach the exit
    if (x == rows - 1 && y == cols - 1) {
        return 1;
    }

    visited[x][y] = 1; // Mark this cell as visited

    // Try moving in all 4 directions
    for (int i = 0; i < 4; i++) {
        int newX = x + dirX[i];
        int newY = y + dirY[i];
        if (isValid(newX, newY, maze, visited, rows, cols) && dfs(newX, newY, maze, visited, rows, cols)) {
            return 1; // If a valid path is found, return true
        }
    }

    return 0; // No valid path found
}

// Function to check if the maze is solvable
int isMazeSolvable(char **maze, int rows, int cols) {
    int visited[100][100] = {0}; // Visited array to keep track of visited cells
    return dfs(0, 0, maze, visited, rows, cols); // Start DFS from the top-left corner (start position)
}

// Function to display the maze
void displayMaze(char **maze, int playerX, int playerY, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (i == playerX && j == playerY)
                printf("P "); // Display player (P)
            else if (maze[i][j] == 'E') 
                printf("E "); // Display exit only if not at player's position
            else
                printf("%c ", maze[i][j]); // Display maze element
        }
        printf("\n");
    }
}

// Function to move the player
int movePlayer(char **maze, int *playerX, int *playerY, char direction, int rows, int cols) {
    int newX = *playerX, newY = *playerY;

    // Move based on the direction input
    if (direction == 'u') // Move up
        newX--;
    else if (direction == 'd') // Move down
        newX++;
    else if (direction == 'l') // Move left
        newY--;
    else if (direction == 'r') // Move right
        newY++;

    // Check if the new position is within bounds and not a wall
    if (newX >= 0 && newX < rows && newY >= 0 && newY < cols && maze[newX][newY] != '#') {
        *playerX = newX;
        *playerY = newY;
        return 1; // Move successful
    }
    return 0; // Invalid move (wall or out of bounds)
}

int main() {
    int rows, cols;

    // Prompt the user for the size of the maze
    printf("Enter the number of rows for the maze (minimum %d and maximum %d): ", MIN_SIZE,MAX_SIZE);
    scanf("%d", &rows);
    printf("Enter the number of columns for the maze (minimum %d): ", MIN_SIZE);
    scanf("%d", &cols);

    // Ensure the maze size is not smaller than the minimum
    if (rows < MIN_SIZE || cols < MIN_SIZE) {
        printf("Maze size is too small.\n Setting to minimum size of %d x %d.\n", MIN_SIZE, MIN_SIZE);
        rows = MIN_SIZE;
        cols = MIN_SIZE;
    }

    if (rows > MAX_SIZE || cols > MAX_SIZE) {
        printf("Maze size is to large.\n Setting to maximum size od %d x %d.\n", MAX_SIZE, MAX_SIZE);
        rows = MAX_SIZE;
        cols = MAX_SIZE;

    }

    // Dynamically allocate memory for the maze
    char **maze = (char **)malloc(rows * sizeof(char *));
    for (int i = 0; i < rows; i++) {
        maze[i] = (char *)malloc(cols * sizeof(char));
    }

    // Seed the random number generator
    srand(time(0));

    // Generate a random maze and check if it is solvable
    do {
        generateMaze(maze, rows, cols);
    } while (!isMazeSolvable(maze, rows, cols)); // Keep regenerating until the maze is solvable

    int playerX = 0, playerY = 0; // Starting position (top-left corner)

    printf("Welcome to the Maze Escape game!\n");
    printf("You are 'P' and the exit is 'E'. Avoid walls ('#').\n");
    printf("Controls: u = up, d = down, l = left, r = right\n");
    printf("\n");

    // Game loop
    while (1) {
        displayMaze(maze, playerX, playerY, rows, cols); // Display the maze with the player's current position

        // Check if the player reached the exit
        if (playerX == rows - 1 && playerY == cols - 1) {
            printf("Congratulations! You reached the exit!\n");
            break;
        }

        char move;
        printf("\nEnter your move (u/d/l/r): ");
        scanf(" %c", &move); // Get the direction input from the player
        printf("\n");

        // Try to move the player
        if (!movePlayer(maze, &playerX, &playerY, move, rows, cols)) {
            printf("Invalid move! Try again.\n");
        }
    }

    // Free the dynamically allocated memory for the maze
    for (int i = 0; i < rows; i++) {
        free(maze[i]);
    }
    free(maze);

    return 0;
}
