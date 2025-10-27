#include "revenda.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h> 


// CONTEÚDO AUXILIAR GERAL E CONSTANTES 

#define ARQ_CARRO "carros.bin"
#define ARQ_CLIENTE "clientes.bin"
#define ARQ_VENDA "vendas.bin"
#define MAX_REGISTRO 500

// Variáveis Globais de Geração
char *MARCAS[] = {"bmw", "fiat", "chevrolet"};
const char *MODELOS_BMW[] = {"320i", "x1", "x5"};
const char *MODELOS_FIAT[] = {"mobi", "toro", "uno"};
const char *MODELOS_CHEVROLET[] = {"celta", "astra", "tracker"};
const char *COMBUSTIVEIS[] = {"alcool", "gasolina", "flex"};
const char *CORES[] = {"branca", "prata", "preta", "cinza", "bi-color"};
const char *NOMES[] = {"Joao Pedro", "Ryan", "Vinicius", "Yzamak", "Amanda", "Gleyci"};
const char *SOBRENOMES[] = {"Schons", "Martins", "Amancio", "Campos", "Meneguete", "Pereira", "Correa"};
const char *BAIRRO[] = {"CENTRO", "Sibipiruna", "Flamboyant"};
const char *RUA[] = {"Avenida 8", "Teiji Matsui", "Major Pedro H. Cavalcante", "Rua 12", "Rua P9"};
const char *CIDADE[] = {"Dourados", "Chapadao do Sul", "Vicentina"};
const char *ESTADO[] = {"MS"};
const char *CEP[] = {"79.560-000", "79.823-640", "79.710-000"};


// Estrutura Auxiliar de Venda 
struct VENDIDOS_AUX{
    char modelo[TAM];
    char placa[9];
    int ano_fabircacao;
    char nome_cliente[TAM];
};

// Auxiliares/Geração
static void inicializar_rand();
void gerar_placa(char placa[9]);
void gerar_dados_carros(struct CARRO *c);
void mostrar_carro(const struct CARRO *carro);
void gerar_dados_clientes(struct CLIENTE *cliente);
void mostrar_cliente(const struct CLIENTE *cli);
static FILE *abrir_arquivo(const char *nome_arq, const char *modo_abertura);
static int arquivo_nao_vazio(const char *arquivo_nome);
int data_valida_simples(int dia, int mes, int ano);

// Módulos de I/O
static FILE *abrir_arquivo_carro(const char *modo_abertura);
static FILE *abrir_arquivo_clientes(const char *modo_abertura);

// Funções de Busca/Regra
int buscar_carro_por_placa(const char *placa, struct CARRO *c_out);
int buscar_cliente_por_codigo(int codigo, struct CLIENTE *cli_out);
int carro_foi_vendido(const char *placa);

// Funções de Ordenação
int comparar_carro_marca_modelo(const void *a, const void *b);
int comparar_vendidos_modelo(const void *a, const void *b); 

// Funções Principais
void InserirCarro();
void ExcluirCarro();
void MostrarCarroDisp_marca_modelo();
int MostrarCarroDispo_selecao();

void InserirCliente();
void AlterarRendaCliente();
void MostrarClientesOrdenadosNome();

void InserirVenda();
void ExcluirVenda();
void MostrarCarrosVend_Marca();
void InformarQuantidadeSomaPreco();
void InformarLucroTotal();

void MenuCarro();
void MenuCliente();
void MenuVenda();
void mostrarMenu();


// OBJETIVO: Valida se a data (dia, mês, ano) é logicamente possível
//parametros: dia, mes e ano
//retorno: 0 se for invalido, 1 se for valido

