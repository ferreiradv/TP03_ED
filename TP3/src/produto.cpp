
#include <iostream>
#include "Produto.hpp"
#include <iomanip>  // std::fixed, std::setprecision


//construtores

//padrão
//necessário para quando Produto for guardado em um DynamicArray<Produto>
Produto::Produto()
    : id(-1), nome(""), preco(0.0), qtd(0),
      categoria(""), marca(""), condicao("")
{
}

Produto::Produto(int id, const std::string& nome, double preco, int qtd,
                 const std::string& categoria, const std::string& marca,
                 const std::string& condicao)
    : id(id), nome(nome), preco(preco), qtd(qtd),
      categoria(categoria), marca(marca), condicao(condicao)
{
}

//getters
int         Produto::getId()        const { return id; }
std::string Produto::getNome()      const { return nome; }
double      Produto::getPreco()     const { return preco; }
int         Produto::getQtd()       const { return qtd; }
std::string Produto::getCategoria() const { return categoria; }
std::string Produto::getMarca()     const { return marca; }
std::string Produto::getCondicao()  const { return condicao; }


//Atualizador de estoqwue
// Chamado pelo Sistema após verificar se a conta é válida
// A validação (qtd disponível >= qtd pedida) é responsabilidade do sistema,
// não do Produto. Portanto, apenas decrementamos o estoque.
void Produto::diminuirEstoque(int quantidade)
{
    qtd -= quantidade;
}

// Chamado pelo sistema ao registrar uma reposição.
// como toda reposição é válida, não há necessidade de validação.
void Produto::aumentarEstoque(int quantidade)
{
    qtd += quantidade;
}

//adiciona compradores

// Lógica idêntica ao adicionarCompra do Usuario:
// mantém usuariosCompradores em ordem crescente de id_usuario
// e acumula quantidades para usuários repetidos.
//
// Custo: O(n) onde n = número de usuários distintos que compraram este produto.
void Produto::adicionarComprador(int id_usuario, int quantidade)
{
    int n = usuariosCompradores.getSize();

    for (int i = 0; i < n; i++)
    {
        if (usuariosCompradores[i] == id_usuario)
        {
            // Usuário já comprou antes: acumula quantidade.
            qtdComprada[i] += quantidade;
            return;
        }

        if (usuariosCompradores[i] > id_usuario)
        {
            // Posição de inserção encontrada no meio.
            usuariosCompradores.insert(i, id_usuario);
            qtdComprada.insert(i, quantidade);
            return;
        }
    }

    // id_usuario é maior que todos os existentes: insere no fim.
    usuariosCompradores.push_back(id_usuario);
    qtdComprada.push_back(quantidade);
}

//impressão

// Exemplo de saída:
// LP resultado_1 produto 0 Mouse 80.00 9 Informatica Logitech Novo
void Produto::imprimirDados(int posicao) const
{
    std::cout << "LP resultado_" << posicao
              << " produto "     << id
              << " "             << nome
              << " "             << std::fixed << std::setprecision(2) << preco
              << " "             << qtd
              << " "             << categoria
              << " "             << marca
              << " "             << condicao
              << "\n";
}

// Imprime a linha de usuários compradores, somente se houver algum.
// Exemplo de saída:
// usuario_1 0 2 usuario_2 1 4
void Produto::imprimirUsuarios() const
{
    int n = usuariosCompradores.getSize();

    if (n == 0)
    {
        return; // segunda linha omitida conforme especificação
    }

    for (int i = 0; i < n; i++)
    {
        if (i > 0)
        {
            std::cout << " ";
        }

        std::cout << "usuario_" << (i + 1)
                  << " "        << usuariosCompradores[i]
                  << " "        << qtdComprada[i];
    }

    std::cout << "\n";
}