#include <stdio.h>
#include "revenda.h"
#include "auxiliar.h"
#include "configuracao.h"
#include <string.h>


void InserirVenda();
void ExcluirVenda();
void MostrarCarrosVend_Marca();
void InformarQuantidadeSomaPreco();
void InformarLucroTotal();



FILE *abrir_arquivo(const char *modo_abertura){
    FILE *arquivo = fopen(ARQ_VENDA, modo_abertura);

    if (arquivo == NULL){
        if (strcmp(modo_abertura, "r+b") || strcmp(modo_abertura, "a+b")){
            printf("Não foi possível abrir ou criar o arquivo de vendas.\n");
            perror("Detalhes do erro");
        }
    }

    return arquivo;
}

int carroFoiVendido(const char *placa){
    FILE *arquivo = abrir_arquivo("rb");
    int vendido = 0;

    /*
    implementar
    */

    fclose(arquivo);
    return vendido;
}

/*
    char placa_carro[9];
    int codigo_cliente;
    float preco_venda;
    struct DATA data_venda;
    int ativo; //1 se a venda esta ativa, ou seja, não foi excluída, ou 0 se ela foi excluída
*/
void InserirVenda(){

    struct VENDA_CARRO nova_venda;
    char placa_busca[9];
    int codigo_cliente;

    struct CARRO carro_info;
    struct CLIENTE cliente_info;


    printf("=======================================\n");
    printf("INSERIR VENDA\n");

    printf("Digite a PLACA do carro a ser vendido (AAA-1234): \n");
    if (scanf("%8s", &placa_busca) != 1)
        return;

    if (carroFoiVendido(placa_busca)){
        printf("=======================================\n");
        printf("Carro com a placa %s ja foi vendido!\n", placa_busca);
        return;
    }

    /*
    if (!buscar_carro_por_placa(placa_busca, &carro_info)){             implementar o buscar carro por placa nos auxiliares
        printf("CArro inativo e tals");
        return;
    }
    */
    printf("=======================================\n");
    printf("Digite o CODIGO do cliente comprador: \n");
    if(scanf("%d", &codigo_cliente) != 1){
        return;
    }

    /*
    if (!buscar_cliente_por_codigo(codigo_cliente, &cliente_info)){             implementar o buscar carro por placa nos auxiliares
        printf("cliente nao encontrado");
        return;
    }
    */
   
    printf("=======================================\n");
    printf("Digite o PRECO de venda R$: ");
    do {
        if (scanf("%f", &nova_venda.preco_venda) != 1){
            printf("Preco invalido. Por favor, digite um preco valido e que seja maior que ZERO!\n");
            while (getchar() != '\n' && !feof(stdin));
        }
    } while (nova_venda.preco_venda <= 0);

    printf("=======================================\n");
    printf("Digite a data de venda (dd mm aaaa): \n");
    int aux = 0;

    do {
        if (scanf("%d %d %d", &nova_venda.data_venda.dia, &nova_venda.data_venda.mes, &nova_venda.data_venda.ano) != 3){
            printf("Data invalida. Por favor, digite 3 numeros (dia mes ano):\n");
            while (getchar() != '\n' && !feof(stdin));
        } else {
            aux = 1;
        }

    } while(aux != 1);

    //Gravar as informacoes adquiridas no arquivo
    FILE *arquivo = abrir_arquivo("ab"); //ab eh pra gravacao, sempre escreve no fim do arquivo
    if (arquivo == NULL){
        return;
    }

    if(fwrite(&nova_venda, sizeof(struct VENDA_CARRO), 1, arquivo) != 1){
        printf("ERRO ao gravar a venda no registro!\n");
    } else {
        printf("Venda registrada com sucesso!\n");
        printf("Carro: %s (%s) | Cliente: %s\n", carro_info.modelo, nova_venda.placa_carro, cliente_info.nome);
    }
    fclose(arquivo);
}

void ExcluirVenda(){
    FILE *arquivo = abrir_arquivo("ab");

    fclose(arquivo);
}

struct VENDIDOS_AUX{
    char modelo[TAM];
    char placa[9];
    int ano_fabircacao;
    char nome_cliente[TAM];
};

int comparar_vendidos_modelo(){
    //ver sobre qsort para fazer uma comparacao por modelo crescente
}

void MostrarCarrosVend_Marca(){
    //uso de array estatico de tamanho fixo para ordenacao em memoria, porem ver se tem outras possibilidades.
}

void InformarQuantidadeSomaPreco(){
    FILE *arquivo = abrir_arquivo("rb");

    fclose(arquivo);

}

void InformarLucroTotal(){

    FILE *arquivo = abrir_arquivo("rb");

    fclose(arquivo);
}


