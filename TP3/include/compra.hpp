#ifndef COMPRA_HPP
#define COMPRA_HPP
#include <string>
#include "dynamicarray.hpp"


struct Compra{
    int id;
    int timestamp;
    int id_usuario;
    DynamicArray<int> id_produtos;   // ordenado por id de produto
    DynamicArray<int> qtd_produtos;  // qtd no mesmo índice

Compra();
Compra(int id, int timestamp, int id_usuario);

//métodos
// Adiciona um par produto/qtd à compra (em ordem crescente de id_produto)
void adicionarProduto(int id_produto, int qtd);

void imprimirDados(int posicao) const;  // primeira linha LC resultado_i
void imprimirProdutos() const;          // segunda linha (sempre presente)


};







#endif