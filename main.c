#include "revenda.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

int comparar_carro_marca_modelo(const void *a, const void *b);

// 1. PROTÓTIPOS - MÓDULO CARRO (carro.c)
// ===================================
void InserirCarro();
void ExcluirCarro();
void MostrarCarroDisp_marca_modelo();
int MostrarCarroDispo_selecao();

// ===================================
// 2. PROTÓTIPOS - MÓDULO CLIENTE (cliente.c)
// ===================================
void InserirCliente();
void AlterarRendaCliente();
void MostrarClientesOrdenadosNome();

// ===================================
// 3. PROTÓTIPOS - MÓDULO VENDA (venda.c)
// ===================================
void InserirVenda();
void ExcluirVenda();
void MostrarCarrosVend_marca();
void InformarQuantidadeSomaPreco();
void InformarLucroTotal();

// 4. PROTÓTIPOS - FUNÇÕES DO PRÓPRIO MAIN.C (Se existirem)
// (Suas funções de menu)
void MenuCarro();
void MenuCliente();
void MenuVenda();
void mostrarMenu();



//CONFIGURACAO

#define ARQ_CARRO "carros.bin"
#define ARQ_CLIENTE "clientes.bin"
#define ARQ_VENDA "vendas.bin"
#define MAX_REGISTRO 500


//auxiliares
// Funções de Busca
int buscar_carro_por_placa(const char *placa, struct CARRO *c_out);
int buscar_cliente_por_codigo(int codigo, struct CLIENTE *cli_out);

// Funções de Checagem
int carro_foi_vendido(const char *placa);

// Funções de Geração (Para uso no código)
void gerar_dados_carro(struct CARRO *c);
void mostrar_carro(const struct CARRO *carro);
void gerar_dados_clientes(struct CLIENTE *cliente);
void mostrar_cliente(const struct CLIENTE *cli);

// Funções de Menu
void InserirCarro();
void ExcluirCarro();
void MostrarCarroDisp_marca_modelo();
int MostrarCarroDispo_selecao();

void InserirCliente();
void AlterarRendaCliente();
void MostrarClientesOrdenadosNome();

void InserirVenda();
void ExcluirVenda();
void MostrarCarrosVend_marca();
void InformarQuantidadeSomaPreco();
void InformarLucroTotal();

void MenuCarro();
void MenuCliente();
void MenuVenda();
void mostrarMenu();

//GERADOR
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
    printf("Ano: %d | Combustivel: %s | Cor: %s\n", carro->ano_fabricacao, carro->combustivel, carro->cor);
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
    int aux_nomes = rand() % 3;

    //randomiza o nome
    strcpy(cliente->nome, NOMES[rand() % 3]);
    strcpy(cliente->endereco.cidade, CIDADE[aux_nomes]);
    strcpy(cliente->endereco.cep, CEP[aux_nomes]);
    strcpy(cliente->endereco.rua, RUA[rand() % 3]);
    strcpy(cliente->endereco.bairro, BAIRRO[rand() % 3]);
    strcpy(cliente->endereco.estado, ESTADO[0]);
    cliente->renda_mensal = rand() % 10000;

    cliente->ativo = 1;
    proximo_codigo_cliente++;
}

void mostrar_cliente(const struct CLIENTE *cli) {
    printf("Codigo: %d\n", cli->codigo);
    printf("Nome: %s\n", cli->nome);
    printf("Renda Mensal: R$ %.2f\n", cli->renda_mensal);
    printf("Endereco: %s, %d - %s/%s\n", cli->endereco.rua, cli->endereco.numero, cli->endereco.cidade, cli->endereco.estado);
}

//auxiliares
static FILE *abrir_arquivo(const char *nome_arq, const char *modo_abertura) {
    FILE *arq = fopen(nome_arq, modo_abertura);
    if (arq == NULL) {
        if (strcmp(modo_abertura, "rb+") == 0 || strcmp(modo_abertura, "ab") == 0) {
            perror("Erro ao abrir/criar arquivo");
        }
    }
    return arq;
}

