#include "mercado.hpp"
#include <iostream>
#include <string>

// ─────────────────────────────────────────────────────────────────────────────
// Construtor
// ─────────────────────────────────────────────────────────────────────────────

MercadoDCC::MercadoDCC()
    : proxIdUsuario(0),
      proxIdProduto(0),
      proxIdCompra(0),
      proxIdReposicao(0)
{
}

// ─────────────────────────────────────────────────────────────────────────────
// Auxiliar: intParaString
// Converte inteiro para string sem depender de std::to_string,
// que pode ter comportamento inconsistente em alguns compiladores C++11.
// ─────────────────────────────────────────────────────────────────────────────

std::string MercadoDCC::intParaString(int n) const
{
    if (n == 0) return "0";

    std::string resultado = "";
    bool negativo = (n < 0);
    if (negativo) n = -n;

    while (n > 0)
    {
        resultado = (char)('0' + n % 10) + resultado;
        n /= 10;
    }

    if (negativo) resultado = "-" + resultado;
    return resultado;
}

// ─────────────────────────────────────────────────────────────────────────────
// Auxiliar: resolverFiltros
//
// Recebe os pares (atributo, valor) de uma consulta e retorna a lista de IDs
// das entidades que satisfazem TODOS os filtros simultaneamente (interseção).
//
// Estratégia:
//   1. Para o atributo "id": resolve diretamente (sem índice invertido).
//   2. Para os demais atributos: busca no índice invertido correspondente.
//   3. Faz interseção sucessiva entre os resultados de cada filtro.
//   4. Se qualquer filtro retornar lista vazia, interrompe imediatamente.
//
// O parâmetro 'tipo' seleciona o conjunto de índices a usar:
//   'U' → índices de usuário
//   'P' → índices de produto
//   'C' → índices de compra
//   'R' → índices de reposição
// ─────────────────────────────────────────────────────────────────────────────

DynamicArray<int> MercadoDCC::resolverFiltros(
    const DynamicArray<std::string>& atributos,
    const DynamicArray<std::string>& valores,
    char tipo) const
{
    int numFiltros = atributos.getSize();

    // Lista de trabalho: começa com todos os IDs existentes do tipo,
    // e vai sendo reduzida a cada filtro aplicado.
    // Inicializamos com nullptr — o primeiro filtro define o conjunto inicial.
    DynamicArray<int> resultado;
    bool primeiroFiltro = true;

    for (int f = 0; f < numFiltros; f++)
    {
        const std::string& atrib = atributos[f];
        const std::string& valor = valores[f];

        DynamicArray<int> candidatos;

        // ── Caso especial: filtro por "id" ────────────────────────────────────
        // O ID é único, então basta verificar se existe e está no intervalo.
        if (atrib == "id")
        {
            int idBuscado = 0;
            for (int i = 0; i < (int)valor.size(); i++)
                idBuscado = idBuscado * 10 + (valor[i] - '0');

            bool existe = false;
            if (tipo == 'U' && idBuscado < usuarios.getSize())   existe = true;
            if (tipo == 'P' && idBuscado < produtos.getSize())   existe = true;
            if (tipo == 'C' && idBuscado < compras.getSize())    existe = true;
            if (tipo == 'R' && idBuscado < reposicoes.getSize()) existe = true;

            if (existe) candidatos.push_back(idBuscado);
            // Se não existe, candidatos fica vazio → resultado será vazio.
        }

        // ── Filtros por índice invertido ──────────────────────────────────────
        else
        {
            const InvertedIndex* idx = nullptr;

            if (tipo == 'U')
            {
                if      (atrib == "nome")           idx = &idxUsuarioNome;
                else if (atrib == "idade")          idx = &idxUsuarioIdade;
                else if (atrib == "cidade")         idx = &idxUsuarioCidade;
                else if (atrib == "estado")         idx = &idxUsuarioEstado;
                else if (atrib == "nacionalidade")  idx = &idxUsuarioNacionalidade;
            }
            else if (tipo == 'P')
            {
                if      (atrib == "nome")       idx = &idxProdutoNome;
                else if (atrib == "categoria")  idx = &idxProdutoCategoria;
                else if (atrib == "marca")      idx = &idxProdutoMarca;
                else if (atrib == "condicao")   idx = &idxProdutoCondicao;
            }
            else if (tipo == 'C')
            {
                if      (atrib == "timestamp")   idx = &idxCompraTimestamp;
                else if (atrib == "id_usuario")  idx = &idxCompraUsuario;
                else if (atrib == "id_produto")  idx = &idxCompraProduto;
            }
            else if (tipo == 'R')
            {
                if      (atrib == "timestamp")  idx = &idxReposicaoTimestamp;
                else if (atrib == "id_produto") idx = &idxReposicaoProduto;
            }

            if (idx != nullptr)
            {
                const DynamicArray<int>* lista = idx->getIds(valor);

                if (lista != nullptr)
                {
                    // Copia a lista para candidatos
                    for (int i = 0; i < lista->getSize(); i++)
                        candidatos.push_back((*lista)[i]);
                }
                // Se lista == nullptr, candidatos fica vazio.
            }
        }

        // ── Aplica o filtro ao resultado acumulado ────────────────────────────
        if (primeiroFiltro)
        {
            resultado = candidatos;
            primeiroFiltro = false;
        }
        else
        {
            resultado = intersectIds(resultado, candidatos);
        }

        // Otimização: se o resultado já está vazio, não há como adicionar mais.
        if (resultado.getSize() == 0)
            return resultado;
    }

    // Caso sem filtros (não deve ocorrer segundo o enunciado, mas por robustez):
    // retorna todos os IDs do tipo.
    if (primeiroFiltro)
    {
        if (tipo == 'U')
            for (int i = 0; i < usuarios.getSize();   i++) resultado.push_back(i);
        else if (tipo == 'P')
            for (int i = 0; i < produtos.getSize();   i++) resultado.push_back(i);
        else if (tipo == 'C')
            for (int i = 0; i < compras.getSize();    i++) resultado.push_back(i);
        else if (tipo == 'R')
            for (int i = 0; i < reposicoes.getSize(); i++) resultado.push_back(i);
    }

    return resultado;
}

