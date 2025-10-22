#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "include/configuracao.h"
#include "include/revenda.h"
#include "include/gerador.h"


//acredito que isso vai facilitar Ryan
FILE *abrir_arquivo_clientes(const char *modo_abertura){
    FILE *arquivo = fopen(ARQ_CLIENTE, modo_abertura);
    if (arquivo == NULL){
        if (strcmp(modo_abertura, "r+b") == 0 || strcmp(modo_abertura, "a+b") == 0){
            printf("Não foi possível abrir ou criar o arquivo de clientes.\n");
            perror("Detalhes do erro");
        }
    }

    return arquivo;
}

void InserirCliente(){
    FILE *arquivo = abrir_arquivo_clientes("ab");
    //implemente aqui
    fclose(arquivo);

}
void AlterarRendaCliente(){
    FILE *arquivo = abrir_arquivo_clientes("r+b");

    fclose(arquivo);
}
void MostrarClientesOrdenadosNome(){
    FILE *arquivo = abrir_arquivo_clientes("rb");

    fclose(arquivo);
}
int buscar_cliente_por_codigo(int codigo){
    FILE *arquivo = abrir_arquivo_clientes("rb");

    fclose(arquivo);
}
static int comparar_cliente_nome(){}


/*
//usei a logica do tb banco
#define MAX_CLIENTES 100 // n especificou valor no pdf, pode alterar jotinha 

Cliente clientes[MAX_CLIENTES]; 
int qtdClientes = 0;


void InserirCliente(){
   if (qtdClientes >= MAX_CLIENTES) {
        printf("Limite máximo de clientess\n");
        return;
    }

    Cliente c;
    printf("Código: ");
    scanf("%d", &c.codigo); // facilitar pra achar o cliente
    printf("Nome: ");
    scanf(" %[^\n]", c.nome); // %[^\n] serve para ler tudo ate o cara apertar enter
    printf("Renda: ");
    scanf("%f", &c.renda);

    clientes[qtdClientes] = c;
    qtdClientes++;

    printf("Cliente inserido");
}  
void AlterarRendaCliente(){
    if (qtdClientes == 0) {
        printf("Nenhum cliente cadastrado!\n");
        return;
    }

    int cod;
    float novaRenda;

    printf("Digite o código do cliente: ");
    scanf("%d", &cod);

    int encontrado = 0; 
    for (int i = 0; i < qtdClientes; i++) {
        if (clientes[i].codigo == cod) {
            printf("Cliente encontrado: %s (Renda atual: %.2f)\n",
                   clientes[i].nome, clientes[i].renda);
            printf("Nova renda: ");
            scanf("%f", &novaRenda);
            clientes[i].renda = novaRenda;
            printf("Renda atualizada\n");
            encontrado = 1;// controle de parada
            break;
        } // da pra fazer a msm logica do banco para ser um menu com opcao de saida ou cancelar... ai posso alterar
    }   
    if (!encontrado) {
        printf("Cliente com código %d não encontrado!\n", cod);
    }
}
void MostrarClientesOrdenadosNome(){
    if (qtdClientes == 0) {
        printf("Nenhum cliente cadastrado!\n");
        return;
    }
 
    Cliente copia[MAX_CLIENTES]; // Cria uma cópia
    for (int i = 0; i < qtdClientes; i++) {
        copia[i] = clientes[i];
    }

    for (int i = 0; i < qtdClientes - 1; i++) {// mostrar por nome
        for (int j = i + 1; j < qtdClientes; j++) {
            if (strcmp(copia[i].nome, copia[j].nome) > 0) {
                Cliente temp = copia[i];
                copia[i] = copia[j];
                copia[j] = temp;
            }
        }
    }

    printf(" CLIENTES ORDENADOS POR NOME\n");
    for (int i = 0; i < qtdClientes; i++) {
        printf("Código: %d | Nome: %s | Renda: %.2f\n",
               copia[i].codigo, copia[i].nome, copia[i].renda);
    }
} 
    */
