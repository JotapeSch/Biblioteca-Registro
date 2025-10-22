#include <stdio.h>


#include "time.h"
#include "string.h"
#include "include/revenda.h"
#include "include/configuracao.h"
/*
char opcionais[][TAM]={
    {"air.bag"},
    {"banco.couro"},
    {"sensor.ponto.cego"},
    {"cambio.automatico"},
    {"cambio.borboleta"},
    {"controle.estabilidade"},
    {"start.stop"},
    {"camera.360"},
    {"ar.condicionado"},
    {"abs"},
    {"sensor.estacionamento"},
    {"partida.sem.chave"}};


struct CARRO {
    char placa[9];          //AAA-1234

    char marca[TAM];   	    //apenas as marcas: bmw, fiat, chevrolet

    char modelo[TAM];       //apenas os modelos em suas respectivas marcas:
                            //bmw: 320i, x1, x5;
                            //fiat: mobi, toro, uno;
                            //chevrolet: celta, astra, tracker

    int ano_fabricacao;	    //1980 à 2025

    char combustivel[TAM];  //apenas os combustiveis: alcool, gasolina, flex

    char cor[TAM];	        //apenas as cores: branca, prata, preta, cinza, bi-color

    int opcional[12];       //ver matriz opcionais, 1 se tem ou 0 se  não tem o opcional
                            //por exemplo se em opcional tiver: 0 0 0 1 1 0 1 0 0 0 0 0, temos apenas 3 opcionais que são:
                            /*
                                "air.bag"                  --> não tem
                                "banco.couro"              --> não tem
                                "sensor.ponto.cego"        --> não tem
                                "cambio.automatico"        --> tem
                                "cambio.borboleta"         --> tem
                                "controle.estabilidade"    --> não tem
                                "start.stop"               --> tem
                                "camera.360"               --> não tem
                                "ar.condicionado"          --> não tem
                                "abs"                      --> não tem
                                "sensor.estacionamento"    --> não tem
                                "partida.sem.chave"        --> não tem
                            */
/*
    float preco_compra;

    int ativo;              //1 se o carro esta ativo, ou seja, não foi excluído, ou 0 se ele foi excluído
};

*/

char *MARCAS[] = {"bmw", "fiat", "chevrolet"};
const char *MODELOS_BMW[] = {"320i", "x1", "x5"};
const char *MODELOS_FIAT[] = {"mobi", "toro", "uno"};
const char *MODELOS_CHEVROLET[] = {"celta", "astra", "tracker"};
const char *COMBUSTIVEIS[] = {"alcool", "gasolina", "flex"};
const char *CORES[] = {"branca", "prata", "preta", "cinza", "bi-color"};

static void inicializar_rand(){
    static int inicializado = 0; //existe apenas uma vez
    if (!inicializado){
        srand(time(NULL));
        inicializado = 1;
    }
}

int num_marca = 3;

int proximo_codigo_cliente = 1;

//gerador carro
void gerar_placa(char placa[9]);
void gerar_dados_carro(struct CARRO *c);
void mostrar_carro(const struct CARRO *carro);


//gerador cliente
void gerar_dados_clientes(struct CLIENTE *cliente);
void mostrar_cliente();


void gerar_placa(char placa[9]){    //AAA-1234
    inicializar_rand();
    for (int i = 0; i < 3; i++){
        placa[i] = 'A' + (rand() % 26);
    }

    placa[3] = '-';

    for(int i = 4; i < 8; i++){
        placa[i] = '0' + (rand() % 10);
    }

    placa[8] = '\0';
}

//OBJETIVO: Gerar dados do carro 
//PARAMETRO: Registro de um carro
//RETORNO: Nenhum
void gerar_dados_carros(struct CARRO *carro){
    inicializar_rand();

    //GERA A PLACA
    gerar_placa(carro->placa); //gera a placa randomizada

    //GERA MARCA E MODELO
    int id_marca = rand() % 3; //um auxiliar para se locomover na rand
    strcpy(carro->marca, MARCAS[id_marca]); //utilizando o auxiliar, define a marca

    if (strcmp(carro->marca, "bmw") == 0){ //apartir da marca, define o modelo nesse if-else
        strcpy(carro->modelo, MODELOS_BMW[rand() % 3]);
    } else if (strcmp(carro->marca, "fiat") == 0){
        strcpy(carro->modelo, MODELOS_FIAT[rand() % 3]);
    } else if (strcmp(carro->marca, "chevrolet") == 0){
        strcpy(carro->modelo, MODELOS_CHEVROLET[rand() % 3]);
    }

    //GERA ANO DE FABRICACAO
    carro->ano_fabricacao = 1980 + (rand() % (2025 - 1980 + 1));

    //GERA COMBUSTIVEL
    strcpy(carro->combustivel, COMBUSTIVEIS[rand() % 3]);

    //GERA CORES
    strcpy(carro->cor, CORES[rand() % 5]);
    
    //GERADOR DE OPCIONAIS
    for (int i = 0; i < 12; i++){
        carro->opcional[i]= rand() % 2;
    }

    //gerar preco
    //(entre 10000.00 e 500000.00)
    carro->preco_compra = (float)(10000 + rand() % 490001) + (float)(rand() % 100) / 100.0f;

    carro->ativo = 1;
}

