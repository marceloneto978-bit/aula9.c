#ifndef MINHABIB_H
#define MINHABIB_H
#include "minhabib.h"


int banco_aberto = 0;
int numero;

void abrirbanco()
{
    if (banco_aberto == 0)
    {
        banco_aberto = 1;
        printf("Banco aberto com sucesso!\n");
    }
    else
    {
        printf("O banco ja esta aberto!\n");
    }
}

void consultarhistorico()
{
    if (banco_aberto == 1)
    {
        printf("Consultando historico...\n");
        printf("Nenhuma movimentacao cadastrada.\n");
    }
    else
    {
        printf("O banco precisa estar aberto!\n");
    }
}

void sorteio()
{
    if (banco_aberto == 1)
    {
        printf("Iniciando sorteio...\n");
    }
    else
    {
        printf("O banco precisa estar aberto!\n");
    }
}
#endif 