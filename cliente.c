#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "include/configuracao.h"
#include "include/revenda.h"
#include "include/gerador.h"

FILE *abrir_arquivo_clientes(const char *modo) {
    FILE *arquivo = fopen(ARQ_CLIENTE, modo);
    if (!arquivo && strcmp(modo, "rb") == 0)
        arquivo = fopen(ARQ_CLIENTE, "wb+");
    if (!arquivo) {
        perror("Erro ao abrir arquivo de clientes");
        exit(1);
    }
    return arquivo;
}

void InserirClienteAutomatico() {
    Cliente c;
    gerar_dados_clientes(&c);

    FILE *arquivo = abrir_arquivo_clientes("ab");
    fwrite(&c, sizeof(Cliente), 1, arquivo);
    fclose(arquivo);

    printf("Cliente gerado:\n");
    mostrar_cliente(&c);
}

void AlterarRendaCliente() {
    FILE *arquivo = abrir_arquivo_clientes("r+b");
    Cliente c;
    int cod;
    float novaRenda;
    int encontrado = 0;

    printf("Digite o código do cliente: ");
    if (scanf("%d", &cod) != 1) return;

    while (fread(&c, sizeof(Cliente), 1, arquivo)) {
        if (c.codigo == cod && c.ativo == 1) {
            printf("Cliente encontrado: %s (Renda atual: %.2f)\n", c.nome, c.renda);
            printf("Nova renda: ");
            if (scanf("%f", &novaRenda) != 1 || novaRenda <= 0) {
                printf("Renda invalida. Operacao cancelada.\n");
                break;
            }

            c.renda = novaRenda;
            fseek(arquivo, -sizeof(Cliente), SEEK_CUR);
            fwrite(&c, sizeof(Cliente), 1, arquivo);
            printf("Renda atualizada!\n");
            encontrado = 1;
            break;
        }
    }

    if (!encontrado)
        printf("Cliente com código %d nao encontrado\n", cod);

    fclose(arquivo);
}

void MostrarClientesOrdenadosNome() {
    FILE *arquivo = abrir_arquivo_clientes("rb");
    int qtd = 0;

    while (fread(&lista[qtd], sizeof(Cliente), 1, arquivo))
        if (lista[qtd].ativo) qtd++;

    fclose(arquivo);

    if (qtd == 0) {
        printf("Nenhum cliente cadastrado.\n");
        return;
    }
 
    for (int i = 0; i < qtd - 1; i++) { // Ordenacao por nome
        for (int j = i + 1; j < qtd; j++) {
            if (strcmp(lista[i].nome, lista[j].nome) > 0) {
                Cliente tmp = lista[i];
                lista[i] = lista[j];
                lista[j] = tmp;
            }
        }
    }

    printf("\nCLIENTES ORDENADOS POR NOME/\n");
    for (int i = 0; i < qtd; i++)
        printf("Código: %d | Nome: %s | Renda: %.2f\n",
               lista[i].codigo, lista[i].nome, lista[i].renda);
}

int buscar_cliente_por_codigo(int codigo, Cliente *c_out) {
    FILE *arquivo = abrir_arquivo_clientes("rb");
    Cliente c;
    int encontrado = 0;

    while (fread(&c, sizeof(Cliente), 1, arquivo)) {
        if (c.codigo == codigo && c.ativo == 1) {
            if (c_out) *c_out = c;
            encontrado = 1;
            break;
        }
    }

    fclose(arquivo);
    return encontrado;
}
