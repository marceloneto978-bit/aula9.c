#include <stdio.h>
int main(void)
 
int qtd ;
int opcao;
int banco_aberto = 0;
int numero;

{
    for (opcao = 0; opcao != 4)
    {
        printf("\n=====carteira digital=====\n");
        printf("1-abrir o banco\n");
        printf("2-consultar historico\n");
        printf("3-sair\n");
        printf("4-sorteio\n");
        printf("escolha uma opcao: ");
        scanf("%d", &opcao); 

        opcao=(lerint("escolha uma opcao: "));                                      

        switch (opcao)
        {
        case 1:
            printf("abrindo o banco...\n");
            break;
        case 2:
            printf("consultando historico...\n");
            break;
        case 3:
            printf("saindo...\n");
            break;
        case 4:
            printf("sorteando...\n");
            break;
        default:
            printf("opcao invalida!\n");
            break;
        }
    }
    return 0;
}