// Implementação da busca de carro
int buscar_carro_por_placa(const char *placa, struct CARRO *c_out) {
    FILE *arquivo = abrir_arquivo(ARQ_CARRO, "rb");
    struct CARRO carro_atual;
    int encontrado = 0;

    if (arquivo == NULL) return 0;
    while (fread(&carro_atual, sizeof(struct CARRO), 1, arquivo) == 1) {
        if (strcmp(carro_atual.placa, placa) == 0 && carro_atual.ativo == 1) {
            *c_out = carro_atual; 
            encontrado = 1;
            break;
        }
    }
    fclose(arquivo);
    return encontrado;
}

// Implementação da busca de cliente
int buscar_cliente_por_codigo(int codigo, struct CLIENTE *cli_out);


//CARRO
FILE *abrir_arquivo_carro(const char *modo_abertura){
    FILE *arquivo = fopen(ARQ_CARRO, modo_abertura);
    if (arquivo == NULL){
        if (strcmp(modo_abertura, "r+b") == 0 || strcmp(modo_abertura, "a+b") == 0){
            printf("Não foi possível abrir ou criar o arquivo de carros.\n");
            perror("Detalhes do erro: ");
        }
    }
    return arquivo;
}

void InserirCarro(){
    FILE *arquivo = abrir_arquivo_carro("a+b");
    if (arquivo == NULL) {
        return;
    }

    struct CARRO novo_carro;
    novo_carro.ativo = 1; // Carro ativo ao inserir

    printf("=======================================\n");
    printf("INSERIR CARRO\n");

    // Placa (AAA-1234)
    printf("Digite a PLACA do carro (AAA-1234): \n");
    if (scanf("%8s", novo_carro.placa) != 1) {
        fclose(arquivo);
        return;
    }
    // Validar formato da placa
    if (strlen(novo_carro.placa) != 8 || 
        !isalpha(novo_carro.placa[0]) || !isalpha(novo_carro.placa[1]) || !isalpha(novo_carro.placa[2]) ||
        novo_carro.placa[3] != '-' ||
        !isdigit(novo_carro.placa[4]) || !isdigit(novo_carro.placa[5]) || 
        !isdigit(novo_carro.placa[6]) || !isdigit(novo_carro.placa[7])) {
        printf("Placa inválida! Use o formato AAA-1234.\n");
        fclose(arquivo);
        return;
    }

    // Verificar se a placa já existe
    struct CARRO temp;
    rewind(arquivo);
    while (fread(&temp, sizeof(struct CARRO), 1, arquivo) == 1) {
        if (strcmp(temp.placa, novo_carro.placa) == 0 && temp.ativo) {
            printf("Carro com placa %s já existe!\n", novo_carro.placa);
            fclose(arquivo);
            return;
        }
    }

    // Marca
    printf("Digite a MARCA do carro (bmw, fiat, chevrolet): \n");
    scanf(" %[^\n]", novo_carro.marca);
    if (strcmp(novo_carro.marca, "bmw") != 0 && 
        strcmp(novo_carro.marca, "fiat") != 0 && 
        strcmp(novo_carro.marca, "chevrolet") != 0) {
        printf("Marca inválida! Use apenas: bmw, fiat, chevrolet.\n");
        fclose(arquivo);
        return;
    }

    // Modelo
    printf("Digite o MODELO do carro: \n");
    scanf(" %[^\n]", novo_carro.modelo);
    if (strcmp(novo_carro.marca, "bmw") == 0 && 
        strcmp(novo_carro.modelo, "320i") != 0 && 
        strcmp(novo_carro.modelo, "x1") != 0 && 
        strcmp(novo_carro.modelo, "x5") != 0) {
        printf("Modelo invalido para BMW! Use apenas: 320i, x1, x5.\n");
        fclose(arquivo);
        return;
    }
    if (strcmp(novo_carro.marca, "fiat") == 0 && 
        strcmp(novo_carro.modelo, "mobi") != 0 && 
        strcmp(novo_carro.modelo, "toro") != 0 && 
        strcmp(novo_carro.modelo, "uno") != 0) {
        printf("Modelo invalido para Fiat! Use apenas: mobi, toro, uno.\n");
        fclose(arquivo);
        return;
    }
    if (strcmp(novo_carro.marca, "chevrolet") == 0 && 
        strcmp(novo_carro.modelo, "celta") != 0 && 
        strcmp(novo_carro.modelo, "astra") != 0 && 
        strcmp(novo_carro.modelo, "tracker") != 0) {
        printf("Modelo invalido para Chevrolet! Use apenas: celta, astra, tracker.\n");
        fclose(arquivo);
        return;
    }

    // Ano de fabricação
    printf("Digite o ANO de fabricação (1980-2025): \n");
    if (scanf("%d", &novo_carro.ano_fabricacao) != 1 || 
        novo_carro.ano_fabricacao < 1980 || novo_carro.ano_fabricacao > 2025) {
        printf("Ano invalido! Deve ser entre 1980 e 2025.\n");
        fclose(arquivo);
        return;
    }

    // Combustível
    printf("Digite o COMBUSTÍVEL do carro (alcool, gasolina, flex): \n");
    scanf(" %[^\n]", novo_carro.combustivel);
    if (strcmp(novo_carro.combustivel, "alcool") != 0 && 
        strcmp(novo_carro.combustivel, "gasolina") != 0 && 
        strcmp(novo_carro.combustivel, "flex") != 0) {
        printf("Combustivel invalido! Use apenas: alcool, gasolina, flex.\n");
        fclose(arquivo);
        return;
    }

    // Cor
    printf("Digite a COR do carro (branca, prata, preta, cinza, bi-color): \n");
    scanf(" %[^\n]", novo_carro.cor);
    if (strcmp(novo_carro.cor, "branca") != 0 && 
        strcmp(novo_carro.cor, "prata") != 0 && 
        strcmp(novo_carro.cor, "preta") != 0 && 
        strcmp(novo_carro.cor, "cinza") != 0 && 
        strcmp(novo_carro.cor, "bi-color") != 0) {
        printf("Cor invalida! Use apenas: branca, prata, preta, cinza, bi-color.\n");
        fclose(arquivo);
        return;
    }

    // Preço de compra
    printf("Digite o PRECO de compra do carro (R$): \n");
    if (scanf("%f", &novo_carro.preco_compra) != 1 || novo_carro.preco_compra <= 0) {
        printf("Preço invalido! Deve ser maior que zero.\n");
        fclose(arquivo);
        return;
    }

    // Opcionais
    printf("Digite os OPCIONAIS do carro (0 ou 1 para cada, na ordem):\n");
    printf("air.bag, banco.couro, sensor.ponto.cego, cambio.automatico, cambio.borboleta, ");
    printf("controle.estabilidade, start.stop, camera.360, ar.condicionado, abs, ");
    printf("sensor.estacionamento, partida.sem.chave\n");
    for (int i = 0; i < 12; i++) {
        int valor;
        if (scanf("%d", &valor) != 1 || (valor != 0 && valor != 1)) {
            printf("Opcional inválido! Use apenas 0 ou 1.\n");
            fclose(arquivo);
            return;
        }
        novo_carro.opcional[i] = valor;
    }

    if (fwrite(&novo_carro, sizeof(struct CARRO), 1, arquivo) != 1) {
        printf("Erro ao gravar o carro no registro!\n");
    } else {
        printf("Carro registrado com sucesso!\n");
        printf("Placa: %s | Marca: %s | Modelo: %s | Ano: %d | Combustível: %s | Cor: %s | Preço: R$%.2f\n",
               novo_carro.placa, novo_carro.marca, novo_carro.modelo, novo_carro.ano_fabricacao,
               novo_carro.combustivel, novo_carro.cor, novo_carro.preco_compra);
    }

    fclose(arquivo);
}

