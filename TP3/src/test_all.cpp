#include <iostream>
#include <string>
#include "hashtable.hpp"
#include "invertedindex.hpp"
#include "usuario.hpp"
#include "produto.hpp"
#include "compra.hpp"
#include "reposicao.hpp"
#include "dynamicarray.hpp"
// ─────────────────────────────────────────────────────────────────────────────
// Utilitário de verificação
// ─────────────────────────────────────────────────────────────────────────────

static int totalTestes  = 0;
static int testesOk     = 0;

void check(bool condicao, const std::string& descricao)
{
    totalTestes++;
    if (condicao)
    {
        std::cout << "  [OK] " << descricao << "\n";
        testesOk++;
    }
    else
    {
        std::cout << "  [FALHA] " << descricao << "\n";
    }
}

void secao(const std::string& titulo)
{
    std::cout << "\n══════════════════════════════════════════\n";
    std::cout << "  " << titulo << "\n";
    std::cout << "══════════════════════════════════════════\n";
}

// ─────────────────────────────────────────────────────────────────────────────
// TESTES: DynamicArray
// ─────────────────────────────────────────────────────────────────────────────
void testeDynamicArray()
{
    secao("DynamicArray");

    // Construção e estado inicial
    DynamicArray<int> arr;
    check(arr.getSize()     == 0,  "tamanho inicial = 0");
    check(arr.getCapacity() == 4,  "capacidade inicial = 4");
    check(arr.empty()       == true, "empty() = true no início");

    // push_back e acesso
    arr.push_back(10);
    arr.push_back(20);
    arr.push_back(30);
    check(arr.getSize() == 3,  "tamanho após 3 push_back = 3");
    check(arr[0] == 10,        "arr[0] = 10");
    check(arr[1] == 20,        "arr[1] = 20");
    check(arr[2] == 30,        "arr[2] = 30");
    check(arr.empty() == false, "empty() = false após inserções");

    // Resize automático: inserir além da capacidade inicial (4)
    arr.push_back(40);
    arr.push_back(50); // dispara resize para capacidade 8
    check(arr.getSize()     == 5, "tamanho após 5 push_back = 5");
    check(arr.getCapacity() == 8, "capacidade dobrou para 8 após resize");
    check(arr[4] == 50,           "arr[4] = 50 após resize");

    // pop_back
    arr.pop_back();
    check(arr.getSize() == 4, "tamanho após pop_back = 4");
    check(arr[3] == 40,       "arr[3] = 40 após pop_back");

    // insert no meio
    arr.insert(1, 99);  // [10, 99, 20, 30, 40]
    check(arr.getSize() == 5, "tamanho após insert = 5");
    check(arr[0] == 10,       "arr[0] = 10 após insert");
    check(arr[1] == 99,       "arr[1] = 99 (inserido)");
    check(arr[2] == 20,       "arr[2] = 20 após shift");
    check(arr[3] == 30,       "arr[3] = 30 após shift");
    check(arr[4] == 40,       "arr[4] = 40 após shift");

    // removeAt no meio
    arr.removeAt(1);  // remove 99 → [10, 20, 30, 40]
    check(arr.getSize() == 4, "tamanho após removeAt = 4");
    check(arr[0] == 10,       "arr[0] = 10 após removeAt");
    check(arr[1] == 20,       "arr[1] = 20 após removeAt");

    // clear
    arr.clear();
    check(arr.getSize() == 0,   "tamanho após clear = 0");
    check(arr.empty()   == true, "empty() = true após clear");

    // Construtor de cópia
    DynamicArray<int> original;
    original.push_back(1);
    original.push_back(2);
    original.push_back(3);
    DynamicArray<int> copia(original);
    check(copia.getSize() == 3, "cópia tem tamanho 3");
    check(copia[0] == 1,        "cópia[0] = 1");
    check(copia[2] == 3,        "cópia[2] = 3");
    // Modifica original — cópia não deve mudar (deep copy)
    original[0] = 999;
    check(copia[0] == 1, "cópia independente do original (deep copy)");

    // Operador de atribuição
    DynamicArray<int> atrib;
    atrib = copia;
    check(atrib.getSize() == 3, "atribuição: tamanho = 3");
    check(atrib[1] == 2,        "atribuição: atrib[1] = 2");

    // Exceção em acesso fora dos limites
    bool excecaoLancada = false;
    try { arr[0]; }
    catch (const std::out_of_range&) { excecaoLancada = true; }
    check(excecaoLancada, "acesso fora dos limites lança out_of_range");

    // insert fora dos limites
    excecaoLancada = false;
    try { original.insert(100, 0); }
    catch (const std::out_of_range&) { excecaoLancada = true; }
    check(excecaoLancada, "insert fora dos limites lança out_of_range");

    // removeAt fora dos limites
    excecaoLancada = false;
    try { original.removeAt(100); }
    catch (const std::out_of_range&) { excecaoLancada = true; }
    check(excecaoLancada, "removeAt fora dos limites lança out_of_range");
}

