#ifndef HASHTABLE_HPP
#define HASHTABLE_HPP

#include <string>
#include <stdexcept>
#include "DynamicArray.hpp"

// ─────────────────────────────────────────────────────────────────────────────
// Functores de hash — definidos fora da HashTable para que qualquer tipo
// possa ter sua própria implementação sem modificar a classe principal.
// ─────────────────────────────────────────────────────────────────────────────

// Hash padrão para tipos inteiros: módulo simples.
template <typename K>
struct DefaultHash
{
    int operator()(const K& key, int cap) const
    {
        return (int)((size_t)key % (size_t)cap);
    }
};

// Hash djb2 para std::string — boa distribuição, amplamente usada.
template <>
struct DefaultHash<std::string>
{
    int operator()(const std::string& key, int cap) const
    {
        unsigned long hash = 5381;

        for (int i = 0; i < (int)key.size(); i++)
        {
            // hash = hash * 33 + c
            hash = ((hash << 5) + hash) + (unsigned char)key[i];
        }

        return (int)(hash % (unsigned long)cap);
    }
};

// ─────────────────────────────────────────────────────────────────────────────
// HashEntry — par chave-valor interno de cada bucket.
// ─────────────────────────────────────────────────────────────────────────────
template <typename K, typename V>
struct HashEntry
{
    K key;
    V value;

    HashEntry() {}
    HashEntry(const K& k, const V& v) : key(k), value(v) {}
};

// ─────────────────────────────────────────────────────────────────────────────
// HashTable<K, V, Hash>
//
// Tabela hash genérica com encadeamento separado (chaining).
// Cada bucket é um DynamicArray<HashEntry<K,V>>.
//
// O terceiro parâmetro template 'Hash' é um functor com assinatura:
//   int operator()(const K& key, int capacity) const
// Por padrão usa DefaultHash<K>, que cobre int e std::string.
// Para tipos customizados, basta definir um functor e passá-lo:
//   HashTable<MinhaChave, int, MeuHash> tabela;
//
// Fator de carga máximo: 0.75
// Complexidade:
//   insert / get / remove — O(1) médio, O(n) pior caso
//   rehash               — O(n), amortizado O(1) por inserção
// ─────────────────────────────────────────────────────────────────────────────
template <typename K, typename V, typename Hash = DefaultHash<K>>
class HashTable
{
private:

    DynamicArray<HashEntry<K,V>>* buckets;
    int capacity;
    int size;
    Hash hasher; // instância do functor — sem estado, custo zero

    static const int INITIAL_CAPACITY = 16;
    static const int LOAD_FACTOR_NUM  = 3; // 3/4 = 0.75
    static const int LOAD_FACTOR_DEN  = 4;

    int hashKey(const K& key, int cap) const
    {
        return hasher(key, cap);
    }

    void rehash()
    {
        int newCapacity = capacity * 2;
        DynamicArray<HashEntry<K,V>>* newBuckets =
            new DynamicArray<HashEntry<K,V>>[newCapacity];

        for (int i = 0; i < capacity; i++)
        {
            int bucketSize = buckets[i].getSize();

            for (int j = 0; j < bucketSize; j++)
            {
                HashEntry<K,V>& entry = buckets[i][j];
                int idx = hashKey(entry.key, newCapacity);
                newBuckets[idx].push_back(entry);
            }
        }

        delete[] buckets;
        buckets  = newBuckets;
        capacity = newCapacity;
    }

public:

    // ── Construtores / destrutor ──────────────────────────────────────────────

    HashTable()
        : capacity(INITIAL_CAPACITY), size(0)
    {
        buckets = new DynamicArray<HashEntry<K,V>>[capacity];
    }

    explicit HashTable(int initialCapacity)
        : capacity(initialCapacity), size(0)
    {
        buckets = new DynamicArray<HashEntry<K,V>>[capacity];
    }