void ExcluirCarro(){
    FILE *arquivo = abrir_arquivo_carro("r+b");
    if (arquivo == NULL) {
        return;
    }

    char placa[9];
    printf("=======================================\n");
    printf("EXCLUIR CARRO\n");
    printf("Digite a PLACA do carro a ser excluído (AAA-1234): \n");
    if (scanf("%8s", placa) != 1) {
        fclose(arquivo);
        return;
    }

    // Validar formato da placa
    if (strlen(placa) != 8 || 
        !isalpha(placa[0]) || !isalpha(placa[1]) || !isalpha(placa[2]) ||
        placa[3] != '-' ||
        !isdigit(placa[4]) || !isdigit(placa[5]) || 
        !isdigit(placa[6]) || !isdigit(placa[7])) {
        printf("Placa inválida! Use o formato AAA-1234.\n");
        fclose(arquivo);
        return;
    }

    struct CARRO carro;
    int encontrado = 0;
    long posicao;
    rewind(arquivo);
    while (fread(&carro, sizeof(struct CARRO), 1, arquivo) == 1) {
        if (strcmp(carro.placa, placa) == 0 && carro.ativo) {
            encontrado = 1;
            carro.ativo = 0; // Marca como excluído
            posicao = ftell(arquivo) - sizeof(struct CARRO);
            fseek(arquivo, posicao, SEEK_SET);
            if (fwrite(&carro, sizeof(struct CARRO), 1, arquivo) != 1) {
                printf("Erro ao excluir o carro!\n");
            } else {
                printf("Carro com placa %s excluído com sucesso!\n", placa);
            }
            break;
        }
    }

    if (!encontrado) {
        printf("Carro com placa %s não encontrado ou já excluído!\n", placa);
    }

    fclose(arquivo);
}

