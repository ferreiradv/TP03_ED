#include "Compra.hpp"
#include <iostream>


//construtoes
// Construtor padrão: necessário para DynamicArray<Compra>.
Compra::Compra()
    : id(-1), timestamp(-1), id_usuario(-1)
{
}

Compra::Compra(int id, int timestamp, int id_usuario)
    : id(id), timestamp(timestamp), id_usuario(id_usuario)
{
}

//getters

int Compra::getId()         const { return id; }
int Compra::getTimestamp()  const { return timestamp; }
int Compra::getIdUsuario()  const { return id_usuario; }

int Compra::getNumProdutos()        const { return id_produtos.getSize(); }
int Compra::getIdProduto(int i)     const { return id_produtos[i]; }
int Compra::getQtdProduto(int i)    const { return qtd_produtos[i]; }

//adiciona produtos

// Insere o par (id_produto, qtd) mantendo a ordem crescente de id_produto.

// observação:
// o enunciado garante que um mesmo produto NÃO se repete dentro de uma compra,
// portanto não há caso de "produto já existe — acumular". Apenas inserimos
// na posição correta para manter a ordem.
//
// Custo: O(k) onde k = número de produtos distintos nesta compra.
void Compra::adicionarProduto(int id_produto, int qtd)
{
    int n = id_produtos.getSize();

    for (int i = 0; i < n; i++)
    {
        if (id_produtos[i] > id_produto)
        {
            // Posição de inserção encontrada no meio.
            id_produtos.insert(i, id_produto);
            qtd_produtos.insert(i, qtd);
            return;
        }
    }

    // id_produto é maior que todos os existentes: insere no fim.
    id_produtos.push_back(id_produto);
    qtd_produtos.push_back(qtd);
}

//impressão

// Exemplo de saída (primeira linha):
// LC resultado_1 compra 0 timestamp 100 usuario 0
void Compra::imprimirDados(int posicao) const
{
    std::cout << "LC resultado_" << posicao
              << " compra "      << id
              << " timestamp "   << timestamp
              << " usuario "     << id_usuario
              << "\n";
}

// Imprime a segunda linha com os produtos da compra.
// Esta linha é SEMPRE impressa para compras (toda compra válida tem ao menos um produto).
// Exemplo de saída:
// produto_1 0 2 produto_2 1 1
void Compra::imprimirProdutos() const
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