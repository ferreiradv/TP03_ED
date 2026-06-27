#include "Reposicao.hpp"
#include <iostream>


// Construtor padrão: necessário para DynamicArray<Reposicao>.
Reposicao::Reposicao()
    : id(-1), timestamp(-1)
{
}

Reposicao::Reposicao(int id, int timestamp): id(id), timestamp(timestamp){
}

//getters

int Reposicao::getId()        const { return id; }
int Reposicao::getTimestamp() const { return timestamp; }

int Reposicao::getNumProdutos()     const { return id_produtos.getSize(); }
int Reposicao::getIdProduto(int i)  const { return id_produtos[i]; }
int Reposicao::getQtdProduto(int i) const { return qtd_produtos[i]; }

//adicionar produto

// Idêntico ao da Compra: insere mantendo ordem crescente de id_produto.
// O enunciado garante que um produto não se repete na mesma reposição,
// portanto não há acumulação — apenas inserção ordenada.
//
// Custo: O(k) onde k = número de produtos nesta reposição.
void Reposicao::adicionarProduto(int id_produto, int qtd)
{
    int n = id_produtos.getSize();

    for (int i = 0; i < n; i++)
    {
        if (id_produtos[i] > id_produto)
        {
            id_produtos.insert(i, id_produto);
            qtd_produtos.insert(i, qtd);
            return;
        }
    }

    id_produtos.push_back(id_produto);
    qtd_produtos.push_back(qtd);
}

//impressão para consultas
// Exemplo de saída (primeira linha):
// LR resultado_1 reposicao 0 timestamp 125
void Reposicao::imprimirDados(int posicao) const
{
    std::cout << "LR resultado_" << posicao
              << " reposicao "   << id
              << " timestamp "   << timestamp
              << "\n";
}

// Imprime a segunda linha com os produtos da reposição.
// Esta linha é SEMPRE impressa (toda reposição tem ao menos um produto).
// Exemplo de saída:
// produto_1 0 3
void Reposicao::imprimirProdutos() const
{
    int n = id_produtos.getSize();

    for (int i = 0; i < n; i++)
    {
        if (i > 0)
        {
            std::cout << " ";
        }

        std::cout << "produto_" << (i + 1)
                  << " "        << id_produtos[i]
                  << " "        << qtd_produtos[i];
    }

    std::cout << "\n";
}