// ─────────────────────────────────────────────────────────────────────────────
// Loop principal
// ─────────────────────────────────────────────────────────────────────────────

void MercadoDCC::processarEntrada()
{
    std::string comando;

    while (std::cin >> comando)
    {
        if      (comando == "U")  cadastrarUsuario();
        else if (comando == "P")  cadastrarProduto();
        else if (comando == "R")  registrarReposicao();
        else if (comando == "C")  registrarCompra();
        else if (comando == "LU") consultarUsuarios();
        else if (comando == "LP") consultarProdutos();
        else if (comando == "LC") consultarCompras();
        else if (comando == "LR") consultarReposicoes();
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// U — Cadastrar usuário
//
// Entrada:  U <nome> <idade> <cidade> <estado> <nacionalidade>
// Saída:    U <id_usuario>
//
// Atribui o próximo ID disponível, armazena o usuário e atualiza
// todos os índices invertidos dos atributos buscáveis.
// ─────────────────────────────────────────────────────────────────────────────

void MercadoDCC::cadastrarUsuario()
{
    std::string nome, cidade, estado, nacionalidade;
    int idade;

    std::cin >> nome >> idade >> cidade >> estado >> nacionalidade;

    int id = proxIdUsuario++;

    Usuario u(id, nome, idade, cidade, estado, nacionalidade);
    usuarios.push_back(u);

    // Atualiza índices invertidos.
    // A idade é armazenada como string no índice (chaves são sempre strings).
    idxUsuarioNome.add(nome, id);
    idxUsuarioIdade.add(intParaString(idade), id);
    idxUsuarioCidade.add(cidade, id);
    idxUsuarioEstado.add(estado, id);
    idxUsuarioNacionalidade.add(nacionalidade, id);

    std::cout << "U " << id << "\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// P — Cadastrar produto
//
// Entrada:  P <nome> <preco> <qtd_inicial> <categoria> <marca> <condicao>
// Saída:    P <id_produto>
// ─────────────────────────────────────────────────────────────────────────────

void MercadoDCC::cadastrarProduto()
{
    std::string nome, categoria, marca, condicao;
    double preco;
    int qtd;

    std::cin >> nome >> preco >> qtd >> categoria >> marca >> condicao;

    int id = proxIdProduto++;

    Produto p(id, nome, preco, qtd, categoria, marca, condicao);
    produtos.push_back(p);

    // qtd e preco NÃO são indexados (enunciado: não usados como atributo de busca).
    idxProdutoNome.add(nome, id);
    idxProdutoCategoria.add(categoria, id);
    idxProdutoMarca.add(marca, id);
    idxProdutoCondicao.add(condicao, id);

    std::cout << "P " << id << "\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// R — Registrar reposição de estoque
//
// Entrada:  R <timestamp> <id_prod_1> <qtd_1> ... <id_prod_k> <qtd_k>
// Saída:    R <id_reposicao>
//
// Toda reposição é válida. Incrementa o estoque dos produtos e registra
// a reposição com seus índices.
//
// Como a entrada de uma reposição tem número variável de pares (produto, qtd),
// lemos o restante da linha caractere a caractere até encontrar '\n'.
// ─────────────────────────────────────────────────────────────────────────────

void MercadoDCC::registrarReposicao()
{
    int timestamp;
    std::cin >> timestamp;

    int id = proxIdReposicao++;
    Reposicao r(id, timestamp);

    // Lê pares (id_produto, qtd) até o fim da linha.
    int idProd, qtd;
    while (std::cin.peek() != '\n' && std::cin.peek() != EOF && std::cin >> idProd >> qtd)
    {
        r.adicionarProduto(idProd, qtd);
        produtos[idProd].aumentarEstoque(qtd);
    }

    // Atualiza índices da reposição.
    idxReposicaoTimestamp.add(intParaString(timestamp), id);

    for (int i = 0; i < r.getNumProdutos(); i++)
        idxReposicaoProduto.add(intParaString(r.getIdProduto(i)), id);

    reposicoes.push_back(r);

    std::cout << "R " << id << "\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// C — Registrar compra
//
// Entrada:  C <timestamp> <id_usuario> <id_prod_1> <qtd_1> ... <id_prod_k> <qtd_k>
// Saída:    C <id_compra>  (se válida)
//           C INV          (se inválida)
//
// Válida se e somente se há estoque suficiente para TODOS os produtos.
// Se inválida, nenhum estado é alterado.
// ─────────────────────────────────────────────────────────────────────────────

void MercadoDCC::registrarCompra()
{
    int timestamp, idUsuario;
    std::cin >> timestamp >> idUsuario;

    // Lê todos os pares (id_produto, qtd) da linha antes de validar,
    // pois precisamos checar todos os produtos antes de alterar qualquer estoque.
    DynamicArray<int> idsProd;
    DynamicArray<int> qtds;

    int idProd, qtd;
    while (std::cin.peek() != '\n' && std::cin.peek() != EOF && std::cin >> idProd >> qtd)
    {
        idsProd.push_back(idProd);
        qtds.push_back(qtd);
    }

    // Validação: verifica estoque de todos os produtos antes de alterar qualquer um.
    for (int i = 0; i < idsProd.getSize(); i++)
    {
        if (produtos[idsProd[i]].getQtd() < qtds[i])
        {
            std::cout << "C INV\n";
            return;
        }
    }

    // Compra válida: cria a entidade, atualiza estoques e índices.
    int id = proxIdCompra++;
    Compra c(id, timestamp, idUsuario);

    for (int i = 0; i < idsProd.getSize(); i++)
    {
        int ip = idsProd[i];
        int q  = qtds[i];

        c.adicionarProduto(ip, q);

        // Atualiza estoque do produto.
        produtos[ip].diminuirEstoque(q);

        // Atualiza histórico agregado: usuário → produtos comprados.
        usuarios[idUsuario].adicionarCompra(ip, q);

        // Atualiza histórico agregado: produto → usuários compradores.
        produtos[ip].adicionarComprador(idUsuario, q);

        // Atualiza índice invertido de compras por produto.
        idxCompraProduto.add(intParaString(ip), id);
    }

    // Atualiza índices de compra por timestamp e por usuário.
    idxCompraTimestamp.add(intParaString(timestamp), id);
    idxCompraUsuario.add(intParaString(idUsuario), id);

    compras.push_back(c);

    std::cout << "C " << id << "\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// LU — Consultar usuários
//
// Entrada:  LU <atrib_1> <valor_1> ... <atrib_k> <valor_k>
// Saída:    Uma ou duas linhas por usuário encontrado, em ordem crescente de id.
//           LU VAZIO se nenhum usuário satisfaz os filtros.
//
// Lê os pares atributo/valor do restante da linha, resolve os filtros
// e imprime cada usuário encontrado com seu histórico de compras.
// ─────────────────────────────────────────────────────────────────────────────

void MercadoDCC::consultarUsuarios()
{
    DynamicArray<std::string> atributos;
    DynamicArray<std::string> valores;

    std::string token;
    while (std::cin.peek() != '\n' && std::cin.peek() != EOF && std::cin >> token)
    {
        atributos.push_back(token);
        std::cin >> token;
        valores.push_back(token);
    }

    DynamicArray<int> ids = resolverFiltros(atributos, valores, 'U');

    if (ids.getSize() == 0)
    {
        std::cout << "LU VAZIO\n";
        return;
    }

    // ids já está em ordem crescente (vem dos índices invertidos que mantêm ordem).
    for (int i = 0; i < ids.getSize(); i++)
    {
        int posicao = i + 1;
        usuarios[ids[i]].imprimirDados(posicao);
        usuarios[ids[i]].imprimirProdutos();
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// LP — Consultar produtos
//
// Entrada:  LP <atrib_1> <valor_1> ... <atrib_k> <valor_k>
// Saída:    Uma ou duas linhas por produto encontrado, em ordem crescente de id.
//           LP VAZIO se nenhum produto satisfaz os filtros.
// ─────────────────────────────────────────────────────────────────────────────

void MercadoDCC::consultarProdutos()
{
    DynamicArray<std::string> atributos;
    DynamicArray<std::string> valores;

    std::string token;
    while (std::cin.peek() != '\n' && std::cin.peek() != EOF && std::cin >> token)
    {
        atributos.push_back(token);
        std::cin >> token;
        valores.push_back(token);
    }

    DynamicArray<int> ids = resolverFiltros(atributos, valores, 'P');

    if (ids.getSize() == 0)
    {
        std::cout << "LP VAZIO\n";
        return;
    }

    for (int i = 0; i < ids.getSize(); i++)
    {
        int posicao = i + 1;
        produtos[ids[i]].imprimirDados(posicao);
        produtos[ids[i]].imprimirUsuarios();
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// LC — Consultar compras
//
// Entrada:  LC <atrib_1> <valor_1> ... <atrib_k> <valor_k>
// Saída:    Sempre duas linhas por compra encontrada, em ordem crescente de id.
//           LC VAZIO se nenhuma compra satisfaz os filtros.
// ─────────────────────────────────────────────────────────────────────────────

void MercadoDCC::consultarCompras()
{
    DynamicArray<std::string> atributos;
    DynamicArray<std::string> valores;

    std::string token;
    while (std::cin.peek() != '\n' && std::cin.peek() != EOF && std::cin >> token)
    {
        atributos.push_back(token);
        std::cin >> token;
        valores.push_back(token);
    }

    DynamicArray<int> ids = resolverFiltros(atributos, valores, 'C');

    if (ids.getSize() == 0)
    {
        std::cout << "LC VAZIO\n";
        return;
    }

    for (int i = 0; i < ids.getSize(); i++)
    {
        int posicao = i + 1;
        compras[ids[i]].imprimirDados(posicao);
        compras[ids[i]].imprimirProdutos();
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// LR — Consultar reposições
//
// Entrada:  LR <atrib_1> <valor_1> ... <atrib_k> <valor_k>
// Saída:    Sempre duas linhas por reposição encontrada, em ordem crescente de id.
//           LR VAZIO se nenhuma reposição satisfaz os filtros.
// ─────────────────────────────────────────────────────────────────────────────

void MercadoDCC::consultarReposicoes()
{
    DynamicArray<std::string> atributos;
    DynamicArray<std::string> valores;

    std::string token;
    while (std::cin.peek() != '\n' && std::cin.peek() != EOF && std::cin >> token)
    {
        atributos.push_back(token);
        std::cin >> token;
        valores.push_back(token);
    }

    DynamicArray<int> ids = resolverFiltros(atributos, valores, 'R');

    if (ids.getSize() == 0)
    {
        std::cout << "LR VAZIO\n";
        return;
    }

    for (int i = 0; i < ids.getSize(); i++)
    {
        int posicao = i + 1;
        reposicoes[ids[i]].imprimirDados(posicao);
        reposicoes[ids[i]].imprimirProdutos();
    }
}