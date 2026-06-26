#ifndef PRODUTO_HPP
#define PRODUTO_HPP
#include <string>
#include "dynamicarray.hpp"


struct Produto
{
int id;
std::string nome;
double preco;
int qtd;          // estoque atual — muda com compras e reposições
std::string categoria;
std::string marca;
std::string condicao;

// historico de compradores
DynamicArray<int> usuariosCompradores;  // ids dos usuários (ordenado)
DynamicArray<int> qtdUsuarios;          // qtd total comprada por cada um

//métodos
Produto();
Produto(int id, std::string nome, double preco, int qtd,
        std::string categoria, std::string marca,
        std::string condicao);

void adicionarComprador(int id_usuario, int qtd); // mesmo padrão do Usuario
void diminuirEstoque(int qtd);   // chamado quando compra é válida
void aumentarEstoque(int qtd);   // chamado em reposição

void imprimirDados(int posicao) const;   // linha LP resultado_i
void imprimirUsuarios() const;           // segunda linha com compradores


};


#endif