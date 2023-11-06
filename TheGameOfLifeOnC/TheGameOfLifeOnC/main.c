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

#define N 25  // высота
#define M 80  // ширина

int creating_points(int A[N][M], int B[N][M]);
void rendering(int A[N][M]);
void initialization(int A[N][M]);
void update() { printf("\033[H\033[J"); }

int main() {
    int A[N][M];
    int B[N][M];
    int speed = 1000;
    int era = 0;
    int life = 0;
    initialization(A);

    initscr();
    cbreak();
    noecho();

    nodelay(stdscr, TRUE);

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
        int input = getch();
        if (input == 'f') {
            speed -= 200;
        } else if ((input == 's') && speed < 5000) {
            speed += 200;
        } else if (input == 'q') {
            exit(1);
        }
        era += 1;
    }
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
    FILE* fp = freopen("/dev/tty", "r", stdin);
    if (fp == NULL) {
        exit(1);
    }
}

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