int data_valida_simples(int dia, int mes, int ano) {
    // 1. Faixa de Anos (Pode ajustar 2025 para 2030, por exemplo, se quiser mais flexibilidade)
    if (ano < 1980 || ano > 2025) {
        printf("Ano (%d) deve ser entre 1980 e 2025. Digite novamente a data (dd mm aaaa)\n", ano);
        return 0;
    }

    // 2. Validação do Mês
    if (mes < 1 || mes > 12) {
        printf("Mes (%d) deve ser entre 1 e 12. Digite novamente a data (dd mm aaaa)\n", mes);
        return 0;
    }

    // 3. Validação do Dia
    if (dia < 1 || dia > 31) {
        printf("Dia (%d) deve ser entre 1 e 31. Digite novamente a data (dd mm aaaa)\n", dia);
        return 0;
    }

    // 4. Checagem de Limite de Dias por Mês (A CHAVE DA VALIDAÇÃO)
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) {
        // Meses com 30 dias (Abril, Junho, Setembro, Novembro)
        if (dia > 30) {
            printf("Mes %d so tem 30 dias. Digite novamente a data (dd mm aaaa)\n", mes);
            return 0;
        }
    } else if (mes == 2) {
        if (dia > 28) {
            printf("Fevereiro (%d) so tem 28 dias. Digite novamente a data (dd mm aaaa)\n", dia);
            return 0;
        }
    }

    return 1; // Data válida
}


//OBJETIVO: apenas inicializar a randominazacao
//PARAMETRO: Nenhum
//RETORNO: Nenhum
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
void gerar_dados_carros(struct CARRO *c);
void mostrar_carro(const struct CARRO *carro);


//gerador cliente
void gerar_dados_clientes(struct CLIENTE *cliente);
void mostrar_cliente(const struct CLIENTE *cli);

//OBJETIVO: Gerar placa do carro
//PARAMETRO: vetor placa aonde sera guardado a placa
//RETORNO: Nenhum
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

    for(int i = 0; i < TAM; i++){
        carro->marca[i] = '\0';
    }
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

//OBJETIVO: Gerar dados do cliente
//PARAMETRO: Registro de um cliente
//RETORNO: Nenhum
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


    const char *NOMES[] = {"Joao Pedro", "Ryan", "Vinicius", "Yzamak", "Amanda", "Gleyci"};
    const char *SOBRENOMES[] = {"Schons", "Martins", "Amancio", "Campos", "Meneguete", "Pereira", "Correa"};
    const char *BAIRRO[] = {"CENTRO", "Sibipiruna", "Flamboyant"};
    const char *RUA[] = {"Avenida 8", "Teiji Matsui", "Major Pedro H. Cavalcante", "Rua 12", "Rua P9"};
    const char *CIDADE[] = {"Dourados", "Chapadao do Sul", "Vicentina"};
    const char *CEP[11] = {"79.560-000", "79.823-640", "79.710-000"};
    const char *ESTADO[] = {"MS"};
    int aux_nomes = rand() % 3;

    char nome_completo[50] = ""; 
    
    strcpy(nome_completo, NOMES[rand() % 6]); 
    strcat(nome_completo, " ");           
    strcat(nome_completo, SOBRENOMES[rand() % 7]);     

    //randomiza o nome
    strcpy(cliente->nome, nome_completo);
    strcpy(cliente->endereco.cidade, CIDADE[aux_nomes]);
    strcpy(cliente->endereco.cep, CEP[aux_nomes]);
    strcpy(cliente->endereco.rua, RUA[rand() % 3]);
    strcpy(cliente->endereco.bairro, BAIRRO[rand() % 3]);
    strcpy(cliente->endereco.estado, ESTADO[0]);
    cliente->renda_mensal = (float) (1 + rand() % 9999);
    cliente->endereco.numero = rand() % 999;
    //char telefone[14];  //99 99999-9999
    cliente->celular->telefone[0] = '6';
    cliente->celular->telefone[1] = '7'; // 1
    cliente->celular->telefone[2] = ' ';
    for (int i = 3; i < 8; i++){
        cliente->celular->telefone[i] = '0' + (rand() % 10);
    }
    cliente->celular->telefone[8] = '-';
    for (int i = 9; i < 13; i++){
        cliente->celular->telefone[i] = '0' + (rand() % 10);
    }
    cliente->celular[0].telefone[13] = '\0';
    cliente->ativo = 1;
    proximo_codigo_cliente++;
}

//OBJETIVO: mostrar dados do cliente
//PARAMETRO: Registro de um cliente
//RETORNO: Nenhum
void mostrar_cliente(const struct CLIENTE *cli) {
    printf("Codigo: %d\n", cli->codigo);
    printf("Nome: %s\n", cli->nome);
    printf("Celular: %s\n", cli->celular->telefone);
    printf("Renda Mensal: R$ %.2f\n", cli->renda_mensal);
    printf("Endereco: %s, %d - %s/%s\n", cli->endereco.rua, cli->endereco.numero, cli->endereco.cidade, cli->endereco.estado);
}

