#include "include/revenda.h"
#include <stdio.h>

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
            break;

        case 'b':
            printf("=======================================\n");
            printf("EXCLUIR CARRO\n");
            break;

        case 'c':
            printf("=======================================\n");
            printf("MOSTRAR CARROS DISPONIVEIS EM ORDEM CRESCENTE POR MARCA E MODELO\n");
            break;

        case 'd':
            printf("=======================================\n");
            printf("MOSTRAR CARROS DISPONIVEIS POR SELECAO DE UMA OU MAIS OPCAO\n");
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
            break;

        case 'b':
            printf("=======================================\n");
            printf("ALTERAR RENDA MENSAL DO CLIENTE\n");
            break;

        case 'c':
            printf("=======================================\n");
            printf("MOSTRAR CLIENTES EM ORDEM ALFABETICA\n");
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
            break;

        case 'b':
            printf("=======================================\n");
            printf("EXCLUIR VENDA\n");
            break;

        case 'c':
            printf("=======================================\n");
            printf("MOSTRAR CARROS VENDIDOS POR DETERMINADA MARCA\n");
            break;

        case 'd':
            printf("=======================================\n");
            printf("QUANTIDADE DE CARROS VENDIDOS COM SOMA DOS PRECOS\n");
            break;

        case 'e':
            printf("=======================================\n");
            printf("LUCRO TOTAL DE VENDAS\n");
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