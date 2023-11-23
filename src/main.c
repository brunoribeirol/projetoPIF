#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "screen.h"
#include "keyboard.h"
#include "timer.h"

#define RED_BG "\e[41m"
#define BOLD_YEL "\e[1;33m"
#define BLU "\e[0;34m"
#define GRN "\e[0;32m"
#define CYN "\e[0;36m"
#define BLK "\e[0;30m"
#define BMAG "\e[1;35m"

#define reset "\e[0m"

#define OBSTACLE "💣"
#define PLAYER "⚪️"
#define MAX_OBSTACLES 5

#define MAX_PLAYERS 100
#define NAME_SIZE 20

int ballX, ballY; // Posição inicial da bola
int ballSpeed = 3;
int ballDirection = 0; // Direção da bola (0 para parada, 1 para direita, -1 para esquerda)

int obstacleX[MAX_OBSTACLES]; // Posição X dos obstáculos
int obstacleY[MAX_OBSTACLES]; // Posição Y dos obstáculos
int obstacleSpeed[MAX_OBSTACLES]; // Velocidade dos obstáculos

int score = 0; // Variável de pontuação
typedef struct PlayerScore {
    char name[NAME_SIZE];
    int score;
    struct PlayerScore *next; 
} PlayerScore;

void cRun();
void menu();
void printBall(int nextX, int nextY);
void printObstacle(int index, int nextX, int nextY);
void game();
void displayScore();
void displayRanking();