// ─────────────────────────────────────────────────────────────────────────────
// TESTES: HashTable
// ─────────────────────────────────────────────────────────────────────────────
void testeHashTable()
{
    secao("HashTable<string, int>");

    HashTable<std::string, int> ht;
    check(ht.getSize() == 0,       "tamanho inicial = 0");
    check(ht.empty()   == true,    "empty() = true no início");
    check(ht.contains("chave") == false, "contains() = false para chave inexistente");
    check(ht.get("chave")      == nullptr, "get() = nullptr para chave inexistente");

    // insert e get
    ht.insert("ana", 21);
    ht.insert("bruno", 24);
    ht.insert("carla", 30);
    check(ht.getSize()       == 3,    "tamanho após 3 inserções = 3");
    check(ht.contains("ana") == true, "contains('ana') = true");
    check(*ht.get("ana")     == 21,   "get('ana') = 21");
    check(*ht.get("bruno")   == 24,   "get('bruno') = 24");
    check(*ht.get("carla")   == 30,   "get('carla') = 30");

    // Sobrescrita de valor existente
    ht.insert("ana", 99);
    check(ht.getSize()   == 3,  "tamanho não muda ao sobrescrever");
    check(*ht.get("ana") == 99, "get('ana') = 99 após sobrescrita");

    // operator[]
    ht["delta"] = 55;
    check(ht.getSize()     == 4,  "tamanho após operator[] com chave nova = 4");
    check(ht["delta"]      == 55, "operator['delta'] = 55");
    ht["delta"] = 77;
    check(ht["delta"]      == 77, "operator['delta'] = 77 após atualização");

    // remove
    bool removeu = ht.remove("bruno");
    check(removeu            == true,  "remove('bruno') retorna true");
    check(ht.getSize()       == 3,     "tamanho após remove = 3");
    check(ht.contains("bruno") == false, "contains('bruno') = false após remove");
    check(ht.get("bruno")    == nullptr, "get('bruno') = nullptr após remove");

    bool removeuInexistente = ht.remove("inexistente");
    check(removeuInexistente == false, "remove de chave inexistente retorna false");

    // Rehash: inserir muitos elementos e verificar que nada se perde
    // Capacidade inicial = 16, fator 0.75 → rehash em 13 elementos
    HashTable<std::string, int> htGrande;
    for (int i = 0; i < 30; i++)
    {
        std::string chave = "chave" + std::to_string(i);
        htGrande.insert(chave, i * 10);
    }
    check(htGrande.getSize() == 30, "30 inserções: tamanho = 30");
    bool todosCorretos = true;
    for (int i = 0; i < 30; i++)
    {
        std::string chave = "chave" + std::to_string(i);
        int* val = htGrande.get(chave);
        if (val == nullptr || *val != i * 10)
        {
            todosCorretos = false;
            break;
        }
    }
    check(todosCorretos, "todos os 30 valores recuperados corretamente após rehash");

    // HashTable<string, string> — verifica que outros tipos de V funcionam
    secao("HashTable<string, string>");
    HashTable<std::string, std::string> hts;
    hts.insert("cidade", "BeloHorizonte");
    hts["estado"] = "MG";
    check(*hts.get("cidade") == "BeloHorizonte", "get('cidade') = 'BeloHorizonte'");
    check(hts["estado"]      == "MG",            "operator['estado'] = 'MG'");

    // HashTable<int, int> — verifica DefaultHash<int>
    secao("HashTable<int, int>");
    HashTable<int, int> hti;
    hti.insert(0,  100);
    hti.insert(7,  200);
    hti.insert(15, 300);
    check(*hti.get(0)  == 100, "get(0) = 100");
    check(*hti.get(7)  == 200, "get(7) = 200");
    check(*hti.get(15) == 300, "get(15) = 300");
    check(hti.contains(99) == false, "contains(99) = false");
}

