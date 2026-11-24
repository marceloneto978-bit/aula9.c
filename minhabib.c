#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "minhabib.h"

int banco_aberto = 0;
int numero;
int total_clientes = 0;
Cliente clientes[MAX_CLIENTES];

static int proximo_numero_conta = 1000;


int lerint(char mensagem[])
{
    int valor;
    int c;

    printf("%s", mensagem);
    while (scanf("%d", &valor) != 1)
    {
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        if (c == EOF)
            return 3; /* fim da entrada: devolve "sair" */
        printf("Valor invalido! %s", mensagem);
    }
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    return valor;
}


double lerdouble(char mensagem[])
{
    double valor;
    int c;

    printf("%s", mensagem);
    while (scanf("%lf", &valor) != 1)
    {
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        if (c == EOF)
            return -1;
        printf("Valor invalido! %s", mensagem);
    }
    while ((c = getchar()) != '\n' && c != EOF)
        ;
    return valor;
}

void lerstring(char mensagem[], char destino[], int tamanho)
{
    printf("%s", mensagem);
    if (fgets(destino, tamanho, stdin) == NULL)
    {
        destino[0] = '\0';
        return;
    }
    destino[strcspn(destino, "\n")] = '\0';
}


static int banco_ok(void)
{
    if (banco_aberto == 0)
    {
        printf("O banco precisa estar aberto!\n");
        return 0;
    }
    return 1;
}

static void registrar_historico(Cliente *c, const char *texto)
{
    if (c->qtd_historico == MAX_HISTORICO)
    {
        memmove(c->historico[0], c->historico[1],
                (MAX_HISTORICO - 1) * TAM_LINHA);
        c->qtd_historico--;
    }
    snprintf(c->historico[c->qtd_historico], TAM_LINHA, "%s", texto);
    c->qtd_historico++;
}

static double saldo_total(const Cliente *c)
{
    double total = 0;
    int i;

    for (i = 0; i < 3; i++)
        if (c->contas[i].ativa)
            total += c->contas[i].saldo;
    return total;
}

static const char *nome_tipo(int tipo)
{
    switch (tipo)
    {
    case CONTA_CORRENTE:
        return "corrente";
    case CONTA_SALARIO:
        return "salario";
    default:
        return "poupanca";
    }
}

static int escolher_cliente(void)
{
    int i, escolha;

    if (total_clientes == 0)
    {
        printf("Nenhum cliente cadastrado.\n");
        return -1;
    }
    printf("Clientes:\n");
    for (i = 0; i < total_clientes; i++)
        printf("%d - %s\n", i + 1, clientes[i].nome);

    escolha = lerint("Escolha o cliente: ");
    if (escolha < 1 || escolha > total_clientes)
    {
        printf("Cliente invalido!\n");
        return -1;
    }
    return escolha - 1;
}

static int ler_tipo_conta(void)
{
    int tipo = lerint("Tipo de conta (1-corrente, 2-salario, 3-poupanca): ");

    if (tipo < 1 || tipo > 3)
    {
        printf("Tipo de conta invalido!\n");
        return 0;
    }
    return tipo;
}


void menubanco(void)
{
    printf("\n=====carteira digital=====\n");
    printf("1-abrir o banco\n");
    printf("2-consultar historico\n");
    printf("3-sair\n");
    printf("4-sorteio\n");
    printf("5-cadastrar cliente\n");
    printf("6-abrir conta\n");
    printf("7-depositar\n");
    printf("8-solicitar emprestimo\n");
}

void abrirbanco(void)
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

void cadastrar_cliente(void)
{
    Cliente *c;

    if (!banco_ok())
        return;
    if (total_clientes == MAX_CLIENTES)
    {
        printf("Limite de clientes atingido!\n");
        return;
    }

    c = &clientes[total_clientes];
    memset(c, 0, sizeof(Cliente));
    lerstring("Nome do cliente: ", c->nome, TAM_NOME);
    if (c->nome[0] == '\0')
    {
        printf("Nome invalido!\n");
        return;
    }
    total_clientes++;
    registrar_historico(c, "Cliente cadastrado.");
    printf("Cliente cadastrado com sucesso!\n");
}