void MostrarCarroDisp_marca_modelo(){
    FILE *arquivo = abrir_arquivo_carro("rb");
    if (arquivo == NULL) {
        return;
    }

    // Contar carros ativos
    struct CARRO carro;
    int count = 0;
    rewind(arquivo);
    while (fread(&carro, sizeof(struct CARRO), 1, arquivo) == 1) {
        if (carro.ativo) {
            count++;
        }
    }

    if (count == 0) {
        printf("Nenhum carro disponível para venda!\n");
        fclose(arquivo);
        return;
    }

    // Alocar array para carros ativos
    struct CARRO *carros = malloc(count * sizeof(struct CARRO));
    if (carros == NULL) {
        printf("Erro de alocação de memória!\n");
        fclose(arquivo);
        return;
    }

    // Ler carros ativos
    rewind(arquivo);
    int index = 0;
    while (fread(&carro, sizeof(struct CARRO), 1, arquivo) == 1 && index < count) {
        if (carro.ativo) {
            carros[index] = carro;
            index++;
        }
    }

    // Ordenar por marca e modelo
    qsort(carros, count, sizeof(struct CARRO), comparar_carro_marca_modelo);

    // Exibir carros
    printf("=======================================\n");
    printf("CARROS DISPONÍVEIS (ORDENADOS POR MARCA E MODELO)\n");
    for (int i = 0; i < count; i++) {
        printf("Placa: %s | Marca: %s | Modelo: %s | Ano: %d | Combustível: %s | Cor: %s | Preço: R$%.2f | Opcionais: ",
               carros[i].placa, carros[i].marca, carros[i].modelo, carros[i].ano_fabricacao,
               carros[i].combustivel, carros[i].cor, carros[i].preco_compra);
        for (int j = 0; j < 12; j++) {
            if (carros[i].opcional[j]) {
                switch (j) {
                    case 0: printf("air.bag "); break;
                    case 1: printf("banco.couro "); break;
                    case 2: printf("sensor.ponto.cego "); break;
                    case 3: printf("cambio.automatico "); break;
                    case 4: printf("cambio.borboleta "); break;
                    case 5: printf("controle.estabilidade "); break;
                    case 6: printf("start.stop "); break;
                    case 7: printf("camera.360 "); break;
                    case 8: printf("ar.condicionado "); break;
                    case 9: printf("abs "); break;
                    case 10: printf("sensor.estacionamento "); break;
                    case 11: printf("partida.sem.chave "); break;
                }
            }
        }
        printf("\n");
    }

    free(carros);
    fclose(arquivo);
}