// ─────────────────────────────────────────────────────────────────────────────
// TESTES: InvertedIndex + intersectIds
// ─────────────────────────────────────────────────────────────────────────────
void testeInvertedIndex()
{
    secao("InvertedIndex");

    InvertedIndex idx;
    check(idx.contains("BeloHorizonte") == false, "índice vazio não contém 'BeloHorizonte'");
    check(idx.getIds("BeloHorizonte")   == nullptr, "getIds retorna nullptr para chave inexistente");

    // add — inserção básica
    idx.add("BeloHorizonte", 0);
    idx.add("BeloHorizonte", 2);
    idx.add("BeloHorizonte", 5);
    idx.add("SaoPaulo", 1);
    idx.add("SaoPaulo", 3);

    check(idx.contains("BeloHorizonte") == true, "contains('BeloHorizonte') = true após add");

    DynamicArray<int>* bh = idx.getIds("BeloHorizonte");
    check(bh != nullptr,       "getIds('BeloHorizonte') != nullptr");
    check(bh->getSize() == 3,  "lista 'BeloHorizonte' tem 3 elementos");
    check((*bh)[0] == 0,       "lista[0] = 0 (ordem crescente)");
    check((*bh)[1] == 2,       "lista[1] = 2");
    check((*bh)[2] == 5,       "lista[2] = 5");

    DynamicArray<int>* sp = idx.getIds("SaoPaulo");
    check(sp->getSize() == 2,  "lista 'SaoPaulo' tem 2 elementos");
    check((*sp)[0] == 1,       "lista[0] = 1");
    check((*sp)[1] == 3,       "lista[1] = 3");

    // add não duplica IDs já existentes
    idx.add("BeloHorizonte", 2); // já existe
    check(bh->getSize() == 3, "add de ID duplicado não altera a lista");

    // add fora de ordem (inserção no meio)
    InvertedIndex idx2;
    idx2.add("MG", 5);
    idx2.add("MG", 1); // deve ir antes do 5
    idx2.add("MG", 3); // deve ir entre 1 e 5
    DynamicArray<int>* mg = idx2.getIds("MG");
    check(mg->getSize() == 3, "inserção fora de ordem: tamanho = 3");
    check((*mg)[0] == 1,      "inserção fora de ordem: [0] = 1");
    check((*mg)[1] == 3,      "inserção fora de ordem: [1] = 3");
    check((*mg)[2] == 5,      "inserção fora de ordem: [2] = 5");

    // intersectIds
    secao("intersectIds");

    DynamicArray<int> a, b;
    a.push_back(0); a.push_back(2); a.push_back(5); // [0,2,5]
    b.push_back(0); b.push_back(3); b.push_back(5); // [0,3,5]

    DynamicArray<int> inter = intersectIds(a, b);
    check(inter.getSize() == 2, "interseção [0,2,5] ∩ [0,3,5] tem 2 elementos");
    check(inter[0] == 0,        "interseção[0] = 0");
    check(inter[1] == 5,        "interseção[1] = 5");

    // Interseção vazia
    DynamicArray<int> c, d;
    c.push_back(1); c.push_back(3);
    d.push_back(2); d.push_back(4);
    DynamicArray<int> vazia = intersectIds(c, d);
    check(vazia.getSize() == 0, "interseção disjunta = vazia");

    // Interseção com lista vazia
    DynamicArray<int> e;
    DynamicArray<int> comVazia = intersectIds(a, e);
    check(comVazia.getSize() == 0, "interseção com lista vazia = vazia");

    // Interseção total (listas iguais)
    DynamicArray<int> f;
    f.push_back(0); f.push_back(2); f.push_back(5);
    DynamicArray<int> total = intersectIds(a, f);
    check(total.getSize() == 3, "interseção de listas iguais tem todos os elementos");
}