//OBJETIVO: Mostrar certo carro, dado o parametro
//PARAMETROS: Um registro de certo carro
//RETORNO: Nenhum
void mostrar_carro(const struct CARRO *carro){
    printf("Placa: %s | Marca: %s | Modelo: %s\n", carro->placa, carro->marca, carro->modelo);
    printf("Ano: %s | Combustivel: %s | Cor: %s\n", carro->ano_fabricacao, carro->combustivel, carro->cor);
    printf("Preco compra: R$ %.2f\n", carro->preco_compra);
    printf("Opcionais presentes: \n");
    int achou = 0;
    for (int i = 0; i < 12; i++){
        if(carro->opcional[i] == 1){
            printf(" - %s\n", opcionais[i]);
            achou = 1;
        }
    }

    if (!achou){
        printf("Nenhum\n");
    }

}

 /*
    int codigo;     //código sequencial, 1, 2, 3, ...
    char nome[TAM];
    struct ENDERECO endereco;
    struct TELEFONE residencial;
    struct TELEFONE comercial[2];
    struct TELEFONE celular[5];
    float renda_mensal;
    int ativo;
    */

    /*
    char rua[TAM];
    int numero;
    char bairro[TAM];
    char cidade[TAM];
    char estado[3];
    char cep[11];   //99.999-999
    */

//GERAR O CLIENTE
void gerar_dados_clientes(struct CLIENTE *cliente){
    inicializar_rand();

    FILE *arq = fopen(ARQ_CLIENTE, "rb");
    if (arq != NULL) {
        struct CLIENTE temp;
        
        if (fseek(arq, -sizeof(struct CLIENTE), SEEK_END) == 0 && fread(&temp , sizeof(struct CLIENTE), 1, arq) == 1){
            proximo_codigo_cliente = temp.codigo + 1;
        }
        fclose(arq);
    }

    cliente->codigo = proximo_codigo_cliente;

    const char *NOMES[] = {"Joao Pedro Martins", "Ryan Campos", "Vinicius Amancio"};
    const char *BAIRRO[] = {"CENTRO", "Sibipiruna", "Flamboyant"};
    const char *RUA[] = {"Avenida 8", "Teiji Matsui", "Major Pedro H. Cavalcante", "Rua 12", "Rua P9"};
    const char *CIDADE[] = {"Dourados", "Chapadao do Sul", "Vicentina"};
    const char *CEP[11] = {"79.560-000", "79.823-640", "79.710-000"};
    const char *ESTADO[] = {"MS"};
    int num_nome = 3;
    int aux_nomes = rand() % 3;

    //randomiza o nome
    strcpy(cliente->nome, NOMES[rand() % 3]);
    strcpy(cliente->endereco.cidade, BAIRRO[aux_nomes]);
    strcpy(cliente->endereco.cep, CEP[aux_nomes]);
    strcpy(cliente->endereco.rua, BAIRRO[rand() % 3]);
    strcpy(cliente->endereco.bairro, BAIRRO[rand() % 3]);
    strcpy(cliente->endereco.estado, ESTADO[0]);

    cliente->ativo = 1;
    proximo_codigo_cliente++;
}

void mostrar_cliente(const struct CLIENTE *cli) {
    printf("Codigo: %d\n", cli->codigo);
    printf("Nome: %s\n", cli->nome);
    printf("Renda Mensal: R$ %.2f\n", cli->renda_mensal);
    printf("Endereco: %s, %d - %s/%s\n", cli->endereco.rua, cli->endereco.numero, cli->endereco.cidade, cli->endereco.estado);
}