//OBJETIVO: funcao para abrir arquivo e verificar ele e tals
//PARAMETRO: nome do arquivo e a flag de abertura
//RETORNO: arquivo, caso for != null
static FILE *abrir_arquivo(const char *nome_arq, const char *modo_abertura) {
    FILE *arq = fopen(nome_arq, modo_abertura);
    if (arq == NULL) {
        if (strcmp(modo_abertura, "rb+") == 0 || strcmp(modo_abertura, "ab") == 0) {
        }
    }
    return arq;
}

//OBJETIVO: Gerar dados do carro 
//PARAMETRO: Registro de um carro
//RETORNO: Nenhum
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

// apenas chamada
int buscar_cliente_por_codigo(int codigo, struct CLIENTE *cli_out);


//OBJETIVO: funcao para abrir arquivo e verificar ele e tals
//PARAMETRO: nome do arquivo e a flag de abertura
//RETORNO: arquivo, caso for != null
FILE *abrir_arquivo_carro(const char *modo_abertura){
    FILE *arquivo = fopen(ARQ_CARRO, modo_abertura);
    if (arquivo == NULL){
        if (strcmp(modo_abertura, "r+b") == 0 || strcmp(modo_abertura, "a+b") == 0){
            printf("Nao foi possivel abrir ou criar o arquivo de carros.\n");
        }
    }
    return arquivo;
}

//OBJETIVO: Inserir carro
//PARAMETRO: nenhum
//RETORNO: nenhum
void InserirCarro(){
    FILE *arquivo = abrir_arquivo_carro("a+b");
    if (arquivo == NULL) {
        return;
    }

    struct CARRO novo_carro;
    novo_carro.ativo = 1; // Carro ativo ao inserir

    printf("=======================================\n");
    printf("INSERIR CARRO\n");
    gerar_dados_carros(&novo_carro);
    mostrar_carro(&novo_carro);


    // Verificar se a placa já existe
    struct CARRO temp;
    rewind(arquivo);
    while (fread(&temp, sizeof(struct CARRO), 1, arquivo) == 1) {
        if (strcmp(temp.placa, novo_carro.placa) == 0 && temp.ativo) {
            printf("Carro com placa %s ja existe!\n", novo_carro.placa);
            fclose(arquivo);
            return;
        }
    }

    
    if (fwrite(&novo_carro, sizeof(struct CARRO), 1, arquivo) != 1) {
        printf("=======================================\n");
        printf("Erro ao gravar o cliente no arquivo.\n");
    } else {
        printf("=======================================\n");
        printf("Um(a) %s %s com a placa %s inserido com sucesso!\n", novo_carro.marca, novo_carro.modelo, novo_carro.placa);
    }

    

    

    fclose(arquivo);
}

//OBJETIVO: Excluir carro
//PARAMETRO: nenhum
//RETORNO: nenhum
void ExcluirCarro(){
    FILE *arquivo = abrir_arquivo_carro("rb");
    if (arquivo == NULL) {
        printf("Nenhum carro inserido para fazer exclusao!\n");
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
        printf("Nenhum carro disponivel para excluir!\n");
        fclose(arquivo);
        return;
    }

    char placa[9];
    printf("=======================================\n");
    printf("EXCLUIR CARRO\n");
    printf("Digite a PLACA do carro a ser excluido (AAA-1234): \n");
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
        printf("Placa invalida! Use o formato AAA-1234.\n");
        fclose(arquivo);
        return;
    }

    if(carro_foi_vendido(placa)){
        printf("ERRO: Carro com placa %s foi vendido e NAO pode ser excluido.\n", placa);
        fclose(arquivo);
        return;
    }


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
                printf("Carro com placa %s excluido com sucesso!\n", placa);
            }
            break;
        }
    }

    if (!encontrado) {
        printf("Carro com placa %s nao encontrado ou ja excluido!\n", placa);
    }

    fclose(arquivo);
}

