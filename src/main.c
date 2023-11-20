#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define OBSTACLE '#'

int ballX, ballY; // Posição inicial da bola
int ballSpeed = 2; // Velocidade da bola
int ballDirection = 0; // Direção da bola (0 para parada, 1 para direita, -1 para esquerda)

int obstacleX = 20; // Posição X do obstáculo
int obstacleY = MINY; // Posição Y inicial do obstáculo

int score = 0; // Variável de pontuação


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
    for (int i = MINY-1; i <= MAXY; i++) {
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

void displayScore() {
    int scorePosX = MAXX - 10; // Ajuste conforme necessário
    int scorePosY = MINY;

    screenSetColor(YELLOW, BLACK); // Escolha a cor desejada
    screenGotoxy(scorePosX, scorePosY);
    printf("Score: %d", score);
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
            if (obstacleY > MAXY)
            {
                // Limpa a posição atual do obstáculo
                for (int i = MINY-1; i <= MAXY; i++) {
                    screenGotoxy(obstacleX, i);
                    printf(" ");
                }

                // Reposiciona o obstáculo
                obstacleX = rand() % (MAXX - MINX - 1) + MINX;
                obstacleY = MINY;
                score++; // Score de acordo com a quantidade de obstáculos desviados
            } else {
                // Se o obstáculo não atingiu o limite inferior, desenhe-o
                screenGotoxy(obstacleX, obstacleY);
                printf(" ");
            
            }
            // Verificar colisão com obstáculo
            if (newBallX == obstacleX && ballY == obstacleY)
            {
                break; // Inverte a direção ao colidir com o obstáculo
            }

            displayScore();
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