// ─────────────────────────────────────────────────────────────────────────────
// TESTES: Usuario
// ─────────────────────────────────────────────────────────────────────────────
void testeUsuario()
{
    secao("Usuario");

    Usuario u(0, "Ana", 21, "BeloHorizonte", "MG", "Brasileira");

    check(u.getId()             == 0,               "getId = 0");
    check(u.getNome()           == "Ana",            "getNome = Ana");
    check(u.getIdade()          == 21,               "getIdade = 21");
    check(u.getCidade()         == "BeloHorizonte",  "getCidade = BeloHorizonte");
    check(u.getEstado()         == "MG",             "getEstado = MG");
    check(u.getNacionalidade()  == "Brasileira",     "getNacionalidade = Brasileira");

    // imprimirDados — saída esperada:
    // LU resultado_1 usuario 0 Ana 21 BeloHorizonte MG Brasileira
    std::cout << "\n  [SAÍDA ESPERADA] LU resultado_1 usuario 0 Ana 21 BeloHorizonte MG Brasileira\n";
    std::cout << "  [SAÍDA REAL]     ";
    u.imprimirDados(1);

    // imprimirProdutos sem compras — não deve imprimir nada
    std::cout << "\n  [SAÍDA ESPERADA] (nenhuma linha — usuário sem compras)\n";
    std::cout << "  [SAÍDA REAL]     ";
    u.imprimirProdutos();
    std::cout << "(fim)\n";

    // adicionarCompra — primeira compra
    u.adicionarCompra(1, 2); // produto 1, qtd 2
    u.adicionarCompra(3, 1); // produto 3, qtd 1
    u.adicionarCompra(0, 5); // produto 0, qtd 5 — deve ir antes de 1 e 3

    // imprimirProdutos — saída esperada (ordem crescente de id_produto):
    // produto_1 0 5 produto_2 1 2 produto_3 3 1
    std::cout << "\n  [SAÍDA ESPERADA] produto_1 0 5 produto_2 1 2 produto_3 3 1\n";
    std::cout << "  [SAÍDA REAL]     ";
    u.imprimirProdutos();

    // adicionarCompra — acumulação em produto já existente
    u.adicionarCompra(1, 3); // produto 1 mais 3 unidades → total 5
    std::cout << "\n  [SAÍDA ESPERADA] produto_1 0 5 produto_2 1 5 produto_3 3 1\n";
    std::cout << "  [SAÍDA REAL]     ";
    u.imprimirProdutos();

    // Construtor padrão
    Usuario uPadrao;
    check(uPadrao.getId() == -1, "construtor padrão: id = -1");
}

// ─────────────────────────────────────────────────────────────────────────────
// TESTES: Produto
// ─────────────────────────────────────────────────────────────────────────────
void testeProduto()
{
    secao("Produto");

    Produto p(0, "Mouse", 80.00, 5, "Informatica", "Logitech", "Novo");

    check(p.getId()        == 0,            "getId = 0");
    check(p.getNome()      == "Mouse",      "getNome = Mouse");
    check(p.getPreco()     == 80.00,        "getPreco = 80.00");
    check(p.getQtd()       == 5,            "getQtd = 5");
    check(p.getCategoria() == "Informatica","getCategoria = Informatica");
    check(p.getMarca()     == "Logitech",   "getMarca = Logitech");
    check(p.getCondicao()  == "Novo",       "getCondicao = Novo");

    // Estoque
    p.diminuirEstoque(2);
    check(p.getQtd() == 3, "diminuirEstoque(2): qtd = 3");
    p.aumentarEstoque(4);
    check(p.getQtd() == 7, "aumentarEstoque(4): qtd = 7");

    // imprimirDados — saída esperada:
    // LP resultado_1 produto 0 Mouse 80.00 7 Informatica Logitech Novo
    std::cout << "\n  [SAÍDA ESPERADA] LP resultado_1 produto 0 Mouse 80.00 7 Informatica Logitech Novo\n";
    std::cout << "  [SAÍDA REAL]     ";
    p.imprimirDados(1);

    // imprimirUsuarios sem compradores — não deve imprimir nada
    std::cout << "\n  [SAÍDA ESPERADA] (nenhuma linha — produto sem compradores)\n";
    std::cout << "  [SAÍDA REAL]     ";
    p.imprimirUsuarios();
    std::cout << "(fim)\n";

    // adicionarComprador
    p.adicionarComprador(2, 3); // usuário 2, qtd 3
    p.adicionarComprador(0, 1); // usuário 0, qtd 1 — deve ir antes do 2
    p.adicionarComprador(2, 2); // usuário 2 mais 2 → total 5

    // imprimirUsuarios — saída esperada:
    // usuario_1 0 1 usuario_2 2 5
    std::cout << "\n  [SAÍDA ESPERADA] usuario_1 0 1 usuario_2 2 5\n";
    std::cout << "  [SAÍDA REAL]     ";
    p.imprimirUsuarios();

    // Preço com duas casas decimais
    Produto p2(1, "Notebook", 3500.00, 2, "Informatica", "Dell", "Novo");
    std::cout << "\n  [SAÍDA ESPERADA] LP resultado_1 produto 1 Notebook 3500.00 2 Informatica Dell Novo\n";
    std::cout << "  [SAÍDA REAL]     ";
    p2.imprimirDados(1);

    // Construtor padrão
    Produto pPadrao;
    check(pPadrao.getId() == -1, "construtor padrão: id = -1");
}