void MostrarCarroDisp_marca_modelo(){
    FILE *arquivo = abrir_arquivo_carro("rb");
    if (arquivo == NULL) {
        printf("Nenhum carro disponivel para mostrar!\n");
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
        printf("Nenhum carro disponivel para venda!\n");
        fclose(arquivo);
        return;
    }

    // Alocar array para carros ativos
    struct CARRO *carros = malloc(count * sizeof(struct CARRO));
    if (carros == NULL) {
        printf("Erro de alocacao de memoria!\n");
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
    printf("CARROS DISPONIVEIS (ORDENADOS POR MARCA E MODELO)\n");
    for (int i = 0; i < count; i++) {
        printf("Placa: %s | Marca: %s | Modelo: %s | Ano: %d | Combustivel: %s | Cor: %s | Preco: R$%.2f\n",
               carros[i].placa, carros[i].marca, carros[i].modelo, carros[i].ano_fabricacao,
               carros[i].combustivel, carros[i].cor, carros[i].preco_compra);

        printf("Opcionais:\n");
        for (int j = 0; j < 12; j++) {
            if (carros[i].opcional[j]) {
                switch (j) {
                    case 0: printf("- air.bag\n"); break;
                    case 1: printf("- banco.couro\n"); break;
                    case 2: printf("- sensor.ponto.cego\n"); break;
                    case 3: printf("- cambio.automatico\n"); break;
                    case 4: printf("- cambio.borboleta\n"); break;
                    case 5: printf("- controle.estabilidade\n"); break;
                    case 6: printf("- start.stop\n"); break;
                    case 7: printf("- camera.360\n"); break;
                    case 8: printf("- ar.condicionado\n"); break;
                    case 9: printf("- abs\n"); break;
                    case 10: printf("- sensor.estacionamento\n"); break;
                    case 11: printf("- partida.sem.chave\n"); break;
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
        printf("Nenhum carro disponivel para mostrar!\n");
        return 0;
    }

    // Contar carros ativos
    struct CARRO carro;
    int count = 0;
    int opcionais_selecionados[12] = {0}; 
    int total_opcionais_selecionados = 0;
    char input_buffer[5];
    int num_opcional;

    rewind(arquivo);
    while (fread(&carro, sizeof(struct CARRO), 1, arquivo) == 1) {
        if (carro.ativo) {
            count++;
        }
    }

    if (count == 0) {
        printf("Nenhum carro disponivel para mostrar!\n");
        fclose(arquivo);
        return 0;
    }

    printf("=======================================\n");
    printf("MOSTRAR CARROS POR SELECAO DE OPCIONAIS\n");
    
    // 1. LISTA E LOOP INTERATIVO (Melhor Usabilidade)
    do {
        printf("---------------------------------------\n");
        printf("OPCIONAIS SELECIONADOS (%d):\n", total_opcionais_selecionados);
        for (int i = 0; i < 12; i++) {
            // Exibe [X] se selecionado, [ ] se não
            printf("%2d. %-20s [%c]\n", i + 1, opcionais[i], opcionais_selecionados[i] ? 'X' : ' ');
            //                                                              
        }
        printf("---------------------------------------\n");
        printf("Digite o NUMERO do opcional (1 a 12) para alternar, ou 'F' para buscar:\n");
        
        printf("Opção: ");
        if (scanf("%4s", input_buffer) != 1) { 
            while (getchar() != '\n' && !feof(stdin)); // Limpa buffer
            continue;
        }

        if (input_buffer[0] == 'F' || input_buffer[0] == 'f') {
            break; // Sai do loop de seleção
        }

        // Tenta converter a entrada para número
        if (sscanf(input_buffer, "%d", &num_opcional) == 1) {
            int indice = num_opcional - 1; // Ajuste para 0 a 11
            
            if (indice >= 0 && indice < 12) {
                // Alterna o status e o contador de selecionados
                if (opcionais_selecionados[indice] == 0) {
                    opcionais_selecionados[indice] = 1;
                    total_opcionais_selecionados++;
                } else {
                    opcionais_selecionados[indice] = 0;
                    total_opcionais_selecionados--;
                }
            } else {
                printf("Numero de opcional invalido.\n");
            }
        } else {
            printf("Entrada invalida.\n");
            while (getchar() != '\n' && !feof(stdin));
        }
    } while (1); // Continua até que 'F' seja digitado
    
    
    if (total_opcionais_selecionados == 0) {
        printf("Nenhum opcional selecionado. Operacao cancelada.\n");
        fclose(arquivo);
        return 0;
    }

    
    int encontrados = 0;
    rewind(arquivo);

    printf("=======================================\n");
    printf("Carros disponiveis com os %d opcionais selecionados:\n", total_opcionais_selecionados);
    
    // exibição dos resultados
    printf("%-8s | %-12s | %-12s | %-10s\n", "Placa", "Marca", "Modelo", "Preço");
    printf("--------------------------------------------------\n");

    while (fread(&carro, sizeof(struct CARRO), 1, arquivo) == 1) {
        
        
        if (carro.ativo && !carro_foi_vendido(carro.placa)) { 
            
            int todos_presentes = 1;
            for (int i = 0; i < 12; i++) {
                // Se o usuário QUER o opcional (opcionais_selecionados[i]==1)
                // E o carro NÃO TEM (carro.opcional[i]==0), o filtro falha.
                if (opcionais_selecionados[i] == 1 && carro.opcional[i] == 0) {
                    todos_presentes = 0;
                    break;
                }
            }
            
            if (todos_presentes) {
                // Exibe o carro em formato tabular 
                printf("%-8s | %-12s | %-12s | R$%.2f\n",
                       carro.placa, carro.marca, carro.modelo, carro.preco_compra);

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
    return abrir_arquivo(ARQ_CLIENTE, modo_abertura);
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
    novo_cliente.ativo = 1;
    
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

    printf("Digite o codigo do cliente: ");
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
        printf("Cliente com codigo %d nao encontrado ou inativo.\n", cod);

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

    while (fread(&c, sizeof(struct CLIENTE), 1, arquivo) == 1) {
        if (c.codigo == codigo && c.ativo == 1) {
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
    FILE *arquivo = abrir_arquivo(ARQ_VENDA, "rb");
    struct VENDA_CARRO carro_atual;
    if (arquivo == NULL){
        return 0;
    }
    int vendido = 0;

    while(fread(&carro_atual, sizeof(struct VENDA_CARRO), 1, arquivo) == 1){
        if(strcmp(placa, carro_atual.placa_carro) == 0){
            if(carro_atual.ativo == 1){
                vendido = 1;
            }
        }
    }

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
//BLOCO AUXILIAR PARA VENDAS

static int arquivo_nao_vazio(const char *arquivo_nome){
    FILE *arquivo = fopen(arquivo_nome, "rb");

    if (arquivo == NULL){
        return 0;
    }

    fseek(arquivo, 0, SEEK_END);

    long tamanho = ftell(arquivo);
    fclose(arquivo);

    return (tamanho > 0);
}

void InserirVenda(){

    struct VENDA_CARRO nova_venda;
    char placa_busca[9];
    int codigo_cliente;

    struct CARRO carro_info;
    struct CLIENTE cliente_info;

    if(!arquivo_nao_vazio(ARQ_CARRO)){
        printf("=======================================\n");
        printf("ERRO: Impossivel registrar venda, pois nenhum carro foi registrado!\n");
        return;
    }

    if (!arquivo_nao_vazio(ARQ_CLIENTE)){
        printf("=======================================\n");
        printf("ERRO: Impossivel registrar venda, pois nenhum cliente foi registrado!\n");
        return;
    }

    printf("=======================================\n");
    printf("INSERIR VENDA\n");

    printf("Digite a PLACA do carro a ser vendido (AAA-1234): \n");
    if (scanf("%8s", placa_busca) != 1){
        return;
    }

    // Validar formato da placa
    if (strlen(placa_busca) != 8 || 
        !isalpha(placa_busca[0]) || !isalpha(placa_busca[1]) || !isalpha(placa_busca[2]) ||
        placa_busca[3] != '-' ||
        !isdigit(placa_busca[4]) || !isdigit(placa_busca[5]) || 
        !isdigit(placa_busca[6]) || !isdigit(placa_busca[7])) {
        printf("Placa invalida! Use o formato AAA-1234.\n");
        return;
    }
    

    placa_busca[8] = '\0';
    if (carro_foi_vendido(placa_busca)){
        printf("=======================================\n");
        printf("Carro com a placa %s ja foi vendido!\n", placa_busca);
        return;
    }

    
    if (!buscar_carro_por_placa(placa_busca, &carro_info)){             
        printf("ERRO: Carro com placa %s nao encontrado. Verifique se a placa esta correta ou se o carro foi excluido do cadastro.\n", placa_busca);
        return;
    }
    
    printf("=======================================\n");
    printf("Digite o CODIGO do cliente comprador: \n");
    if(scanf("%d", &codigo_cliente) != 1){
        return;
    }

    
    if (!buscar_cliente_por_codigo(codigo_cliente, &cliente_info)){            
        printf("cliente nao encontrado");
        return;
    }

    strcpy(nova_venda.placa_carro, placa_busca);
    nova_venda.codigo_cliente = codigo_cliente;
    nova_venda.ativo = 1;
    
   
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

    do {
        
        int leitura_sucesso = scanf("%d %d %d", &nova_venda.data_venda.dia, &nova_venda.data_venda.mes, &nova_venda.data_venda.ano);
        
        if (leitura_sucesso == 3 && data_valida_simples(nova_venda.data_venda.dia, nova_venda.data_venda.mes, nova_venda.data_venda.ano)) 
        {
            break; // Data é válida, sai do loop
        }
    } while(1);

    
    //Gravar as informacoes adquiridas no arquivo
    FILE *arquivo = abrir_arquivo(ARQ_VENDA, "ab"); //ab eh pra gravacao, sempre escreve no fim do arquivo
    if (arquivo == NULL){
        return;
    }

    if(fwrite(&nova_venda, sizeof(struct VENDA_CARRO), 1, arquivo) != 1){
        printf("ERRO ao gravar a venda no registro!\n");
    } else {
        printf("Venda registrada com sucesso!\n");
        printf("Carro: %s - %s | Cliente: %s\n", carro_info.modelo, nova_venda.placa_carro, cliente_info.nome);
    
    }
    fclose(arquivo);
}

void ExcluirVenda(){
    char placa[9];
    

    FILE *arquivo = abrir_arquivo(ARQ_VENDA, "rb+");
    if(arquivo == NULL){
        printf("Nenhuma venda registrada para exclusao.\n");
        return;
    }

    struct VENDA_CARRO check_venda;
    int contador_ativo = 0;
    
    // Volta o ponteiro para o início para garantir a leitura completa
    rewind(arquivo); 
    
    while (fread(&check_venda, sizeof(struct VENDA_CARRO), 1, arquivo) == 1) {
        if (check_venda.ativo) {
            contador_ativo++;
        }
    }

    if (contador_ativo == 0) {
        printf("Nenhuma venda ATIVA disponivel para exclusao. (Total: 0)\n");
        fclose(arquivo);
        return; // Retorna antes de pedir a placa
    }


    printf("=======================================\n");
    printf("EXCLUIR VENDA\n");
    printf("Digite a PLACA do carro que deseja excluir a VENDA!(AAA-XXXX):\n");
    if(scanf("%8s", placa) != 1){
        printf("Placa invalida, por favor digite uma placa do modelo valido (AAA-XXXX)\n");
        fclose(arquivo);
        return;
    }

    struct VENDA_CARRO nova_venda;
    int encontrado = 0;
    long posicao_atual = 0;
    rewind(arquivo);
    while (fread(&nova_venda, sizeof(struct VENDA_CARRO), 1, arquivo) == 1) {

        if (strcmp(nova_venda.placa_carro, placa) == 0 && nova_venda.ativo == 1){

            fseek(arquivo, posicao_atual, SEEK_SET);
            nova_venda.ativo = 0;
            if(fwrite(&nova_venda, sizeof(struct VENDA_CARRO), 1, arquivo) == 1){
                printf("Carro com placa %s teve sua venda excluida com sucesso!\n", nova_venda.placa_carro);
                encontrado = 1;
            } else {
                printf("ERRO: Falha ao reescrever o registro de venda no arquivo.\n");
            }
            
            break;
        }
        posicao_atual = ftell(arquivo);
    }

    if(!encontrado){
        printf("Venda para o carro com placa %s nao encontrada ou ja inativa.\n", placa);
    }

    fclose(arquivo);
}



int comparar_vendidos_modelo(const void *a, const void *b){
    //ver sobre qsort para fazer uma comparacao por modelo crescente
    struct VENDIDOS_AUX *carro_a = (struct VENDIDOS_AUX *)a;
    struct VENDIDOS_AUX *carro_b = (struct VENDIDOS_AUX *)b;
    
    return strcmp(carro_a->modelo, carro_b->modelo);
}

void MostrarCarrosVend_Marca(){
    struct VENDIDOS_AUX lista_vendidos [MAX_REGISTRO];
    struct VENDA_CARRO venda_atual;
    struct CARRO carro_info;
    struct CLIENTE cliente_info;
    int num_registro = 0; //contador de registro no array
    

    char marca_busca[TAM];
    
    FILE *arquivo = abrir_arquivo(ARQ_VENDA, "rb");
        if(arquivo == NULL){
            printf("Nenhuma venda registrada para ordenar!.\n");
            return;
        }


    struct VENDA_CARRO check_venda;
    int contador_ativo = 0;
    
    // Volta o ponteiro para o início para garantir a leitura completa
    rewind(arquivo); 
    
    while (fread(&check_venda, sizeof(struct VENDA_CARRO), 1, arquivo) == 1) {
        if (check_venda.ativo) {
            contador_ativo++;
        }
    }

    if (contador_ativo == 0) {
        printf("Nenhuma venda ATIVA disponivel para mostrar as vendas!\n");
        fclose(arquivo);
        return; // Retorna antes de pedir a placa
    }

    printf("Digite a MARCA para listar os carros vendidos\n");
    if(scanf("%49s", marca_busca) != 1){
        printf("Marca invalida, tente novamente!\n");
        fclose(arquivo);
        return;
    }
    
   // Converter a entrada do usuário para minúsculas
    for(int i = 0; marca_busca[i]; i++){
        marca_busca[i] = tolower((unsigned char)marca_busca[i]);
    }
    rewind(arquivo);
    // 2. Leitura e Processamento (Apenas um loop)
    while (fread(&venda_atual, sizeof(struct VENDA_CARRO), 1, arquivo) == 1){

        if (venda_atual.ativo == 1){
            // Buscar carro para obter a marca e verificar se está ativo
            if(buscar_carro_por_placa(venda_atual.placa_carro, &carro_info)){
                
                // CRITÉRIO DE FILTRO: Marca do carro (lida do arquivo) deve ser igual à marca buscada
                // (Ambas já em minúsculas ou consistentes)
                if(strcmp(carro_info.marca, marca_busca) == 0){
                    
                    if(num_registro >= MAX_REGISTRO){
                        printf("AVISO: Limite de %d registros para ordenacao atingido.\n", MAX_REGISTRO);
                        break;
                    }

                    // 3. Busca do Cliente (Se o cliente não for encontrado, ele é "INATIVO")
                    if (buscar_cliente_por_codigo(venda_atual.codigo_cliente, &cliente_info)){
                        strcpy(lista_vendidos[num_registro].nome_cliente, cliente_info.nome);
                    } else {
                        // O problema do "CLIENTE INATIVO" ocorre aqui
                        strcpy(lista_vendidos[num_registro].nome_cliente, "CLIENTE INATIVO");
                    }

                    // 4. Carrega os dados combinados no array
                    strcpy(lista_vendidos[num_registro].modelo, carro_info.modelo);
                    strcpy(lista_vendidos[num_registro].placa, venda_atual.placa_carro);
                    lista_vendidos[num_registro].ano_fabircacao = carro_info.ano_fabricacao;
                    
                    num_registro++;
                }
            }
        }
    }
    
    // 5. Fechamento e Checagem de Resultados
    fclose(arquivo);

    if (num_registro == 0) {
        printf("=======================================\n");
        printf("Nao foi encontrada nenhuma venda para a marca '%s'. Verifique a digitacao.\n", marca_busca);
        printf("=======================================\n");
        return;
    }

    // 6. Ordenação e Exibição
    qsort(lista_vendidos, num_registro, sizeof(struct VENDIDOS_AUX), comparar_vendidos_modelo);
    
    printf("=======================================\n");
    printf("\nCARROS VENDIDOS ORDENADOS PELO MODELO (Total: %d)\n", num_registro);
    printf("-------------------------------------------------\n");
    printf("%-15s | %-8s | %-4s | %-40s\n", "MODELO", "PLACA", "ANO", "NOME CLIENTE");
    printf("-------------------------------------------------\n");
    for(int i = 0; i < num_registro; i++){
        printf("%-15s | %-8s | %-4d | %-40s\n",
        lista_vendidos[i].modelo,
        lista_vendidos[i].placa,
        lista_vendidos[i].ano_fabircacao,
        lista_vendidos[i].nome_cliente);
    }
    printf("-------------------------------------------------\n");
}



void InformarQuantidadeSomaPreco(){
    struct VENDA_CARRO venda_atual;

    FILE *arquivo = abrir_arquivo(ARQ_VENDA, "rb");
        if(arquivo == NULL){
            printf("Nenhuma venda registrada para informar!.\n");
            return;
        }

    struct VENDA_CARRO check_venda;
    int contador_ativo = 0;
    
    // Volta o ponteiro para o início para garantir a leitura completa
    rewind(arquivo); 
    
    while (fread(&check_venda, sizeof(struct VENDA_CARRO), 1, arquivo) == 1) {
        if (check_venda.ativo) {
            contador_ativo++;
        }
    }

    if (contador_ativo == 0) {
        printf("Nenhuma venda para mostrar soma total de preco!\n");
        fclose(arquivo);
        return; 
    }
        double soma_precos = 0.0;
        int quantidade = 0;


        while(fread(&venda_atual, sizeof(struct VENDA_CARRO), 1, arquivo) == 1){
            if(venda_atual.ativo == 1){
                quantidade++;
                soma_precos += venda_atual.preco_venda;

            }
        }

        fclose(arquivo);
        printf("=======================================\n");
        printf("\nQUANTIDADE DE CARRO(s) VENDIDO(s) (Total: %d)\n", quantidade);
        printf("-------------------------------------------------\n");
        printf("Soma dos precos vendidos: R$ %.2f\n", soma_precos);
        printf("-------------------------------------------------\n");

}

void InformarLucroTotal(){

    struct VENDA_CARRO venda_atual;
    struct CARRO vendinha;
    FILE *arquivo = abrir_arquivo(ARQ_VENDA, "rb+");
        if(arquivo == NULL){
            printf("Nenhuma venda registrada para informar!.\n");
            return;
        }

    struct VENDA_CARRO check_venda;
    int contador_ativo = 0;
    
    // Volta o ponteiro para o início para garantir a leitura completa
    rewind(arquivo); 
    
    while (fread(&check_venda, sizeof(struct VENDA_CARRO), 1, arquivo) == 1) {
        if (check_venda.ativo) {
            contador_ativo++;
        }
    }

    if (contador_ativo == 0) {
        printf("Nenhuma venda ATIVA para mostrar o lucro\n");
        fclose(arquivo);
        return; 
    }

  

        float lucro_total = 0.0f;
       
        while(fread(&venda_atual, sizeof(struct VENDA_CARRO), 1, arquivo) == 1){
            if(venda_atual.ativo == 1){

                if (buscar_carro_por_placa(venda_atual.placa_carro, &vendinha)){
                    float lucro_venda_atual = venda_atual.preco_venda - vendinha.preco_compra;
                    lucro_total += lucro_venda_atual;
                } else {
                    printf("AVISO: Carro %s vendido mas nao encontrado no cadastro (Custo desconhecido).\n", 
                       venda_atual.placa_carro);
                }
                
            }
        }
        
        printf("=======================================\n");
        printf("-------------------------------------------------\n");
        if (lucro_total < 0 )
            printf("Divida/Prejuizo Total: R$ %.2f\n", lucro_total);
        else {
            printf("Lucro de vendas de: R$ %.2f\n", lucro_total);
        }
        printf("-------------------------------------------------\n");
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
            InserirVenda();
            break;

        case 'b':
            printf("=======================================\n");
            printf("EXCLUIR VENDA\n");
            ExcluirVenda();
            break;

        case 'c':
            printf("=======================================\n");
            printf("MOSTRAR CARROS VENDIDOS POR DETERMINADA MARCA\n");
            MostrarCarrosVend_Marca();
            break;

        case 'd':
            printf("=======================================\n");
            printf("QUANTIDADE DE CARROS VENDIDOS COM SOMA DOS PRECOS\n");
            InformarQuantidadeSomaPreco();
            break;

        case 'e':
            printf("=======================================\n");
            printf("LUCRO TOTAL DE VENDAS\n");
            InformarLucroTotal();
            break;

        case 'f':
            printf("=======================================\n");
            printf("Saindo...\n");
            break;
        
        default:
            printf("OPCAO INEXISTENTE\n");
            break;
        }
    } while (opcao != 'f');

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