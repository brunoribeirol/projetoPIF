#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "timer.h"

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        // Para sistemas baseados em Unix (incluindo Linux e macOS)
        // A sequência de escape ANSI '\e[3J' ajuda a limpar o buffer de rolagem no macOS
        system("clear && printf '\e[3J'");
    #endif
}

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
    sleep(2);
    timerDestroy();

    printf(
        " ####     ####\n"
        "    ##   ##  ##\n"
        " #####   ##  ##\n"
        " ##  ##   ##  ##\n"
        " #####    ####\n"
        );

    fflush(stdout);
    sleep(2);
    timerDestroy();

    printf(
        "  ####             ######   ##  ##   #####\n"
        " ##  ##             ##  ##  ##  ##   ##  ##\n"
        " ##                 ##      ##  ##   ##  ##\n"
        " ##  ##             ##      ##  ##   ##  ##\n"
        "  ####             ####      ######  ##  ##\n"
        );   
}