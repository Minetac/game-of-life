#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>

#define SCREENWIDTH  500
#define SCREENHEIGHT 500

#define CELLCOLUMNS 50
#define CELLROWS 50

#define CELLWIDTH (SCREENWIDTH / CELLCOLUMNS)
#define CELLHEIGHT (SCREENHEIGHT / CELLROWS)

#define MAXCELLS 1000

typedef struct {
    int x;
    int y;
    bool active;
} Cell;


// Refactor to use X and Y instead of Cell 
/*
int countBorderingCells(Cell otherCells[MAXCELLS], Cell *currentCell) {
    int count = 0;
    for (int i = 0; i < MAXCELLS; i++) {
        if (&otherCells[i] == currentCell) continue;
        if (!otherCells[i].active) break;
        int xDistance = abs(otherCells[i].x - currentCell->x);
        int yDistance = abs(otherCells[i].y - currentCell->y);
        printf("X: %d, Y: %d, Active: %d\n", otherCells[i].x, otherCells[i].y, otherCells[i].active);

        printf("Distance to current Cell X: %d Y:%d\n", xDistance, yDistance);

        if (xDistance <= 1 && yDistance <= 1) {
            count += 1;
        } else {
        }
    }

    return count;
}
*/

bool cellExistsAtPos(Cell otherCells[MAXCELLS], int currentX, int currentY) {
    for (int i = 0; i < MAXCELLS; i++) {
        if (otherCells[i].active) {
            if (otherCells[i].x == currentX && otherCells[i].y == currentY && otherCells[i].active) return true;
        } else {
            break;
        }
    }
    return false;
}

int countBorderingCells(Cell otherCells[MAXCELLS], int currentX, int currentY) {
    int count = 0;
    for (int i = 0; i < MAXCELLS; i++) {
        if (otherCells[i].x == currentX && otherCells[i].y == currentY) continue;
        if (!otherCells[i].active) break;
        int xDistance = abs(otherCells[i].x - currentX);
        int yDistance = abs(otherCells[i].y - currentY);
        if (xDistance <= 1 && yDistance <= 1) {
            count += 1;
        }
    }
    return count;
}

void addCell(Cell cells[MAXCELLS], int x, int y) {

    for (int i = 0; i < MAXCELLS; i++) {
        if (cells[i].active) continue;
        else {
            cells[i].x = x;
            cells[i].y = y;
            cells[i].active = true;
            break;
        }
    }
}

void reorganizeArray(Cell cells[MAXCELLS]) {
    Cell tempCells[MAXCELLS];
    memcpy(tempCells, cells, MAXCELLS * sizeof(Cell));
    for (int i = 0; i < MAXCELLS; i++) {
        cells[i].x = -1;
        cells[i].y = -1;
        cells[i].active = false;
    }
    for (int i = 0; i < MAXCELLS; i++) {
        if (tempCells[i].active) {
            addCell(cells, tempCells[i].x, tempCells[i].y);
        }
    }
}

void removeCellAtPos(Cell cells[MAXCELLS], int x, int y) {
    for (int i = 0; i < MAXCELLS; i++) {
        if (cells[i].active && cells[i].x == x && cells[i].y == y) {
            cells[i].active = false;
            reorganizeArray(cells);
            return;
        }
    }
}