void abrir_conta(void)
{
    int idx, tipo;
    Cliente *c;
    char linha[TAM_LINHA];

    if (!banco_ok())
        return;
    idx = escolher_cliente();
    if (idx < 0)
        return;
    tipo = ler_tipo_conta();
    if (tipo == 0)
        return;

    c = &clientes[idx];

    if (c->contas[tipo - 1].ativa)
    {
        printf("O cliente ja possui conta %s!\n", nome_tipo(tipo));
        return;
    }
    if (tipo == CONTA_SALARIO && c->contas[CONTA_CORRENTE - 1].ativa)
    {
        printf("Cliente nao pode ter conta-salario e corrente ao mesmo tempo!\n");
        return;
    }
    if (tipo == CONTA_CORRENTE && c->contas[CONTA_SALARIO - 1].ativa)
    {
        printf("Cliente nao pode ter conta corrente e conta-salario ao mesmo tempo!\n");
        return;
    }

    c->contas[tipo - 1].ativa = 1;
    c->contas[tipo - 1].numero = proximo_numero_conta++;
    c->contas[tipo - 1].saldo = 0;

    snprintf(linha, TAM_LINHA, "Conta %s aberta: %d-%02d",
             nome_tipo(tipo), c->contas[tipo - 1].numero, tipo);
    registrar_historico(c, linha);
    printf("Conta aberta! Numero: %d-%02d\n", c->contas[tipo - 1].numero, tipo);
}

void depositar(void)
{
    int idx, tipo;
    double valor;
    Cliente *c;
    char linha[TAM_LINHA];

    if (!banco_ok())
        return;
    idx = escolher_cliente();
    if (idx < 0)
        return;
    tipo = ler_tipo_conta();
    if (tipo == 0)
        return;

    c = &clientes[idx];
    if (!c->contas[tipo - 1].ativa)
    {
        printf("O cliente nao possui conta %s!\n", nome_tipo(tipo));
        return;
    }

    valor = lerdouble("Valor do deposito: ");
    if (valor <= 0)
    {
        printf("Valor invalido!\n");
        return;
    }

    c->contas[tipo - 1].saldo += valor;
    snprintf(linha, TAM_LINHA, "Deposito de R$ %.2f na conta %d-%02d",
             valor, c->contas[tipo - 1].numero, tipo);
    registrar_historico(c, linha);
    printf("Deposito realizado! Novo saldo: R$ %.2f\n", c->contas[tipo - 1].saldo);
}

void solicitar_emprestimo(void)
{
    int idx;
    double limite;
    Cliente *c;
    char linha[TAM_LINHA];

    if (!banco_ok())
        return;
    idx = escolher_cliente();
    if (idx < 0)
        return;

    c = &clientes[idx];

    if (c->tem_emprestimo)
    {
        printf("O cliente ja possui um emprestimo (maximo de um por cliente)!\n");
        return;
    }
    if (!c->contas[CONTA_POUPANCA - 1].ativa || c->contas[CONTA_POUPANCA - 1].saldo <= 0)
    {
        printf("E preciso ter saldo na conta-poupanca para pedir emprestimo!\n");
        return;
    }
    if (!c->contas[CONTA_CORRENTE - 1].ativa)
    {
        printf("E preciso ter conta corrente para pedir emprestimo!\n");
        return;
    }

    limite = 0.25 * 10 * saldo_total(c);
    printf("Limite de emprestimo: R$ %.2f\n", limite);

    c->tem_emprestimo = 1;
    c->valor_emprestimo = limite;
    c->contas[CONTA_CORRENTE - 1].saldo += limite;

    snprintf(linha, TAM_LINHA, "Emprestimo de R$ %.2f creditado na conta corrente", limite);
    registrar_historico(c, linha);
    printf("Emprestimo concedido e creditado na conta corrente!\n");
}

void consultar_historico(void)
{
    int idx, i;
    Cliente *c;

    if (!banco_ok())
        return;
    idx = escolher_cliente();
    if (idx < 0)
        return;

    c = &clientes[idx];
    printf("\nConsultando historico de %s...\n", c->nome);

    for (i = 0; i < 3; i++)
        if (c->contas[i].ativa)
            printf("Conta %s %d-%02d | saldo: R$ %.2f\n",
                   nome_tipo(i + 1), c->contas[i].numero, i + 1, c->contas[i].saldo);

    if (c->qtd_historico == 0)
        printf("Nenhuma movimentacao cadastrada.\n");
    for (i = 0; i < c->qtd_historico; i++)
        printf("%d. %s\n", i + 1, c->historico[i]);
}

void efetuarsorteio(void)
{
    if (!banco_ok())
        return;
    printf("Iniciando sorteio...\n");
    srand((unsigned)time(NULL));
    numero = rand() % 100 + 1;
    printf("Numero sorteado: %d\n", numero);
}
