#include <stdio.h>
#include "minhabib.h"

int main(void)
{
    int opcao = 0;

    while (opcao != 3)
    {
        menubanco();
        opcao = lerint("escolha uma opcao: ");

        switch (opcao)
        {
        case 1:
            abrirbanco();
            break;
        case 2:
            consultar_historico();
            break;
        case 3:
            printf("saindo...\n");
            break;
        case 4:
            efetuarsorteio();
            break;
        case 5:
            cadastrar_cliente();
            break;
        case 6:
            abrir_conta();
            break;
        case 7:
            depositar();
            break;
        case 8:
            solicitar_emprestimo();
            break;
        default:
            printf("opcao invalida!\n");
            break;
        }
    }
    return 0;
}
