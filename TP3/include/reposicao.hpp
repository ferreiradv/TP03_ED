#ifndef REPOSICAO_HPP
#define REPOSICAO_HPP

#include "DynamicArray.hpp"

class Reposicao
{
private:

    int id;
    int timestamp;

    // Itens desta reposição.
    // id_produtos[i] e qtd_produtos[i] formam um par (id_produto, qtd).
    // Mantidos em ordem crescente de id_produto.
    DynamicArray<int> id_produtos;
    DynamicArray<int> qtd_produtos;

public:

    // Construtores
    Reposicao();
    Reposicao(int id, int timestamp);

    // Getters
    int getId()         const;
    int getTimestamp()  const;

    // Getters dos itens
    int getNumProdutos()        const;
    int getIdProduto(int i)     const;
    int getQtdProduto(int i)    const;

    // Adiciona o par (id_produto, qtd) à reposição, mantendo ordem crescente.
    // O enunciado garante que um mesmo produto não se repete dentro de uma reposição
    void adicionarProduto(int id_produto, int qtd);

    // Imprime a primeira linha do resultado LR:
    // "LR resultado_<posicao> reposicao <id> timestamp <timestamp>"
    void imprimirDados(int posicao) const;

    // Imprime a segunda linha com os produtos da reposição (sempre presente):
    // "produto_1 <id_p1> <qtd_p1> produto_2 <id_p2> <qtd_p2> ..."
    void imprimirProdutos() const;
};
#endif