int MostrarCarroDispo_selecao(){
    FILE *arquivo = abrir_arquivo_carro("rb");
    if (arquivo == NULL) {
        return 0;
    }

    int opcionais[12] = {0};
    printf("=======================================\n");
    printf("MOSTRAR CARROS POR OPCIONAIS\n");
    printf("Selecione os opcionais desejados (0 ou 1 para cada, na ordem):\n");
    printf("air.bag, banco.couro, sensor.ponto.cego, cambio.automatico, cambio.borboleta, ");
    printf("controle.estabilidade, start.stop, camera.360, ar.condicionado, abs, ");
    printf("sensor.estacionamento, partida.sem.chave\n");
    for (int i = 0; i < 12; i++) {
        int valor;
        if (scanf("%d", &valor) != 1 || (valor != 0 && valor != 1)) {
            printf("Opcional inválido! Use apenas 0 ou 1.\n");
            fclose(arquivo);
            return 0;
        }
        opcionais[i] = valor;
    }

    struct CARRO carro;
    int encontrados = 0;
    rewind(arquivo);
    printf("=======================================\n");
    printf("Carros disponíveis com os opcionais selecionados:\n");
    while (fread(&carro, sizeof(struct CARRO), 1, arquivo) == 1) {
        if (carro.ativo) {
            int todos_presentes = 1;
            for (int i = 0; i < 12; i++) {
                if (opcionais[i] == 1 && carro.opcional[i] == 0) {
                    todos_presentes = 0;
                    break;
                }
            }
            if (todos_presentes) {
                printf("Placa: %s | Marca: %s | Modelo: %s | Ano: %d | Combustível: %s | Cor: %s | Preço: R$%.2f | Opcionais: ",
                       carro.placa, carro.marca, carro.modelo, carro.ano_fabricacao,
                       carro.combustivel, carro.cor, carro.preco_compra);
                for (int j = 0; j < 12; j++) {
                    if (carro.opcional[j]) {
                        switch (j) {
                            case 0: printf("air.bag "); break;
                            case 1: printf("banco.couro "); break;
                            case 2: printf("sensor.ponto.cego "); break;
                            case 3: printf("cambio.automatico "); break;
                            case 4: printf("cambio.borboleta "); break;
                            case 5: printf("controle.estabilidade "); break;
                            case 6: printf("start.stop "); break;
                            case 7: printf("camera.360 "); break;
                            case 8: printf("ar.condicionado "); break;
                            case 9: printf("abs "); break;
                            case 10: printf("sensor.estacionamento "); break;
                            case 11: printf("partida.sem.chave "); break;
                        }
                    }
                }
                printf("\n");
                encontrados++;
            }
        }
    }

    if (encontrados == 0) {
        printf("Nenhum carro encontrado com os opcionais especificados!\n");
    }

    fclose(arquivo);
    return encontrados;
}


int comparar_carro_marca_modelo(const void *a, const void *b) {
    struct CARRO *carro_a = (struct CARRO *)a;
    struct CARRO *carro_b = (struct CARRO *)b;
    int cmp = strcmp(carro_a->marca, carro_b->marca);
    if (cmp == 0) {
        cmp = strcmp(carro_a->modelo, carro_b->modelo);
    }
    return cmp;
}


//CLIENTE
static FILE *abrir_arquivo_clientes(const char *modo_abertura) {
    FILE *arquivo = fopen(ARQ_CLIENTE, modo_abertura);
    if (arquivo == NULL) {
        // Reporta erro apenas para modos de escrita/alteração
        if (strcmp(modo_abertura, "rb+") == 0 || strcmp(modo_abertura, "ab") == 0) {
             printf("Não foi possível abrir ou criar o arquivo de clientes.\n");
             perror("Detalhes do erro");
        }
    }
    return arquivo;
}



