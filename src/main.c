#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h> //REMOVER??? sleep

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define RED_BG "\e[41m"
#define BOLD_YEL "\e[1;33m"
#define BLU "\e[0;34m"
#define GRN "\e[0;32m"
#define CYN "\e[0;36m"
#define UMAG "\e[4;35m"
#define BLK "\e[0;30m"
#define reset "\e[0m"

#define OBSTACLE "#"

int ballX, ballY; // Posição inicial da bola
int ballSpeed = 2; // Velocidade da bola
int ballDirection = 0; // Direção da bola (0 para parada, 1 para direita, -1 para esquerda)

int obstacleX = 20; // Posição X do obstáculo
int obstacleY = MINY; // Posição Y inicial do obstáculo

int score = 0; // Variável de pontuação

#define MAX_PLAYERS 100
#define NAME_SIZE 20

typedef struct PlayerScore {
    char name[NAME_SIZE];
    int score;
    struct PlayerScore *next; 
} PlayerScore;

void cRun();
void menu();
void printBall(int nextX, int nextY);
void printObstacle(int nextX, int nextY);
void game();
void displayScore();
void displayRanking();

int main()
{
    //cRun();

    while (1)
    {
        menu();

        int choice;

        printf(reset "Escolha a opção que deseja realizar: " reset);
        if (scanf("%d", &choice) != 1)
        {
            printf(RED_BG "Entrada inválida. Por favor, insira um número.\n" reset);
            // Clear the input buffer to prevent an infinite loop
            while (getchar() != '\n');
            continue;
        }

        switch (choice)
        {
            case 1:
                game();
                break;
            case 2:
                displayRanking();
                break;
            case 3:
                printf(RED_BG "Você encerrou o programa.\n" reset);
                return 0;
            default:
                printf(RED_BG "Opção inválida, por favor insira um número válido!\n" reset);
        }
    }

    return 0;
}

/*
void cRun()
{
    printf(
        "  ##                                            ##                  ##\n"
        "  ##                                                                ##\n"
        "  ##       ####    ##  ##            ##  ##    ###     #####        ##    ####\n"
        "  #####   ##  ##   #######           ##  ##     ##     ##  ##    #####   ##  ##\n"
        "  ##  ##  ######   ## # ##           ##  ##     ##     ##  ##   ##  ##   ##  ##\n"
        "  ##  ##  ##       ##   ##            ####      ##     ##  ##   ##  ##   ##  ##\n"
        " ######    #####   ##   ##             ##      ####    ##  ##    ######   ####\n"
        );
    
    fflush(stdout);
    screenDestroy(); //Nao ta pegando

    printf(
        " ####     ####\n"
        "    ##   ##  ##\n"
        " #####   ##  ##\n"
        " ##  ##   ##  ##\n"
        " #####    ####\n"
        );

    fflush(stdout);
    screenDestroy(); //Nao ta pegando

    printf(
        "  ####             ######   ##  ##   #####\n"
        " ##  ##             ##  ##  ##  ##   ##  ##\n"
        " ##                 ##      ##  ##   ##  ##\n"
        " ##  ##             ##      ##  ##   ##  ##\n"
        "  ####             ####      ######  ##  ##\n"
        );   
}
*/

void menu()
{
    printf(BOLD_YEL "\n----- MENU PRINCIPAL -----\n" reset);
    printf(BLU "1. Jogar C->RUN\n" reset);
    printf(GRN "2. Visualizar Ranking\n" reset);
    printf(CYN "3. Sair\n" reset);
}

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
    printf("%s", OBSTACLE);
}

void game()
{
    char playerName[100]; // Para armazenar o nome do jogador
    printf("Digite seu nome: ");
    fgets(playerName, 100, stdin); // Lê o nome do jogador
    playerName[strcspn(playerName, "\n")] = 0;

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
                break; 
            }

            displayScore();
            printBall(newBallX, ballY);
            printObstacle(obstacleX, obstacleY);
            screenUpdate();
        }
    }

    FILE *file = fopen("scores.txt", "a"); // Abre o arquivo para adicionar a pontuação
    if (file != NULL) {
        fprintf(file, "%s: %d\n", playerName, score); // Escreve o nome do jogador e a pontuação
        fclose(file); // Fecha o arquivo
    } else {
        printf("Não foi possível abrir o arquivo de pontuações.\n");
    }
    
    keyboardDestroy();
    screenDestroy();
    timerDestroy();
}

void displayScore()
{
    int scorePosX = MAXX - 10; // Ajuste conforme necessário
    int scorePosY = MINY;

    screenSetColor(YELLOW, BLACK); // Escolha a cor desejada
    screenGotoxy(scorePosX, scorePosY);
    printf("Score: %d", score);
}

void displayRanking() {
    FILE *file = fopen("scores.txt", "r");
    if (file == NULL) {
        printf("Não foi possível abrir o arquivo de pontuações.\n");
        return;
    }

    PlayerScore *head = NULL;  

    while (1) {
        PlayerScore *newNode = (PlayerScore *)malloc(sizeof(PlayerScore));
        if (fscanf(file, "%s %d", newNode->name, &newNode->score) != 2) {
            free(newNode);
            break;
        }

        newNode->next = head; 
        head = newNode;       
        }

    fclose(file);

    // Ordenar a lista encadeada por score
    PlayerScore *lista_ordenada = NULL;
    while (head != NULL) {
        PlayerScore *atual = head;
        head = head->next;
       
        if (lista_ordenada == NULL || atual->score > lista_ordenada->score) {
            atual->next = lista_ordenada;
            lista_ordenada = atual;
        } else {
            PlayerScore *temp = lista_ordenada;
            while (temp->next != NULL && atual->score <= temp->next->score) {
                temp = temp->next;
            }
            atual->next = temp->next;
            temp->next = atual;
        }
    }

    printf(UMAG "\nRanking:\n");
    int i = 1;
    PlayerScore *atual = lista_ordenada;
    while (atual != NULL) {
        printf(BLK "%d- %s = %d\n", i, atual->name, atual->score);
        i++;
        atual = atual->next;
    }

    // Libera a memória alocada para a lista encadeada
    atual = head;
    while (atual != NULL) {
        PlayerScore *temp = atual;
        atual = atual->next;
        free(temp);
    }
}