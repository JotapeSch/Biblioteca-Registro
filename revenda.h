#ifndef REVENDA_H_INCLUDED
#define REVENDA_H_INCLUDED

#define TAM 50

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

    float preco_compra;

    int ativo;              //1 se o carro esta ativo, ou seja, não foi excluído, ou 0 se ele foi excluído
};


struct ENDERECO {
    char rua[TAM];
    int numero;
    char bairro[TAM];
    char cidade[TAM];
    char estado[3];
    char cep[11];   //99.999-999
};


struct TELEFONE {
    char telefone[14];  //99 99999-9999
};


struct CLIENTE {
    int codigo;     //código sequencial, 1, 2, 3, ...
    char nome[TAM];
    struct ENDERECO endereco;
    struct TELEFONE residencial;
    struct TELEFONE comercial[2];
    struct TELEFONE celular[5];
    float renda_mensal;
    int ativo;  //1 se o cliente esta ativo, ou seja, não foi excluído, ou 0 se ele foi excluído
};

struct DATA {
   int dia, mes, ano;
};

struct VENDA_CARRO {
    char placa_carro[9];
    int codigo_cliente;
    float preco_venda;
    struct DATA data_venda;
    int ativo; //1 se a venda esta ativa, ou seja, não foi excluída, ou 0 se ela foi excluída
};


#endif // REVENDA_H_INCLUDED;