void InserirCliente() {
    struct CLIENTE novo_cliente;
    FILE *arquivo;
    
    // CORREÇÃO: Usar a geração automática, conforme requisito do PDF
    gerar_dados_clientes(&novo_cliente); 

    printf("=======================================\n");
    printf("Cliente gerado automaticamente (Cod %d):\n", novo_cliente.codigo);
    mostrar_cliente(&novo_cliente); 
    printf("=======================================\n");

    if (novo_cliente.renda_mensal <= 0) {
        printf("ERRO: Renda mensal invalida. Insercao cancelada.\n");
        return;
    }
    
    // CORREÇÃO: Abre o arquivo binário para salvar
    arquivo = abrir_arquivo_clientes("ab"); 
    if (arquivo == NULL) return;

    if (fwrite(&novo_cliente, sizeof(struct CLIENTE), 1, arquivo) != 1) {
        printf("Erro ao gravar o cliente no arquivo.\n");
    } else {
        printf("Cliente com codigo %d inserido com sucesso!\n", novo_cliente.codigo);
    }

    fclose(arquivo);
}


void AlterarRendaCliente() {
    FILE *arquivo = abrir_arquivo_clientes("rb+"); // r+b para ler e reescrever
    struct CLIENTE c;
    int cod;
    float novaRenda;
    int encontrado = 0;

    if (arquivo == NULL) {
        printf("Nenhum cliente cadastrado no arquivo.\n");
        return;
    }

    printf("Digite o código do cliente: ");
    if (scanf("%d", &cod) != 1) {
        fclose(arquivo);
        return;
    }

    long posicao_atual = 0;
    while (fread(&c, sizeof(struct CLIENTE), 1, arquivo)) {
        if (c.codigo == cod && c.ativo == 1) {
            
            
            printf("Cliente encontrado: %s (Renda atual: %.2f)\n", c.nome, c.renda_mensal);
            printf("Nova renda: ");
            
            if (scanf("%f", &novaRenda) != 1 || novaRenda <= 0) {
                printf("Renda invalida ou menor/igual a zero. Operacao cancelada.\n");
                encontrado = 1; 
                break;
            }

            c.renda_mensal = novaRenda;
            
            
            fseek(arquivo, posicao_atual, SEEK_SET); 
            
            // Reescreve o registro atualizado (Persistência)
            if (fwrite(&c, sizeof(struct CLIENTE), 1, arquivo) != 1) {
                printf("Erro ao atualizar a renda no arquivo!\n");
            } else {
                printf("Renda atualizada para R$ %.2f!\n", novaRenda);
            }
            encontrado = 1;
            break;
        }
        // Guarda a posição do registro ANTES de ler o próximo
        posicao_atual = ftell(arquivo); 
    }

    if (!encontrado)
        printf("Cliente com código %d nao encontrado ou inativo.\n", cod);

    fclose(arquivo);
}



static int comparar_cliente_nome(const void *a, const void *b) {
    const struct CLIENTE *cliA = (const struct CLIENTE *)a;
    const struct CLIENTE *cliB = (const struct CLIENTE *)b;
    return strcmp(cliA->nome, cliB->nome);
}

void MostrarClientesOrdenadosNome() {
    FILE *arquivo = abrir_arquivo_clientes("rb");
    
    
    struct CLIENTE clientes_em_memoria[MAX_REGISTRO]; 
    int qtd = 0;

    if (arquivo == NULL) {
        printf("Nenhum cliente cadastrado no arquivo.\n");
        return;
    }

    
    while (fread(&clientes_em_memoria[qtd], sizeof(struct CLIENTE), 1, arquivo) == 1) {
        if (clientes_em_memoria[qtd].ativo) {
            if (qtd >= MAX_REGISTRO) {
                printf("AVISO: Limite de %d registros na memoria atingido.\n", MAX_REGISTRO);
                break; 
            }
            qtd++; 
        }
    }
    fclose(arquivo);

    if (qtd == 0) {
        printf("Nenhum cliente ativo cadastrado.\n");
        return;
    }
    
    
    qsort(clientes_em_memoria, qtd, sizeof(struct CLIENTE), comparar_cliente_nome);
    
    printf("\nCLIENTES ORDENADOS POR NOME (Total: %d)\n", qtd);
    printf("-------------------------------------------------\n");
    printf("%-6s | %-40s | %-12s\n", "Codigo", "Nome", "Renda Mensal");
    printf("-------------------------------------------------\n");
    
    for (int i = 0; i < qtd; i++)
        
        printf("%-6d | %-40s | R$ %.2f\n",
               clientes_em_memoria[i].codigo, clientes_em_memoria[i].nome, clientes_em_memoria[i].renda_mensal);
    printf("-------------------------------------------------\n");
}




