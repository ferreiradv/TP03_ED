#ifndef COMPRA_HPP
#define COMPRA_HPP

#include "DynamicArray.hpp"

class Compra
{
private:

    int id;
    int timestamp;
    int id_usuario;

    // Itens desta transação específica.
    // id_produtos[i] e qtd_produtos[i] formam um par (id_produto, qtd).
    // Mantidos em ordem crescente de id_produto.
    DynamicArray<int> id_produtos;
    DynamicArray<int> qtd_produtos;

public:

    // Construtores
    Compra();
    Compra(int id, int timestamp, int id_usuario);

    // Getters
    int getId()         const;
    int getTimestamp()  const;
    int getIdUsuario()  const;

    // Getters dos itens (necessários para o Sistema montar índices invertidos
    // e para verificar se um produto específico está nesta compra)
    int getNumProdutos()            const;
    int getIdProduto(int i)         const;
    int getQtdProduto(int i)        const;

    // Adiciona o par (id_produto, qtd) à compra, mantendo ordem crescente de id_produto.
    // O enunciado garante que um mesmo produto não se repete dentro de uma compra,
    // portanto não há necessidade de acumular — apenas inserir na posição correta.
    void adicionarProduto(int id_produto, int qtd);

    // Imprime a primeira linha do resultado LC:
    // "LC resultado_<posicao> compra <id> timestamp <timestamp> usuario <id_usuario>"
    void imprimirDados(int posicao) const;

    // Imprime a segunda linha com os produtos da compra (sempre presente):
    // "produto_1 <id_p1> <qtd_p1> produto_2 <id_p2> <qtd_p2> ..."
    void imprimirProdutos() const;
};

#endif