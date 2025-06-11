#include <iostream>
#include <string>
#include "sqlite/include/sqlite3.h"

void criarTabela(sqlite3* db) {
    const char* sql = "CREATE TABLE IF NOT EXISTS produtos ("
                     "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                     "nome TEXT NOT NULL,"
                     "quantidade INTEGER NOT NULL);";
    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Erro ao criar tabela: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

void adicionarProduto(sqlite3* db) {
    std::string nome;
    int quantidade;
    std::cout << "\n=== ADICIONAR PRODUTO ===\n";
    std::cout << "Nome do produto: ";
    std::getline(std::cin, nome);
    std::cout << "Quantidade: ";
    std::cin >> quantidade;
    std::cin.ignore();
    std::string sql = "INSERT INTO produtos (nome, quantidade) VALUES ('" + nome + "', " + std::to_string(quantidade) + ");";
    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Erro ao inserir produto: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "\nProduto adicionado com sucesso!\n";
    }
}

void listarProdutos(sqlite3* db) {
    std::cout << "\n=== LISTA DE PRODUTOS ===\n";
    const char* sql = "SELECT * FROM produtos;";
    char* errMsg = 0;
    auto callback = [](void*, int argc, char** argv, char** azColName) -> int {
        for (int i = 0; i < argc; i++) {
            std::cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << " ";
        }
        std::cout << std::endl;
        return 0;
    };
    int rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Erro ao listar produtos: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

void atualizarProduto(sqlite3* db) {
    std::cout << "\n=== ATUALIZAR PRODUTO ===\n";
    int id, quantidade;
    std::cout << "ID do produto a atualizar: ";
    std::cin >> id;
    std::cout << "Nova quantidade: ";
    std::cin >> quantidade;
    std::cin.ignore();
    std::string sql = "UPDATE produtos SET quantidade = " + std::to_string(quantidade) + " WHERE id = " + std::to_string(id) + ";";
    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Erro ao atualizar produto: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "\nProduto atualizado com sucesso!\n";
    }
}

void removerProduto(sqlite3* db) {
    std::cout << "\n=== REMOVER PRODUTO ===\n";
    int id;
    std::cout << "ID do produto a remover: ";
    std::cin >> id;
    std::cin.ignore();
    std::string sql = "DELETE FROM produtos WHERE id = " + std::to_string(id) + ";";
    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Erro ao remover produto: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "\nProduto removido com sucesso!\n";
    }
}

int main() {
    sqlite3* db;
    int rc = sqlite3_open("inventario.db", &db);
    if (rc) {
        std::cerr << "Erro ao abrir banco de dados: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }
    criarTabela(db);
    int opcao;
    do {
        std::cout << "\n=== MENU INVENTÁRIO COM SQLITE ===\n";
        std::cout << "1. Adicionar item\n";
        std::cout << "2. Listar itens\n";
        std::cout << "3. Atualizar item\n";
        std::cout << "4. Remover item\n";
        std::cout << "0. Sair\n";
        std::cout << "Escolha uma opção: ";
        std::cin >> opcao;
        std::cin.ignore();
        switch (opcao) {
            case 1:
                adicionarProduto(db);
                break;
            case 2:
                listarProdutos(db);
                break;
            case 3:
                atualizarProduto(db);
                break;
            case 4:
                removerProduto(db);
                break;
            case 0:
                std::cout << "\nSaindo do sistema...\n";
                break;
            default:
                std::cout << "\nOpção inválida!\n";
        }
    } while (opcao != 0);
    sqlite3_close(db);
    return 0;
}
