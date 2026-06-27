#ifndef MERCADODCC_HPP
#define MERCADODCC_HPP

#include <string>
#include "DynamicArray.hpp"
#include "InvertedIndex.hpp"
#include "Usuario.hpp"
#include "Produto.hpp"
#include "Compra.hpp"
#include "Reposicao.hpp"

class MercadoDCC
{
private:

    // ── Repositórios principais ───────────────────────────────────────────────
    // Acesso por ID = acesso por índice direto no array → O(1).
    // usuarios[i] é o usuário de id i, produtos[i] é o produto de id i, etc.
    DynamicArray<Usuario>   usuarios;
    DynamicArray<Produto>   produtos;
    DynamicArray<Compra>    compras;
    DynamicArray<Reposicao> reposicoes;

    // ── Contadores de IDs ─────────────────────────────────────────────────────
    int proxIdUsuario;
    int proxIdProduto;
    int proxIdCompra;
    int proxIdReposicao;

    // ── Índices invertidos de Usuário ─────────────────────────────────────────
    InvertedIndex idxUsuarioNome;
    InvertedIndex idxUsuarioIdade;
    InvertedIndex idxUsuarioCidade;
    InvertedIndex idxUsuarioEstado;
    InvertedIndex idxUsuarioNacionalidade;

    // ── Índices invertidos de Produto ─────────────────────────────────────────
    InvertedIndex idxProdutoNome;
    InvertedIndex idxProdutoCategoria;
    InvertedIndex idxProdutoMarca;
    InvertedIndex idxProdutoCondicao;

    // ── Índices invertidos de Compra ──────────────────────────────────────────
    InvertedIndex idxCompraTimestamp;
    InvertedIndex idxCompraUsuario;
    InvertedIndex idxCompraProduto;

    // ── Índices invertidos de Reposição ───────────────────────────────────────
    InvertedIndex idxReposicaoTimestamp;
    InvertedIndex idxReposicaoProduto;

    // ── Métodos privados auxiliares ───────────────────────────────────────────

    // Resolve uma lista de pares (atributo, valor) para um tipo de entidade
    // ('U', 'P', 'C', 'R') e retorna os IDs que satisfazem todos os filtros.
    // Se algum atributo não tiver resultados, retorna lista vazia imediatamente.
    // Caso especial: atributo "id" resolve diretamente sem usar índice invertido.
    DynamicArray<int> resolverFiltros(
        const DynamicArray<std::string>& atributos,
        const DynamicArray<std::string>& valores,
        char tipo) const;

    // Converte inteiro para string (evita std::to_string que pode não estar
    // disponível em todos os ambientes C++11).
    std::string intParaString(int n) const;

    // Processa os comandos de cadastro
    void cadastrarUsuario();
    void cadastrarProduto();
    void registrarReposicao();
    void registrarCompra();

    // Processa os comandos de consulta
    void consultarUsuarios();
    void consultarProdutos();
    void consultarCompras();
    void consultarReposicoes();

public:

    MercadoDCC();

    // Loop principal: lê comandos de std::cin até EOF e processa cada um.
    void processarEntrada();
};

#endif