int main(void) {

    InitWindow(SCREENWIDTH, SCREENHEIGHT, "Cellular Automata");

    Cell cells[MAXCELLS];
    Cell cellsUpdated[MAXCELLS];

    for (int i = 0; i < MAXCELLS; i++) {
        cells[i].x = -1;
        cells[i].y = -1;
        cells[i].active = false;
        cellsUpdated[i].x = -1;
        cellsUpdated[i].y = -1;
        cellsUpdated[i].active = false;
    }

    addCell(cells, 5, 5);
    addCell(cells, 6, 3);
    addCell(cells, 6, 5);
    addCell(cells, 7, 4);
    addCell(cells, 7, 5);

    SetTargetFPS(60);
    int currentFrame = 0;
    bool processLogic = false;

    while (!WindowShouldClose()) {
        if (!cells[0].active && currentFrame == 0) printf("No Active Cells!\n");

        // Reset Cell Mirror
        for (int i = 0; i < MAXCELLS; i++) {
            if (!cellsUpdated[i].active) break;
            cellsUpdated[i].x = -1;
            cellsUpdated[i].y = -1;
            cellsUpdated[i].active = false;
        }


        // Input Handling
        int key = GetKeyPressed();

        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
            Vector2 mousePosition = GetMousePosition();
            printf("%d, %d\n", mousePosition.x, mousePosition.y);
            if (!cellExistsAtPos(cells, (int) mousePosition.x / CELLWIDTH, (int) mousePosition.y / CELLHEIGHT)) {
                addCell(cells, (int) mousePosition.x / CELLWIDTH, (int) mousePosition.y / CELLHEIGHT);
            }
        }
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
            Vector2 mousePosition = GetMousePosition();
            printf("%d, %d\n", mousePosition.x, mousePosition.y);
            if (cellExistsAtPos(cells, (int) mousePosition.x / CELLWIDTH, (int) mousePosition.y / CELLHEIGHT)) {
                removeCellAtPos(cells, (int) mousePosition.x / CELLWIDTH, (int) mousePosition.y / CELLHEIGHT);
            }
        }

        while (key > 0) {
            printf("%c\n", (char) key);
            if ((char) key == ' ') {
                processLogic = !processLogic;
            }

            key = GetKeyPressed();
        }

        /* Debug Active Cell Positions*/
        /*
        for (int i = 0; i < MAXCELLS; i++) {
            if (cells[i].active) {
                printf("Active Cell at Index: %d, at X: %d Y: %d\n", i, cells[i].x, cells[i].y);
            } 
        }
        */

        // Begin Drawing
        BeginDrawing();
            ClearBackground(WHITE);
            // for (int i = 0; !cells[i].active; i++) {
            for (int i = 0; i < MAXCELLS; i++) {
                if (!cells[i].active) break;
                DrawRectangle(CELLWIDTH * cells[i].x, CELLHEIGHT * cells[i].y, CELLWIDTH, CELLHEIGHT, BLACK);
            }
            for (int i = 0; i <= CELLCOLUMNS; i++) {
                DrawLine(i * CELLWIDTH, 0, i * CELLWIDTH, SCREENHEIGHT, GRAY);
            }
            for (int i = 0; i <= CELLROWS; i++) {
                DrawLine(0, i * CELLWIDTH, SCREENWIDTH, i * CELLWIDTH, GRAY);
            }
        EndDrawing();


        // Logic
        if (currentFrame == 0 && processLogic) {
            int index = 0;
            while(cells[index].active) {
                int borderingCells = countBorderingCells(cells, cells[index].x, cells[index].y);
                
                if (borderingCells >= 2 && borderingCells <= 3) {
                    addCell(cellsUpdated, cells[index].x, cells[index].y);
                }


                // Check Surrounding Cells
                for (int xOffset = -1; xOffset <= 1; xOffset++) {
                    for (int yOffset = -1; yOffset <= 1; yOffset++) {
                        if (xOffset == 0 && yOffset == 0) continue;
                        int xCurrent = cells[index].x + xOffset;
                        int yCurrent = cells[index].y + yOffset;
                        borderingCells = countBorderingCells(cells, xCurrent, yCurrent);
                        if (borderingCells == 3) {
                            if (!cellExistsAtPos(cells, xCurrent, yCurrent) && !cellExistsAtPos(cellsUpdated, xCurrent, yCurrent)) {
                                addCell(cellsUpdated, cells[index].x + xOffset, cells[index].y + yOffset);
                            }
                        }
                    }
                }
                index += 1;
            }
            // Update Cells with Mirror
            memcpy(cells, cellsUpdated, MAXCELLS * sizeof(Cell));
        }

        if (++currentFrame >= 60) {
            currentFrame = 0;
        }
    }

    CloseWindow();

    return 0;
}