    HashTable(const HashTable& other)
        : capacity(other.capacity), size(other.size), hasher(other.hasher)
    {
        buckets = new DynamicArray<HashEntry<K,V>>[capacity];

        for (int i = 0; i < capacity; i++)
        {
            buckets[i] = other.buckets[i];
        }
    }

    HashTable& operator=(const HashTable& other)
    {
        if (this == &other) return *this;

        delete[] buckets;

        capacity = other.capacity;
        size     = other.size;
        hasher   = other.hasher;
        buckets  = new DynamicArray<HashEntry<K,V>>[capacity];

        for (int i = 0; i < capacity; i++)
        {
            buckets[i] = other.buckets[i];
        }

        return *this;
    }

    ~HashTable()
    {
        delete[] buckets;
    }

    // ── Interface pública ─────────────────────────────────────────────────────

    int  getSize() const { return size; }
    bool empty()   const { return size == 0; }

    // Verifica se a chave existe. Custo: O(1) médio.
    bool contains(const K& key) const
    {
        int idx = hashKey(key, capacity);
        int n   = buckets[idx].getSize();

        for (int i = 0; i < n; i++)
        {
            if (buckets[idx][i].key == key)
                return true;
        }

        return false;
    }

    // Retorna ponteiro para o valor, ou nullptr se não existe. Custo: O(1) médio.
    // ATENÇÃO: ponteiro invalidado após rehash ou remoção.
    V* get(const K& key)
    {
        int idx = hashKey(key, capacity);
        int n   = buckets[idx].getSize();

        for (int i = 0; i < n; i++)
        {
            if (buckets[idx][i].key == key)
                return &buckets[idx][i].value;
        }

        return nullptr;
    }

    const V* get(const K& key) const
    {
        int idx = hashKey(key, capacity);
        int n   = buckets[idx].getSize();

        for (int i = 0; i < n; i++)
        {
            if (buckets[idx][i].key == key)
                return &buckets[idx][i].value;
        }

        return nullptr;
    }

    // Insere ou sobrescreve. Custo: O(1) amortizado.
    void insert(const K& key, const V& value)
    {
        if (size * LOAD_FACTOR_DEN > capacity * LOAD_FACTOR_NUM)
            rehash();

        int idx = hashKey(key, capacity);
        int n   = buckets[idx].getSize();

        for (int i = 0; i < n; i++)
        {
            if (buckets[idx][i].key == key)
            {
                buckets[idx][i].value = value;
                return;
            }
        }

        buckets[idx].push_back(HashEntry<K,V>(key, value));
        size++;
    }

    // Acessa ou cria com valor padrão — similar ao std::map::operator[].
    // Custo: O(1) amortizado.
    V& operator[](const K& key)
    {
        if (size * LOAD_FACTOR_DEN > capacity * LOAD_FACTOR_NUM)
            rehash();

        int idx = hashKey(key, capacity);
        int n   = buckets[idx].getSize();

        for (int i = 0; i < n; i++)
        {
            if (buckets[idx][i].key == key)
                return buckets[idx][i].value;
        }

        buckets[idx].push_back(HashEntry<K,V>(key, V()));
        size++;

        return buckets[idx][buckets[idx].getSize() - 1].value;
    }

    // Remove a chave. Retorna true se removeu, false se não existia.
    // Custo: O(1) médio.
    bool remove(const K& key)
    {
        int idx = hashKey(key, capacity);
        int n   = buckets[idx].getSize();

        for (int i = 0; i < n; i++)
        {
            if (buckets[idx][i].key == key)
            {
                buckets[idx].removeAt(i);
                size--;
                return true;
            }
        }

        return false;
    }

    // Itera sobre todos os pares. Assinatura de func: void(const K&, V&).
    template <typename Func>
    void forEach(Func func)
    {
        for (int i = 0; i < capacity; i++)
        {
            int n = buckets[i].getSize();

            for (int j = 0; j < n; j++)
            {
                func(buckets[i][j].key, buckets[i][j].value);
            }
        }
    }
};

#endif