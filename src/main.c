#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define OBSTACLE '#'

int ballX, ballY; // Posição inicial da bola
int ballSpeed = 1; // Velocidade da bola
int ballDirection = 0; // Direção da bola (0 para parada, 1 para direita, -1 para esquerda)

int obstacleX = 20; // Posição X do obstáculo
int obstacleY = MAXY-24; // Posição Y inicial do obstáculo

void printBall(int nextX, int nextY)
{
    screenSetColor(GREEN, BLACK);
    screenGotoxy(ballX, ballY);
    printf(" "); // Limpa a posição anterior da bola
    ballX = nextX;
    ballY = nextY;
    screenGotoxy(ballX, ballY);
    printf("O"); // O é usado para representar a bola
}

void printObstacle(int nextX, int nextY)
{
    screenSetColor(RED, BLACK);
    //Limpa a linha vertical anterior do obstáculo
    for (int i = MINY; i <= MAXY; i++) {
        screenGotoxy(obstacleX, i);
        printf(" ");
    }
    screenGotoxy(obstacleX, obstacleY);
    printf(" ");

    // Atualiza as coordenadas do obstáculo para as novas coordenadas fornecidas
    obstacleX = nextX;
    obstacleY = nextY;

    // Imprime o obstáculo na nova posição
    screenGotoxy(obstacleX, obstacleY);
    printf("%c", OBSTACLE);
}


int main()
{
    static int ch = 0;

    screenInit(0); // Não desenha bordas

    // Definir fundo preto
    screenSetColor(BLACK, BLACK);
    for (int i = MINY; i <= MAXY+1; i++)
    {
        for (int j = MINX; j <= MAXX; j++)
        {
            screenGotoxy(j, i);
            printf(" ");
        }
    }

    // Inicializar a posição da bola no meio da parte inferior da tela
    ballX = (MINX + MAXX) / 2;
    ballY = MAXY;

    keyboardInit();
    timerInit(50);

    printBall(ballX, ballY);
    printObstacle(obstacleX, obstacleY);
    screenUpdate();
    timerInit(100);
    while (ch != 10) // Enter
    {
        // Handle user input
        if (keyhit())
        {
            ch = readch();

            // Iniciar movimento da bola para a esquerda
            if (ch == 'a')
            {
                ballDirection = -1;
            }
            // Iniciar movimento da bola para a direita
            else if (ch == 'd')
            {
                ballDirection = 1;
            }

            screenUpdate();
        }

        // Update game state (verificar colisões, etc)
        if (timerTimeOver() == 1)
        {
            // Lógica de movimento da bola
            int newBallX = ballX + (ballSpeed * ballDirection);

            // Verificar limites da tela
            if (newBallX <= MINX || newBallX >= (MAXX - 1))
            {
                ballDirection = 0; // Parar movimento ao atingir bordas
            }

            // Movimentar o obstáculo para baixo
            obstacleY++;
            

            // Verificar se o obstáculo atingiu o limite inferior da tela
            if (obstacleY >= MAXY)
            {
                printObstacle(obstacleX, obstacleY);
                
                // Reposicionar o obstáculo para uma nova posição aleatória no topo da tela
                obstacleX = rand() % (MAXX - MINX - 1) + MINX;
                obstacleY = MINY;
            }

            // Verificar colisão com obstáculo
            if (newBallX == obstacleX && ballY == obstacleY)
            {
                break; // Inverte a direção ao colidir com o obstáculo
            }

            printBall(newBallX, ballY);
            printObstacle(obstacleX, obstacleY);
            screenUpdate();
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
