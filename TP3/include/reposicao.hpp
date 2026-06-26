#ifndef REPOSICAO_HPP
#define REPOSICAO_HPP

#include <string>
#include "dynamicarray.hpp" 

struct Reposicao{
    int id;
    int timestamp;
    DynamicArray<int> id_produtos;
    DynamicArray<int> qtd_produtos;

        //métodos
    Reposicao();
    Reposicao(int id, int timestamp);

    void adicionarProduto(int id_produto, int qtd);

    void imprimirDados(int posicao) const;  // primeira linha LR resultado_i
    void imprimirProdutos() const;          // segunda linha (sempre presente)



};









#endif