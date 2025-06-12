#include <iostream>
#include <string>
#include <fstream>
#include "sqlite/include/sqlite3.h"
#include <iomanip>
#include <cstdlib>  // Para system()
#include <filesystem>
#include <limits> // Necessário para std::numeric_limits

// ===================
// PROTÓTIPOS
// ===================
void criarTabela(sqlite3* db);
void listarProdutos(sqlite3* db);  // ✅ Adicionado: protótipo da função para evitar erro de escopo
void adicionarProduto(sqlite3* db);
void atualizarProduto(sqlite3* db);
void removerProduto(sqlite3* db);

// Função para escrever no arquivo de saída
void writeOutput(const std::string& message) {
    std::string outputPath = "C:/Users/USER/Desktop/trabalho.C++/output.txt";
    std::ofstream outFile(outputPath, std::ios::app);
    if (outFile.is_open()) {
        outFile << message << std::endl;
        outFile.close();
    }
}

// ===================
// CRIA TABELA
// ===================
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

// ===================
// ADICIONAR PRODUTO
// ===================
void adicionarProduto(sqlite3* db) {
    std::string nome;
    int quantidade;
    std::cout << "\n=== ADICIONAR PRODUTO ===\n";
    std::cout << "Nome do produto: ";
    std::getline(std::cin, nome);
    std::cout << "Quantidade: ";
    std::cin >> quantidade;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa o buffer de entrada
    
    std::string sql = "INSERT INTO produtos (nome, quantidade) VALUES ('" + nome + "', " + std::to_string(quantidade) + ");";
    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Erro ao inserir produto: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "\nProduto adicionado com sucesso!\n";
        std::cout << "\nTabela atualizada:\n";
        listarProdutos(db);  // ✅ Agora essa função foi declarada antes, então funciona
    }
}

// ===================
// LISTAR PRODUTOS
// ===================
void listarProdutos(sqlite3* db) {
    std::cout << "\n=== LISTA DE PRODUTOS ===\n";
    std::cout << std::setw(5) << "ID" << std::setw(20) << "Nome" << std::setw(15) << "Quantidade" << std::endl;
    std::cout << std::string(40, '-') << std::endl;
    
    const char* sql = "SELECT * FROM produtos ORDER BY id DESC;";
    char* errMsg = 0;
    auto callback = [](void*, int argc, char** argv, char** azColName) -> int {
        std::cout << std::setw(5) << argv[0] << std::setw(20) << argv[1] << std::setw(15) << argv[2] << std::endl;
        return 0;
    };
    int rc = sqlite3_exec(db, sql, callback, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Erro ao listar produtos: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    }
}

// ===================
// ATUALIZAR PRODUTO
// ===================
void atualizarProduto(sqlite3* db) {
    std::cout << "\n=== ATUALIZAR PRODUTO ===\n";
    int id, quantidade;
    std::cout << "ID do produto a atualizar: ";
    std::cin >> id;
    std::cout << "Nova quantidade: ";
    std::cin >> quantidade;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa o buffer de entrada
    
    std::string sql = "UPDATE produtos SET quantidade = " + std::to_string(quantidade) + " WHERE id = " + std::to_string(id) + ";";
    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        std::cerr << "Erro ao atualizar produto: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        std::cout << "\nProduto atualizado com sucesso!\n";
        std::cout << "\nTabela atualizada:\n";
        listarProdutos(db);
    }
}

// ===================
// REMOVER PRODUTO
// ===================
void removerProduto(sqlite3* db) {
    std::cout << "\n=== REMOVER PRODUTO ===\n";
    int id;
    std::cout << "ID do produto a remover: ";
    std::cin >> id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Limpa o buffer de entrada
    
    std::string sql = "DELETE FROM produtos WHERE id = " + std::to_string(id) + ";";
    
    char* errMsg = 0;
    int rc = sqlite3_exec(db, sql.c_str(), 0, 0, &errMsg);
    
    if (rc != SQLITE_OK) {
        std::cerr << "Erro ao remover produto: " << errMsg << std::endl;
        sqlite3_free(errMsg);
    } else {
        if (sqlite3_changes(db) == 0) {
            std::cout << "\nNenhum produto encontrado com o ID " << id << ".\n";
        } else {
            std::cout << "\nProduto removido com sucesso!\n";
        }
        std::cout << "\nTabela atualizada:\n";
        listarProdutos(db);
    }
}

// ===================
// FUNÇÃO PRINCIPAL
// ===================
int main() {
    std::cout << "Iniciando sistema de inventário...\n";
    
    sqlite3* db;
    const char* dbPath = "inventario.db"; // Usando uma variável para o caminho
    std::filesystem::path fullDbPath = std::filesystem::current_path() / dbPath; // Construção correta do caminho

    int rc = sqlite3_open(dbPath, &db);
    if (rc) {
        std::cerr << "Erro ao abrir banco de dados: " << sqlite3_errmsg(db) << std::endl;
        return 1;
    }

    std::cout << "Banco de dados conectado com sucesso!\n";
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
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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
