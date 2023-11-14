
#include <string.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

int ballX, ballY;  // Posição inicial da bola
int ballSpeed = 1;  // Velocidade da bola
int ballDirection = 0;  // Direção da bola (0 para parada, 1 para direita, -1 para esquerda)

void printBall(int nextX, int nextY)
{
    screenGotoxy(ballX, ballY);
    printf(" ");  // Limpa a posição anterior da bola
    ballX = nextX;
    ballY = nextY;
    screenGotoxy(ballX, ballY);
    printf("O");  // O é usado para representar a bola
}

int main() 
{
    static int ch = 0;

    screenInit(0);  // Não desenha bordas

    // Definir fundo verde
    screenSetColor(GREEN, GREEN);
    for (int i = MINY; i <= MAXY; i++) {
        for (int j = MINX; j <= MAXX; j++) {
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
    screenUpdate();

    while (ch != 10) // Enter
    {
        // Handle user input
        if (keyhit()) 
        {
            ch = readch();

            // Iniciar movimento da bola para a esquerda
            if (ch == 'a') {
                ballDirection = -1;
            }
            // Iniciar movimento da bola para a direita
            else if (ch == 'd') {
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
            if (newBallX <= MINX || newBallX >= (MAXX - 1)) {
                ballDirection = 0;  // Parar movimento ao atingir bordas
            }

            printBall(newBallX, ballY);
            screenUpdate();
        }
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();

    return 0;
}
