#ifndef GERADOR_H_INCLUDED
#define GERADOR_H_INCLUDED

#include "revenda.h" // Necessita das structs CARRO e CLIENTE

void gerar_placa(char placa[9]);
void gerar_dados_carro(struct CARRO *carro);
void mostrar_carro(const struct CARRO *carro);
void gerar_dados_clientes(struct CLIENTE *cliente);
void mostrar_cliente(const struct CLIENTE *cli);


#endif // GERADOR_H_INCLUDED