#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "revenda.h"
#include <configuracao.h>

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
        printf("Modelo inválido para BMW! Use apenas: 320i, x1, x5.\n");
        fclose(arquivo);
        return;
    }
    if (strcmp(novo_carro.marca, "fiat") == 0 && 
        strcmp(novo_carro.modelo, "mobi") != 0 && 
        strcmp(novo_carro.modelo, "toro") != 0 && 
        strcmp(novo_carro.modelo, "uno") != 0) {
        printf("Modelo inválido para Fiat! Use apenas: mobi, toro, uno.\n");
        fclose(arquivo);
        return;
    }
    if (strcmp(novo_carro.marca, "chevrolet") == 0 && 
        strcmp(novo_carro.modelo, "celta") != 0 && 
        strcmp(novo_carro.modelo, "astra") != 0 && 
        strcmp(novo_carro.modelo, "tracker") != 0) {
        printf("Modelo inválido para Chevrolet! Use apenas: celta, astra, tracker.\n");
        fclose(arquivo);
        return;
    }

    // Ano de fabricação
    printf("Digite o ANO de fabricação (1980-2025): \n");
    if (scanf("%d", &novo_carro.ano_fabricacao) != 1 || 
        novo_carro.ano_fabricacao < 1980 || novo_carro.ano_fabricacao > 2025) {
        printf("Ano inválido! Deve ser entre 1980 e 2025.\n");
        fclose(arquivo);
        return;
    }

    // Combustível
    printf("Digite o COMBUSTÍVEL do carro (alcool, gasolina, flex): \n");
    scanf(" %[^\n]", novo_carro.combustivel);
    if (strcmp(novo_carro.combustivel, "alcool") != 0 && 
        strcmp(novo_carro.combustivel, "gasolina") != 0 && 
        strcmp(novo_carro.combustivel, "flex") != 0) {
        printf("Combustível inválido! Use apenas: alcool, gasolina, flex.\n");
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
        printf("Cor inválida! Use apenas: branca, prata, preta, cinza, bi-color.\n");
        fclose(arquivo);
        return;
    }

    // Preço de compra
    printf("Digite o PREÇO de compra do carro (R$): \n");
    if (scanf("%f", &novo_carro.preco_compra) != 1 || novo_carro.preco_compra <= 0) {
        printf("Preço inválido! Deve ser maior que zero.\n");
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

int buscar_carro_por_placa(const char *placa){
    FILE *arquivo = abrir_arquivo_carro("rb");
    if (arquivo == NULL) {
        return 0;
    }

    struct CARRO carro;
    rewind(arquivo);
    while (fread(&carro, sizeof(struct CARRO), 1, arquivo) == 1) {
        if (strcmp(carro.placa, placa) == 0 && carro.ativo) {
            fclose(arquivo);
            return 1; // Carro encontrado e ativo
        }
    }

    fclose(arquivo);
    return 0; // Carro não encontrado ou inativo
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