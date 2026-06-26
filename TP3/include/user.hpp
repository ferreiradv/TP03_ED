#ifndef USER_HPP
#define USER_HPP
#include <string>
#include "dynamicarray.hpp"

struct User
{
int id;
std::string nome;
int idade;
std::string cidade;
std::string estado;
std::string nacionalidade;

// historico de compras
DynamicArray<int> produtosComprados;    // ids dos produtos (ordenado)
DynamicArray<int> qtdProdutosComprados; // qtd total por produto (mesmo índice)


//métodos  
    User();
    User(int id, std::string nome, int idade,
            std::string cidade, std::string estado,
            std::string nacionalidade);

    // Adiciona qtd ao produto id_produto no histórico agregado.
    // Se o produto já existe, soma a quantidade.
    // Se não existe, insere mantendo ordem crescente de id.
    void adicionarCompra(int id_produto, int qtd);

    // Imprime a linha principal do LU resultado_i
    void imprimirDados(int posicao) const;

    // Imprime a segunda linha com produtos (só se tiver comprado algo)
    void imprimirProdutos() const;
    };
#endif 