int main()
{
    cRun();

    while (1)
    {
        menu();

        int choice;
        printf(reset "Escolha a opção que deseja realizar: " reset);

        if (scanf("%d", &choice) != 1)
        {
            printf(RED_BG "Entrada inválida. Por favor, insira um número.\n" reset);
            // Limpar o buffer de entrada
            while (getchar() != '\n');
            // Voltar ao início do loop para obter uma entrada válida
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
 
void cRun()
{
    printf(BMAG
        "  ##                                            ##                  ##\n"
        "  ##                                                                ##\n"
        "  ##       ####    ##  ##            ##  ##    ###     #####        ##    ####\n"
        "  #####   ##  ##   #######           ##  ##     ##     ##  ##    #####   ##  ##\n"
        "  ##  ##  ######   ## # ##           ##  ##     ##     ##  ##   ##  ##   ##  ##\n"
        "  ##  ##  ##       ##   ##            ####      ##     ##  ##   ##  ##   ##  ##\n"
        " ######    #####   ##   ##             ##      ####    ##  ##    ######   ####\n"
    );

    printf(
        "                          ####     ####\n"
        "                             ##   ##  ##\n"
        "                          #####   ##  ##\n"
        "                         ##  ##   ##  ##\n"
        "                          #####    ####\n"
    );

    printf(
        "                   ####             ######   ##  ##   #####\n"
        "                   ##  ##             ##  ##  ##  ##   ##  ##\n"
        "                   ##                 ##      ##  ##   ##  ##\n"
        "                   ##  ##             ##      ##  ##   ##  ##\n"
        "                   ####              ###      ######   ##  ##\n"
    );
}

void menu()
{
    printf(BOLD_YEL "\n----- MENU PRINCIPAL -----\n" reset);
    printf(BLU "1. Jogar C->RUN\n" reset);
    printf(GRN "2. Visualizar Ranking\n" reset);
    printf(CYN "3. Sair\n" reset);
}

void printBall(int nextX, int nextY)
{
    screenGotoxy(ballX, ballY);
    printf(" "); // Limpa a posição anterior da bola
    ballX = nextX;
    ballY = nextY;
    screenGotoxy(ballX, ballY);
    printf(PLAYER);
}

void printObstacle(int index, int nextX, int nextY)
{
    // Limpa a posição atual do obstáculo
    for (int i = MINY - 1; i <= MAXY; i++)
    {
        screenGotoxy(obstacleX[index], i);
        printf(" ");
    }

    // Atualiza as coordenadas do obstáculo para as novas coordenadas fornecidas
    obstacleX[index] = nextX;
    obstacleY[index] = nextY;

    // Imprime o obstáculo na nova posição
    screenGotoxy(obstacleX[index], obstacleY[index]);
    printf("%s", OBSTACLE);
}

void game()
{
    char playerName[NAME_SIZE];
    printf("Digite seu nome: ");
    // Limpar o buffer de entrada antes de ler o nome
    while ((getchar()) != '\n');
    fgets(playerName, NAME_SIZE, stdin);
    playerName[strcspn(playerName, "\n")] = 0;

    // Resetar a pontuação
    score = 0;

    // Resetar a posição e direção da bola
    ballX = (MINX + MAXX) / 2;
    ballY = MAXY;
    ballDirection = 0; // Direção da bola inicialmente parada

    // Resetar a posição e velocidade dos obstáculos
    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        obstacleX[i] = rand() % (MAXX - MINX - 1) + MINX;
        obstacleY[i] = MINY;
        obstacleSpeed[i] = 1; // Velocidade inicial dos obstáculos
    }

    screenInit(0); // Não desenha bordas

    
    // Definir fundo preto
    screenSetColor(BLACK, BLACK);
    for (int i = MINY; i <= MAXY + 1; i++)
    {
        for (int j = MINX; j <= MAXX; j++)
        {
            screenGotoxy(j, i);
            printf(" ");
        }
    }


    keyboardInit();
    timerInit(50);

    printBall(ballX, ballY);

    // Imprimir obstáculos iniciais
    for (int i = 0; i < MAX_OBSTACLES; i++)
    {
        printObstacle(i, obstacleX[i], obstacleY[i]);
    }

    screenUpdate();
    timerInit(100);
    static int ch = 0;

    while (ch != 10) // Enter (encerra o jogo)
    {
        // Handle user input
        if (keyhit())
        {
            ch = readch();

            // Iniciar movimento da bola para a esquerda
            if (ch == 'a' || ch == 'A')
            {
                ballDirection = -1;
            }
            // Iniciar movimento da bola para a direita
            else if (ch == 'd' || ch == 'D')
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

            // Movimentar os obstáculos
            for (int i = 0; i < MAX_OBSTACLES; i++)
            {
                obstacleY[i] += obstacleSpeed[i];

                // Verificar se a bola colidiu com algum obstáculo
                if (newBallX >= obstacleX[i] && newBallX < (obstacleX[i] + strlen(OBSTACLE)) && ballY == obstacleY[i])
                {
                    printf(RED_BG "Você colidiu com um obstáculo! Fim de jogo.\n" reset);
                    ch = 10; // Encerra o jogo
                    break;
                }

                // Verificar se o obstáculo atingiu o limite inferior da tela
                if (obstacleY[i] > MAXY)
                {
                    // Limpa a posição atual do obstáculo
                    for (int j = MINY - 1; j <= MAXY; j++)
                    {
                        screenGotoxy(obstacleX[i], j);
                        printf(" ");
                    }

                    // Reposiciona o obstáculo
                    obstacleX[i] = rand() % (MAXX - MINX - 1) + MINX;
                    obstacleY[i] = MINY;
                    score++; // Score de acordo com a quantidade de obstáculos desviados

                    // Ajusta a velocidade do obstáculo e o intervalo de aparecimento
                    if (score > 0 && score % 10 == 0 && obstacleSpeed[i] < 3)
                    {
                        obstacleSpeed[i] += score / 10 + 1; // Aumenta a velocidade dos obstáculos a cada 10 pontos
                    }
                }
                else
                {
                    // Se o obstáculo não atingiu o limite inferior, desenhe-o
                    screenGotoxy(obstacleX[i], obstacleY[i]);
                    printf(" ");
                }
            }

            // Verificar se a bola colidiu com algum obstáculo
            if (ch == 10)
            {
                break;
            }

            displayScore();
            printBall(newBallX, ballY); // Bola nov posiço
            // Imprimir obstáculos na nova posição
            for (int i = 0; i < MAX_OBSTACLES; i++)
            {
                printObstacle(i, obstacleX[i], obstacleY[i]);
            }

            screenUpdate();
        }
    }

    FILE *file = fopen("scores.txt", "a"); //Abe o arquivo para adicionar a pontuação
    if (file != NULL)
    {
        fprintf(file, "%s: %d\n", playerName, score);
        fclose(file);
    }
    else
    {
        printf("Não foi possível abrir o arquivo de pontuações.\n");
    }

    keyboardDestroy();
    screenDestroy();
    timerDestroy();
    displayRanking();
}

void displayScore()
{
    int scorePosX = MAXX - 10; // Ajuste conforme necessário
    int scorePosY = MINY;

    screenSetColor(YELLOW, BLACK); // Cor
    screenGotoxy(scorePosX, scorePosY);
    printf("Score: %d", score);
}

void displayRanking()
{
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
    while (head != NULL)
    {
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

    printf(BMAG
    "  ######     ##     ##   ##  ###  ##   ####    ##   ##    ####\n"
    "  ##  ##   ####    ###  ##   ##  ##    ##     ###  ##   ##  ##\n"
    "  ##  ##  ##  ##   #### ##   ## ##     ##     #### ##  ##\n"
    "  #####   ##  ##   ## ####   ####      ##     ## ####  ##\n"
    "  ## ##   ######   ##  ###   ## ##     ##     ##  ###  ##  ###\n"
    "  ##  ##  ##  ##   ##   ##   ##  ##    ##     ##   ##   ##  ##\n"
    " #### ##  ##  ##   ##   ##  ###  ##   ####    ##   ##    #####\n"
    );

    int i = 1;
    PlayerScore *atual = lista_ordenada;
    while (atual != NULL)
    {
        printf(BLK "%d %s = %d\n", i, atual->name, atual->score);
        i++;
        atual = atual->next;
    }

    atual = head;
    while (atual != NULL)
    {
        PlayerScore *temp = atual;
        atual = atual->next;
        free(temp);
    }
}