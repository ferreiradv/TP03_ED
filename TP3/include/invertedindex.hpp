#ifndef INVERTED_INDEX_HPP
#define INVERTED_INDEX_HPP

#include <string>
#include "HashTable.hpp"
#include "DynamicArray.hpp"

// ─────────────────────────────────────────────────────────────────────────────
// InvertedIndex
//
// Encapsula uma HashTable<string, DynamicArray<int>>.
// Cada chave é o valor de um atributo (ex: "BeloHorizonte", "21", "Logitech").
// Cada valor é a lista ordenada de IDs das entidades que possuem aquele atributo.
//
// Exemplo conceitual para o atributo "cidade":
//   "BeloHorizonte" → [0, 2, 5]
//   "SaoPaulo"      → [1, 3]
//
// A lista de IDs é mantida em ordem crescente para facilitar a interseção
// nas consultas multiatributo (algoritmo de merge em O(n+m)).
//
// Um InvertedIndex por atributo buscável é instanciado no MercadoDCC.
// ─────────────────────────────────────────────────────────────────────────────
class InvertedIndex
{
private:

    // Mapeamento: valor_do_atributo → lista_de_ids (ordenada)
    HashTable<std::string, DynamicArray<int>> table;

public:

    // ── Modificação ───────────────────────────────────────────────────────────

    // Registra que a entidade de id 'id' possui o valor 'value' neste atributo.
    // Se 'value' já existe no índice, insere 'id' na lista mantendo ordem crescente.
    // Se 'value' não existe, cria a entrada e insere 'id'.
    //
    // IMPORTANTE: como os IDs são atribuídos em ordem crescente pelo sistema,
    // na prática 'id' será sempre maior que todos os IDs já na lista —
    // ou seja, push_back é suficiente para manter a ordem no caso de cadastro.
    // O insert ordenado genérico está aqui para robustez.
    //
    // Custo: O(k) onde k = tamanho atual da lista para 'value'. Na prática O(1)
    // para cadastros sequenciais (push_back direto).
    void add(const std::string& value, int id)
    {
        // operator[] cria a entrada com DynamicArray vazio se não existir.
        DynamicArray<int>& lista = table[value];

        int n = lista.getSize();

        // Como IDs são crescentes no cadastro, o novo ID é sempre o maior.
        // Verificamos igualmente a ordem para garantir robustez geral.
        for (int i = 0; i < n; i++)
        {
            if (lista[i] == id)
            {
                return; // ID já presente — não duplica
            }

            if (lista[i] > id)
            {
                lista.insert(i, id);
                return;
            }
        }

        lista.push_back(id); // caso mais comum: insere no fim
    }

    // ── Consulta ──────────────────────────────────────────────────────────────

    // Retorna ponteiro para a lista de IDs associada a 'value'.
    // Retorna nullptr se 'value' não existe no índice.
    // Custo: O(1) médio.
    DynamicArray<int>* getIds(const std::string& value)
    {
        return table.get(value);
    }

    const DynamicArray<int>* getIds(const std::string& value) const
    {
        return table.get(value);
    }

    // Verifica se 'value' existe como chave no índice.
    bool contains(const std::string& value) const
    {
        return table.contains(value);
    }
};

// ─────────────────────────────────────────────────────────────────────────────
// Função auxiliar global: interseção de duas listas ordenadas de IDs.
//
// Recebe duas listas ordenadas e produz uma terceira contendo apenas
// os IDs presentes em ambas (merge intersection em O(n + m)).
//
// Usada pelas consultas multiatributo para combinar os resultados de
// múltiplos filtros com AND implícito.
// ─────────────────────────────────────────────────────────────────────────────
inline DynamicArray<int> intersectIds(const DynamicArray<int>& a,
                                      const DynamicArray<int>& b)
{
    DynamicArray<int> result;

    int i = 0, j = 0;
    int na = a.getSize(), nb = b.getSize();

    // Algoritmo de dois ponteiros — funciona porque ambas estão ordenadas.
    while (i < na && j < nb)
    {
        if (a[i] == b[j])
        {
            result.push_back(a[i]);
            i++;
            j++;
        }
        else if (a[i] < b[j])
        {
            i++;
        }
        else
        {
            j++;
        }
    }

    return result;
}

#endif