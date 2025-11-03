// JOGO DE ESTRATÉGIA WAR ESTRUTURADO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Definição da estrutura Territorio para armazenar dados de cada território
struct Territorio {
    char nome[30];    // Nome do território (máximo 29 caracteres + '\0')
    char cor[10];     // Cor do exército que ocupa o território
    int tropas;       // Quantidade de tropas no território
};

// Protótipos das funções (se necessário)
void cadastrarTerritorios(struct Territorio* mapa, int n);
void exibirTerritorios(struct Territorio* mapa, int n);
void atacar(struct Territorio* atacante, struct Territorio* defensor);
void liberarMemoria(struct Territorio* mapa);

int main() {
    // Semente para números aleatórios
    srand((unsigned int)time(NULL));

    int n = 0;
    printf("\n=== CADASTRO (DINÂMICO) DE TERRITÓRIOS DO WAR ===\n");
    // Pedir n° de territórios ao usuário (validação mínima)
    do {
        printf("Digite o número de territórios: ");
        if (scanf("%d", &n) != 1) {
            // Limpar entrada inválida
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            n = 0; // Forçar repetição do loop
        }
        // Limpar '\n' restante deixado pelo scanf
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) { }
    } while (n <= 0);

    // Alocação dinâmica do vetor de territórios usando calloc
    struct Territorio* mapa = (struct Territorio*) calloc((size_t)n, sizeof(struct Territorio));
    if (mapa == NULL) {
        fprintf(stderr, "Erro: Falha na alocação de memória.\n");
        return 1; // Encerrar o programa com erro
    }

    // Cadastro dos territórios usando ponteiros
    cadastrarTerritorios(mapa, n);

    // Exibe o mapa inicial depois do cadastro
        printf("\n=====================");
        printf("\n=== MAPA DO MUNDO ===\n");
        printf("=====================\n");
        exibirTerritorios(mapa, n);

    // Loop de interação para realizar ataques até o usuário decidir sair
    while (1) {

        printf("\n*** FASE DE ATAQUE *** (1 para avançar, 0 para sair): ");
        int escolha;
        if (scanf("%d", &escolha) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            escolha = 0;
        }
        // Limpar '\n' restante deixado pelo scanf
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) { }

        if (escolha == 0) break;

        int atacanteIdx = -1, defensorIdx = -1;
        printf("Escolha o território ATACANTE (1 a %d): ", n);
        if (scanf("%d", &atacanteIdx) != 1) {atacanteIdx = -1;}
        printf("Escolha o território DEFENSOR (1 a %d): ", n);
        if (scanf("%d", &defensorIdx) != 1) {defensorIdx = -1;}
        while ((ch = getchar()) != '\n' && ch != EOF) { }

        // Validar índices
        if (atacanteIdx < 1 || atacanteIdx > n || defensorIdx < 1 || defensorIdx > n) {
            printf("Índices inválidos. Tente novamente.\n");
            continue;
        }
        if (atacanteIdx == defensorIdx) {
            printf("Um território não pode atacar a si mesmo. Tente novamente.\n");
            continue;
        }

        struct Territorio* atacante = mapa + (atacanteIdx - 1);
        struct Territorio* defensor = mapa + (defensorIdx - 1);

        // Validar que não são da mesma cor
        if (strcmp(atacante->cor, defensor->cor) == 0) {
            printf("Territórios pertencem ao mesmo exército. Ataque inválido.\n");
            continue;
        }

        // Validar que atacante tem tropas suficientes (pelo menos 1)
        if (atacante->tropas < 1) {
            printf("Território atacante não possui tropas suficientes para atacar.\n");
            continue;
        }

        // Realizar o ataque (com ponteiros)
        atacar(atacante, defensor);

        // Mostrar apenas o estado atual após ataque
        printf("\n==================================");
        printf("\n=== MAPA DO MUNDO - PÓS ATAQUE ===\n");
        printf("==================================\n");
        exibirTerritorios(mapa, n);
    }

    // Liberar memória alocada
    liberarMemoria(mapa);

    printf("\nJogo encerrado e memória liberada. Obrigado por jogar, até a próxima!\n");
    return 0;
}

// Função para cadastrar territórios
// Recebe ponteiro para o mapa e o número de territórios
// Usa fgets para ler strings com segurança e atribui valores via ponteiro

void cadastrarTerritorios(struct Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\nCadastro do território %d:\n", i + 1);

        printf("Digite o nome: ");
        if (fgets((mapa + i)->nome, sizeof((mapa + i)->nome), stdin) == NULL) {
            (mapa + i)->nome[0] = '\0'; // Em caso de erro, definir string vazia
        } else {
            (mapa + i)->nome[strcspn((mapa + i)->nome, "\n")] = '\0'; // Remover '\n'
        }

        printf("Digite a cor do exército: ");
        if (fgets((mapa + i)->cor, sizeof((mapa + i)->cor), stdin) == NULL) {
            (mapa + i)->cor[0] = '\0';
        } else {
            (mapa + i)->cor[strcspn((mapa + i)->cor, "\n")] = '\0';
        }

        printf("Digite a quantidade de tropas: ");
        if (scanf("%d", &((mapa + i)->tropas)) != 1) {
            (mapa + i)->tropas = 0;
        }
        // Limpar buffer até o fim da linha
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF) { }
    }
}

// Função para exibir os territórios.
// Percorre o vetor usando ponteiros e imprime os campos.
void exibirTerritorios(struct Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        struct Territorio* t = mapa + i;
        printf("\nTerritório %d\n", i + 1);
        printf("Nome: %s\n", t->nome);
        printf("Cor do exército: %s\n", t->cor);
        printf("N° de tropas: %d\n", t->tropas);
    }
}

// Função que simula um ataque entre dois territórios.
// Usa rand() para gerar um dado (1-6) para atacante e defensor.
// Se atacante vence (valor maior), defensor muda de cor e recebe metade das tropas do atacante.
// Se atacante perde (ou empata), o atacante perde uma tropa.
void atacar(struct Territorio* atacante, struct Territorio* defensor) {
    // Cada lado rola um dado de 1 a 6
    int dadoAt = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;

    printf("\nRolagem: Atacante(%s) = %d  vs  Defensor(%s) = %d\n", 
           atacante->cor, dadoAt, defensor->cor, dadoDef);

    if (dadoAt > dadoDef) {
        // Atacante vence: defensor perde 1 tropa
        defensor->tropas--;
        printf("Atacante venceu! Defensor perde 1 tropa.\n");
        
        // Se o defensor ficou sem tropas, o território é conquistado
        if (defensor->tropas <= 0) {
            printf("O território %s foi CONQUISTADO pelo exército %s!\n", defensor->nome, atacante->cor);
            
            // Mudança de dono (cor)
            strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
            defensor->cor[sizeof(defensor->cor) - 1] = '\0';
            defensor->tropas = atacante->tropas / 2; // Transfere metade das tropas quando há uma conquista
            atacante->tropas -= defensor->tropas;
        }
    } else {
        // Defensor vence ou empata: atacante perde 1 tropa
        atacante->tropas--;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("Defensor resistiu! Atacante perde 1 tropa.\n");
    }
}

// Função para liberar a memória alocada dinamicamente.
// Recebe o ponteiro alocado e chama free.
void liberarMemoria(struct Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
    }
}