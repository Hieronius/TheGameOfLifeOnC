//
//  main.c
//  TheGameOfLifeOnC
//
//  Created by Арсентий Халимовский on 06.11.2023.
//

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// MARK: ncurses library guide

// 1. initscr() - first command to initiate a programm with ncurses
// It will initialize all the tools and options of the Ncurses
// Also it's will refresh to clear the screen

// 2. cbreak() -

// 3. noecho() -

// 4. nodelay(stdscr, TRUE) -

// 5. endwin() -


#define N 25  // высота
#define M 80  // ширина

int creating_points(int A[N][M], int B[N][M]);
void rendering(int A[N][M]);
void initialization(int A[N][M]);
void update() { printf("\033[H\033[J"); }

int main() {
    /// First matrix to store the game field
    int A[N][M];
    /// Second matrix to store the second one game field
    int B[N][M];
    // Speed of the game
    int speed = 1000;
    // Seems like a cycle phase of the last living cells
    int era = 0;
    // Some kind of the counter inside the program
    int life = 0;
    // Start configuration of the game field settings
    initialization(A);

    // Control operation block of Ncurses (library)
    // It's a way to substitute the mouse and let the user work with graphic, sound and other tools right from the terminal line.
    initscr();
    cbreak();
    noecho();

    nodelay(stdscr, TRUE);

    // Main cycle of the program. Until it end cells will grow and die without any limit of speed
    while (life == 0) {
        printf("ERA: %d\n", era);
        rendering(A);
        usleep(speed * 100);
        update();
        if (creating_points(A, B) == 1) {
            printf("GAME OVER\n");
            printf("LIFE IS OVER");
            life = 1;
        }
        // Block of code to set the speed of the game by input different keys
        // Can't be faster than 5000
        int input = getch();
        if (input == 'f') {
            speed -= 200;
        } else if ((input == 's') && speed < 5000) {
            speed += 200;
        } else if (input == 'q') {
            exit(1);
        }
        // Add a new era to the counter each iteration of TheGameOfLife cycle
        era += 1;
    }
    // You should initiate this command each time when programm is done.
    // So it will return to the basic settings before Ncurses implementation.
    endwin();
    return 0;
}

int creating_points(int A[N][M], int B[N][M]) {
    int i1, i3, j1, j3, life = 1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (i == 0) {
                i1 = N - 1;
                i3 = i + 1;
            } else if (i == N - 1) {
                i1 = i - 1;
                i3 = 0;
            } else {
                i1 = i - 1;
                i3 = i + 1;
            }

            if (j == 0) {
                j1 = M - 1;
                j3 = j + 1;
            } else if (j == M - 1) {
                j1 = j - 1;
                j3 = 0;
            } else {
                j1 = j - 1;
                j3 = j + 1;
            }

            int sum01 =
                A[i1][j1] + A[i1][j] + A[i1][j3] + A[i][j1] + A[i][j3] + A[i3][j1] + A[i3][j] + A[i3][j3];
            if ((sum01 == 2 && A[i][j] == 1) || (sum01 == 3)) {
                B[i][j] = 1;
                life = 0;
            } else {
                B[i][j] = 0;
            }
        }
    }

    // Запись расчётов в матрицу А из матрицы В
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            A[i][j] = B[i][j];
        }
    }
    return life;
}
// Open the file and get it's data to initialize game elements algorithms
void initialization(int A[N][M]) {
    // Заполнение массива информацией из файла    `
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            char buf;
            if ((buf = getchar()) != EOF) {
                A[i][j] = buf % 2;
            }
        }
    }
    // Get access to the file
    FILE* fp = freopen("/dev/tty", "r", stdin);
    if (fp == NULL) {
        exit(1);
    }
}
// Draw the game field
void rendering(int A[N][M]) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (A[i][j] == 1) {
                printf("#");
            } else {
                printf(" ");
            }
        }
        printf("\n\r");
    }
    fflush(stdout);
}
