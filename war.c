// JOGO DE ESTRATÉGIA WAR ESTRUTURADO

#include <stdio.h>
#include <string.h>

// Definição da estrutura Territorio para armazenar dados de cada território do jogo
struct Territorio {
    char nome[30];    // Nome do território (máximo 29 caracteres + '\0')
    char cor[10];     // Cor do exército que ocupa o território
    int tropas;       // Quantidade de tropas no território
};

int main() {
    // Declaração do vetor de territórios
    struct Territorio territorios[5];
    
    printf("\n=== CADASTRO DE TERRITÓRIOS DO WAR ===\n");
    
    // Laço para cadastro dos territórios
    for(int i = 0; i < 5; i++) {
        printf("\nCadastro do território %d\n", i + 1);
        
        // NÃO usar fflush(stdin) — comportamento indefinido em C.
        // fgets para ler strings de forma segura.
        printf("Digite o nome: ");
        fgets(territorios[i].nome, sizeof(territorios[i].nome), stdin);
        // Remove o '\n' do final da string, se presente
        territorios[i].nome[strcspn(territorios[i].nome, "\n")] = '\0';
        
        printf("Digite a cor do exército: ");
        fgets(territorios[i].cor, sizeof(territorios[i].cor), stdin);
        territorios[i].cor[strcspn(territorios[i].cor, "\n")] = '\0';
        
        printf("Digite a quantidade de tropas: ");
        // Ler inteiro com scanf e validar retorno
        if (scanf("%d", &territorios[i].tropas) != 1) {
            // entrada inválida -> definir 0 e limpar buffer
            territorios[i].tropas = 0;
        }
        // Limpar o restante da linha (inclui o '\n' deixado pelo scanf)
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) { }
    }
    
    // Exibição dos dados cadastrados
    printf("\n=== TERRITÓRIOS CADASTRADOS ===\n");
    for(int i = 0; i < 5; i++) {
        printf("\nTerritório %d\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exército: %s\n", territorios[i].cor);
        printf("Quantidade de tropas: %d\n", territorios[i].tropas);
    }
    
    return 0;
}