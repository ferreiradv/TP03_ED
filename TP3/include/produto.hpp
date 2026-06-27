#ifndef PRODUTO_HPP
#define PRODUTO_HPP

#include <string>
#include "DynamicArray.hpp"

class Produto
{
private:

    int id;
    std::string nome;
    double preco;
    int qtd;              // estoque atual
    std::string categoria;
    std::string marca;
    std::string condicao;

    // Histórico agregado de compradores:
    // usuariosCompradores[i] e qtdComprada[i] formam um par (id_usuario, qtd_total).
    // Mantidos em ordem crescente de id_usuario.
    DynamicArray<int> usuariosCompradores;
    DynamicArray<int> qtdComprada;

public:

    // Construtores
    Produto();
    Produto(int id, const std::string& nome, double preco, int qtd,
            const std::string& categoria, const std::string& marca,
            const std::string& condicao);

    // Getters
    int         getId()         const;
    std::string getNome()       const;
    double      getPreco()      const;
    int         getQtd()        const;
    std::string getCategoria()  const;
    std::string getMarca()      const;
    std::string getCondicao()   const;

    // gerenciamento de estoque
    void diminuirEstoque(int qtd);
    void aumentarEstoque(int qtd);

    // Registra que o usuário 'id_usuario' comprou 'qtd' unidades deste produto.
    // Se o usuário já está no histórico, acumula a quantidade.
    // Se não está, insere mantendo ordem crescente de id_usuario.
    void adicionarComprador(int id_usuario, int qtd);

    // Imprime a linha principal do resultado LP:
    // "LP resultado_<posicao> produto <id> <nome> <preco> <qtd> <categoria> <marca> <condicao>"
    void imprimirDados(int posicao) const;

    // Imprime a segunda linha com os usuários compradores (somente se existirem):
    // "usuario_1 <id_u1> <qtd_u1> usuario_2 <id_u2> <qtd_u2> ..."
    void imprimirUsuarios() const;
};

#endif