// ─────────────────────────────────────────────────────────────────────────────
// TESTES: Compra
// ─────────────────────────────────────────────────────────────────────────────
void testeCompra()
{
    secao("Compra");

    Compra c(0, 100, 0); // id=0, timestamp=100, id_usuario=0

    check(c.getId()        == 0,   "getId = 0");
    check(c.getTimestamp() == 100, "getTimestamp = 100");
    check(c.getIdUsuario() == 0,   "getIdUsuario = 0");
    check(c.getNumProdutos() == 0, "getNumProdutos = 0 antes de adicionar");

    // adicionarProduto — inserção fora de ordem para testar ordenação
    c.adicionarProduto(1, 1); // produto 1, qtd 1
    c.adicionarProduto(0, 2); // produto 0, qtd 2 — deve ir antes do 1
    c.adicionarProduto(3, 4); // produto 3, qtd 4

    check(c.getNumProdutos()  == 3, "getNumProdutos = 3 após 3 inserções");
    check(c.getIdProduto(0)   == 0, "produto[0] = 0 (ordem crescente)");
    check(c.getIdProduto(1)   == 1, "produto[1] = 1");
    check(c.getIdProduto(2)   == 3, "produto[2] = 3");
    check(c.getQtdProduto(0)  == 2, "qtd[0] = 2");
    check(c.getQtdProduto(1)  == 1, "qtd[1] = 1");
    check(c.getQtdProduto(2)  == 4, "qtd[2] = 4");

    // imprimirDados — saída esperada:
    // LC resultado_1 compra 0 timestamp 100 usuario 0
    std::cout << "\n  [SAÍDA ESPERADA] LC resultado_1 compra 0 timestamp 100 usuario 0\n";
    std::cout << "  [SAÍDA REAL]     ";
    c.imprimirDados(1);

    // imprimirProdutos — saída esperada:
    // produto_1 0 2 produto_2 1 1 produto_3 3 4
    std::cout << "\n  [SAÍDA ESPERADA] produto_1 0 2 produto_2 1 1 produto_3 3 4\n";
    std::cout << "  [SAÍDA REAL]     ";
    c.imprimirProdutos();

    // Construtor padrão
    Compra cPadrao;
    check(cPadrao.getId() == -1, "construtor padrão: id = -1");
}

// ─────────────────────────────────────────────────────────────────────────────
// TESTES: Reposicao
// ─────────────────────────────────────────────────────────────────────────────
void testeReposicao()
{
    secao("Reposicao");

    Reposicao r(0, 125); // id=0, timestamp=125

    check(r.getId()          == 0,   "getId = 0");
    check(r.getTimestamp()   == 125, "getTimestamp = 125");
    check(r.getNumProdutos() == 0,   "getNumProdutos = 0 antes de adicionar");

    // adicionarProduto — mesmo padrão da Compra
    r.adicionarProduto(2, 5); // produto 2, qtd 5
    r.adicionarProduto(0, 3); // produto 0, qtd 3 — deve ir antes do 2

    check(r.getNumProdutos()  == 2, "getNumProdutos = 2 após 2 inserções");
    check(r.getIdProduto(0)   == 0, "produto[0] = 0 (ordem crescente)");
    check(r.getIdProduto(1)   == 2, "produto[1] = 2");
    check(r.getQtdProduto(0)  == 3, "qtd[0] = 3");
    check(r.getQtdProduto(1)  == 5, "qtd[1] = 5");

    // imprimirDados — saída esperada:
    // LR resultado_1 reposicao 0 timestamp 125
    std::cout << "\n  [SAÍDA ESPERADA] LR resultado_1 reposicao 0 timestamp 125\n";
    std::cout << "  [SAÍDA REAL]     ";
    r.imprimirDados(1);

    // imprimirProdutos — saída esperada:
    // produto_1 0 3 produto_2 2 5
    std::cout << "\n  [SAÍDA ESPERADA] produto_1 0 3 produto_2 2 5\n";
    std::cout << "  [SAÍDA REAL]     ";
    r.imprimirProdutos();

    // Construtor padrão
    Reposicao rPadrao;
    check(rPadrao.getId() == -1, "construtor padrão: id = -1");
}

// ─────────────────────────────────────────────────────────────────────────────
// main
// ─────────────────────────────────────────────────────────────────────────────
int main()
{
    testeDynamicArray();
    testeHashTable();
    testeInvertedIndex();
    testeUsuario();
    testeProduto();
    testeCompra();
    testeReposicao();

    std::cout << "\n══════════════════════════════════════════\n";
    std::cout << "  RESULTADO FINAL: "
              << testesOk << "/" << totalTestes << " testes passaram\n";
    std::cout << "══════════════════════════════════════════\n";

    return (testesOk == totalTestes) ? 0 : 1;
}