int buscar_cliente_por_codigo(int codigo, struct CLIENTE *c_out) {
    FILE *arquivo = abrir_arquivo_clientes("rb");
    struct CLIENTE c;
    int encontrado = 0;

    if (arquivo == NULL) return 0;

    while (fread(&c, sizeof(struct CLIENTE), 1, arquivo)) {
        if (c.codigo == codigo && c.ativo == 1) {
            // Copia os dados para o endereço de saída fornecido (SEM MALLOC)
            if (c_out) *c_out = c; 
            encontrado = 1;
            break;
        }
    }

    fclose(arquivo);
    return encontrado;
}

//venda
int carro_foi_vendido(const char *placa){
    FILE *arquivo = abrir_arquivo(ARQ_CARRO, "rb");
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
    if (scanf("%8s", placa_busca) != 1)
        return;

    if (carro_foi_vendido(placa_busca)){
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
    FILE *arquivo = abrir_arquivo(ARQ_VENDA, "ab"); //ab eh pra gravacao, sempre escreve no fim do arquivo
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
    FILE *arquivo = abrir_arquivo(ARQ_VENDA, "ab");

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
    FILE *arquivo = abrir_arquivo(ARQ_VENDA, "rb");

    fclose(arquivo);

}

void InformarLucroTotal(){

    FILE *arquivo = abrir_arquivo(ARQ_VENDA, "rb");

    fclose(arquivo);
}


void MenuCarro(){
    char opcao;
    do{
        printf("=======================================\n");
        printf("Menu Carro\n");
        printf("=======================================\n");
        printf("a. Inserir um carro no cadastro\n");
        printf("b. Excluir carro no cadastro\n");
        printf("c. Mostrar carros disponiveis p/ venda por marca e modelo crescentemente\n");
        printf("d. Mostrar os carros disp. p/ venda por selecao de uma ou mais opcao\n");
        printf("e. sair\n");
        printf("=======================================\n");
        printf("Escolha uma opcao:\n");
        if (scanf (" %c", &opcao) != 1){
            printf("=======================================\n"); 
            printf("Entrada invalida! Por favor digite uma opcao que esta no Menu Carro\n");
            continue;
        }

        switch (opcao)
        {
        case 'a':
            printf("=======================================\n");
            printf("INSERIR CARRO\n");
            InserirCarro();
            break;

        case 'b':
            printf("=======================================\n");
            printf("EXCLUIR CARRO\n");
            ExcluirCarro();
            break;

        case 'c':
            printf("=======================================\n");
            printf("MOSTRAR CARROS DISPONIVEIS EM ORDEM CRESCENTE POR MARCA E MODELO\n");
            MostrarCarroDisp_marca_modelo();
            break;

        case 'd':
            printf("=======================================\n");
            printf("MOSTRAR CARROS DISPONIVEIS POR SELECAO DE UMA OU MAIS OPCAO\n");
            MostrarCarroDispo_selecao();
            break;

        case 'e':
            printf("=======================================\n");
            printf("Saindo...\n");
            break;
        
        default:
            printf("=======================================\n");
            printf("OPCAO INEXISTENTE\n");
            break;
        }
    } while (opcao != 'e');
}

void MenuCliente(){
    char opcao;
    do{
        printf("=======================================\n");
        printf("Menu Cliente\n");
        printf("=======================================\n");
        printf("a. Inserir cliente no cadastro\n");
        printf("b. Alterar renda mensal do cliente\n");
        printf("c. Mostrar clientes ordenadas crescentemente por nome\n");
        printf("d. sair\n");
        printf("=======================================\n");
        printf("Escolha uma opcao:\n");
        if (scanf (" %c", &opcao) != 1){
            printf("Entrada invalida! Por favor digite uma opcao que esta no Menu Cliente\n");
            continue;
        }

        switch (opcao)
        {
        case 'a':
            printf("=======================================\n");
            printf("INSERIR CLIENTE\n");
            InserirCliente();
            break;

        case 'b':
            printf("=======================================\n");
            printf("ALTERAR RENDA MENSAL DO CLIENTE\n");
            AlterarRendaCliente();
            break;

        case 'c':
            printf("=======================================\n");
            printf("MOSTRAR CLIENTES EM ORDEM ALFABETICA\n");
            MostrarClientesOrdenadosNome();
            break;

        case 'd':
            printf("=======================================\n");
            printf("Saindo...\n");
            break;
        
        default:
            printf("=======================================\n");
            printf("OPCAO INEXISTENTE\n");
            break;
        }
    } while (opcao != 'd');
}

void MenuVenda(){
    char opcao;
    do{
        printf("=======================================\n");
        printf("Menu Venda\n");
        printf("=======================================\n");
        printf("a. Inserir uma venda\n");
        printf("b. Excluir uma venda\n");
        printf("c. Mostrar os carros vendidos de uma determinada marca, ordenados crescentemente pelo modelo\n");
        printf("d. Informar a quantidade de carros vendidos com a soma dos precos vendidos\n");
        printf("e. Informar total lucro das vendas\n");
        printf("f. sair\n");
        printf("=======================================\n");
        printf("Escolha uma opcao:\n");
        if (scanf (" %c", &opcao) != 1){
            printf("=======================================\n");
            printf("Entrada invalida! Por favor digite uma opcao que esta no Menu Carro\n");
            continue;
        }

        switch (opcao)
        {
        case 'a':
            printf("=======================================\n");
            printf("INSERIR VENDA\n");
            //InserirVenda();
            break;

        case 'b':
            printf("=======================================\n");
            printf("EXCLUIR VENDA\n");
            //ExcluirVenda();
            break;

        case 'c':
            printf("=======================================\n");
            printf("MOSTRAR CARROS VENDIDOS POR DETERMINADA MARCA\n");
            MostrarCarrosVend_Marca();
            break;

        case 'd':
            printf("=======================================\n");
            printf("QUANTIDADE DE CARROS VENDIDOS COM SOMA DOS PRECOS\n");
            //InformarQuantidadeSomaPreco();
            break;

        case 'e':
            printf("=======================================\n");
            printf("LUCRO TOTAL DE VENDAS\n");
            //InformarLucroTotal();
            break;

        case 'f':
            printf("=======================================\n");
            printf("Saindo...\n");
            break;
        
        default:
            printf("OPCAO INEXISTENTE\n");
            break;
        }
    } while (opcao != 'e');

}

void mostrarMenu(){
    printf("=======================================\n");
    printf("1- Carro\n");
    printf("2- Cliente\n");
    printf("3- Venda\n");
    printf("4- Sair do sistema\n");
    printf("=======================================\n");
}

int main (){
    int opcao;
    do {
        mostrarMenu();
        printf("Escolha uma opcao: \n");
        if (scanf("%d", &opcao) != 1) {
            printf("=======================================\n");
            printf("Entrada invalida!\n");
            while (getchar() != '\n'); // limpa buffer
            continue;
        }  

        switch (opcao)
        {
        case 1:
            MenuCarro();
            break;
        
        case 2: 
            MenuCliente();
            break;

        case 3:
            MenuVenda();
            break;

        case 4:
            printf("=======================================\n");
            printf("Saindo do programa...\n");
            break;
        default:
            break;
        }
    } while (opcao != 4);
    


    return 0;   
}