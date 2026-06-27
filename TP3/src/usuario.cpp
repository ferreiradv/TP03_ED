#include <iostream>
#include "usuario.hpp"


// ─────────────────────────────────────────────
// Construtores
// ─────────────────────────────────────────────

// Construtor padrão: inicializa com valores neutros.
// Necessário para quando Usuario for guardado em um DynamicArray<Usuario>
// (o DynamicArray aloca com "new T[n]", o que exige construtor padrão).
Usuario::Usuario()
    : id(-1), nome(""), idade(0), cidade(""), estado(""), nacionalidade("")
{
}

Usuario::Usuario(int id, const std::string& nome, int idade,
                 const std::string& cidade, const std::string& estado,
                 const std::string& nacionalidade)
    : id(id), nome(nome), idade(idade),
      cidade(cidade), estado(estado), nacionalidade(nacionalidade)
{
    // produtosComprados e qtdComprada são inicializados automaticamente
    // pelos seus próprios construtores padrão (DynamicArray()).
}


// Getters
int         Usuario::getId()                    const { return id; }
std::string Usuario::getNome()                  const { return nome; }
int         Usuario::getIdade()                 const { return idade; }
std::string Usuario::getCidade()                const { return cidade; }
std::string Usuario::getEstado()                const { return estado; }
std::string Usuario::getNacionalidade()         const { return nacionalidade; }


// adicionarCompra

// Registra que o usuário comprou 'qtd' unidades do produto 'id_produto'.
//
// Estratégia: busca linear no array produtosComprados (mantido em ordem
// crescente de id_produto). Duas situações:
//
//   1) Produto já existe no histórico → apenas acumula a quantidade.
//   2) Produto ainda não existe → insere o par (id_produto, qtd) na posição
//      correta para manter a ordenação.
//
// Custo: O(n) no pior caso (n = número de produtos distintos já comprados),
// tanto pela busca linear quanto pelo shift do insert.
// Na prática n é pequeno por usuário, então isso é aceitável.
void Usuario::adicionarCompra(int id_produto, int qtd)
{
    int n = produtosComprados.getSize();

    // Percorre o array procurando o produto ou a posição de inserção.
    for (int i = 0; i < n; i++)
    {
        if (produtosComprados[i] == id_produto)
        {
            // Situação 1: produto já comprado antes —> acumula quantidade.
            qtdComprada[i] += qtd;
            return;
        }

        if (produtosComprados[i] > id_produto)
        {
            // Situação 2a: encontrou posição de inserção no meio do array.
            // insert() do DynamicArray faz o shift dos elementos à direita.
            produtosComprados.insert(i, id_produto);
            qtdComprada.insert(i, qtd);
            return;
        }
    }

    // Situação 2b: id_produto é maior que todos os existentes -> insere no fim.
    produtosComprados.push_back(id_produto);
    qtdComprada.push_back(qtd);
}


// Impressão
// Exemplo de saída:
// LU resultado_1 usuario 0 Ana 21 BeloHorizonte MG Brasileira
void Usuario::imprimirDados(int posicao) const
{
    std::cout << "LU resultado_" << posicao
              << " usuario "     << id
              << " "             << nome
              << " "             << idade
              << " "             << cidade
              << " "             << estado
              << " "             << nacionalidade
              << "\n";
}

// Imprime a linha de produtos comprados, somente se o usuário tiver histórico.
// Exemplo de saída:
// produto_1 0 1 produto_2 1 2
//
// Não imprime nada se o usuário nunca comprou — o enunciado especifica que
// a segunda linha só aparece quando há produtos.
void Usuario::imprimirProdutos() const
{
    int n = produtosComprados.getSize();

    if (n == 0)
    {
        return; // segunda linha omitida conforme especificação
    }

    for (int i = 0; i < n; i++)
    {
        // Separador: não imprime espaço antes do primeiro item
        if (i > 0)
        {
            std::cout << " ";
        }

        std::cout << "produto_" << (i + 1)
                  << " "        << produtosComprados[i]
                  << " "        << qtdComprada[i];
    }

    std::cout << "\n";
}