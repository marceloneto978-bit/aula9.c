#ifndef MINHABIB_H
#define MINHABIB_H

#define MAX_CLIENTES 100
#define MAX_HISTORICO 50
#define TAM_NOME 50
#define TAM_LINHA 100

#define CONTA_CORRENTE 1
#define CONTA_SALARIO 2
#define CONTA_POUPANCA 3

typedef struct
{
    int ativa;    
    int numero;   
    double saldo;
} Conta;

typedef struct
{
    char nome[TAM_NOME];
    Conta contas[3]; 
    char historico[MAX_HISTORICO][TAM_LINHA];
    int qtd_historico;
    int tem_emprestimo;
    double valor_emprestimo;
} Cliente;
extern int banco_aberto;
extern int numero;
extern int total_clientes;
extern Cliente clientes[MAX_CLIENTES];
int lerint(char mensagem[]);
double lerdouble(char mensagem[]);
void lerstring(char mensagem[], char destino[], int tamanho);
void menubanco(void);
void abrirbanco(void);
void cadastrar_cliente(void);
void abrir_conta(void);
void depositar(void);
void solicitar_emprestimo(void);
void consultar_historico(void);
void efetuarsorteio(void);

#endif
