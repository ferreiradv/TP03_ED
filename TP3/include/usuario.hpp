#ifndef USUARIO_HPP
#define USUARIO_HPP

#include <string>
#include "DynamicArray.hpp"

class Usuario
{
private:

    int id;
    std::string nome;
    int idade;
    std::string cidade;
    std::string estado;
    std::string nacionalidade;

    // Histórico agregado de compras:
    // produtosComprados[i] e qtdComprada[i] formam um par (id_produto, qtd_total).
    // mantém a ordem crescente de id_produto.
    DynamicArray<int> produtosComprados;
    DynamicArray<int> qtdComprada;

public:

    // Construtores
    Usuario();
    Usuario(int id, const std::string& nome, int idade,
            const std::string& cidade, const std::string& estado,
            const std::string& nacionalidade);

    // Getters
    int getId()                     const;
    std::string getNome()           const;
    int getIdade()                  const;
    std::string getCidade()         const;
    std::string getEstado()         const;
    std::string getNacionalidade()  const;

    // Registra a compra de 'qtd' unidades do produto 'id_produto'.
    // Se o produto já está no histórico, acumula a quantidade.
    // Se não está, insere mantendo a ordem crescente de id_produto.
    void adicionarCompra(int id_produto, int qtd);

    // Imprime a linha principal do resultado LU:
    // "LU resultado_<posicao> usuario <id> <nome> <idade> <cidade> <estado> <nacionalidade>"
    void imprimirDados(int posicao) const;

    // Imprime a segunda linha com os produtos comprados (somente se existirem):
    // "produto_1 <id_p1> <qtd_p1> produto_2 <id_p2> <qtd_p2> ..."
    void imprimirProdutos() const;
};

#endif