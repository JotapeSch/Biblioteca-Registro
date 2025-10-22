#ifdef AUXILIAR_H_INCLUDED
#define AUXILIAR_H_INCLUDED

#include "revenda.h"


struct CARRO *buscar_carro_por_placa(const char *placa);
struct CARRO *buscar_cliente_por_codigo(int codigo);
int carro_foi_vendido(const char *placa);

#endif