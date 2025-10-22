#include <stdio.h>
#include "revenda.h"
#include <configuracao.h>

FILE *abrir_arquivo_carro(const char *modo_abertura){
    FILE *arquivo = fopen(ARQ_CLIENTE, modo_abertura);
    if (arquivo == NULL){
        if (strcmp(modo_abertura, "r+b") || strcmp(modo_abertura, "a+b")){
            printf("Não foi possível abrir ou criar o arquivo de clientes.\n");
            perror("Detalhes do erro: ");
        }
    }

    return arquivo;
}


void InserirCarro(){
    FILE *arquivo = abrir_arquivo_carro("ab");

    fclose(arquivo);
}


void ExcluirCarro(){
    FILE *arquivo = abrir_arquivo_carro("r+b");

    fclose(arquivo);
}

void MostrarCarroDisp_marca_modelo(){
    FILE *arquivo = abrir_arquivo_carro("rb");

    fclose(arquivo);
}

int MostrarCarroDispo_selecao(){
    FILE *arquivo = abrir_arquivo_carro("rb");

    fclose(arquivo);
}


int buscar_carro_por_placa(const char *placa){
    FILE *arquivo = abrir_arquivo_carro("rb");

    fclose (arquivo);
}

int comparar_carro_marca_modelo(){}

