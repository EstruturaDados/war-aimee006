// JOGO DE ESTRATÉGIA WAR ESTRUTURADO COM MISSÕES

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// ==================== ESTRUTURA ====================
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// ==================== PROTÓTIPOS ====================
void cadastrarTerritorios(Territorio* mapa, int n);
void exibirTerritorios(Territorio* mapa, int n);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa, char* missao);

void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
void exibirMissao(char* missao);
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador);

// ==================== FUNÇÃO PRINCIPAL ====================
int main() {
    srand((unsigned int)time(NULL));

    int n = 0;
    printf("\n=== CADASTRO (DINÂMICO) DE TERRITÓRIOS DO WAR ===\n");

    // Lê o número de territórios
    do {
        printf("Digite o número de territórios: ");
        if (scanf("%d", &n) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            n = 0;
        }
        int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
    } while (n <= 0);

    // Aloca o vetor de territórios dinamicamente
    Territorio* mapa = (Territorio*) calloc((size_t)n, sizeof(Territorio));
    if (mapa == NULL) {
        fprintf(stderr, "Erro: Falha na alocação de memória.\n");
        return 1;
    }

    cadastrarTerritorios(mapa, n);

    // Exibe o mapa inicial
    printf("\n=====================");
    printf("\n=== MAPA DO MUNDO ===\n");
    printf("=====================\n");
    exibirTerritorios(mapa, n);

    // Definição do jogador único
    char corJogador[10];
    printf("\nDigite a cor do seu exército: ");
    fgets(corJogador, sizeof(corJogador), stdin);
    corJogador[strcspn(corJogador, "\n")] = '\0';

    // Definição das missões possíveis
    char* missoes[] = {
        "Conquistar 2 territórios seguidos.",
        "Eliminar todas as tropas vermelhas.",
        "Controlar pelo menos 3 territórios."
    };
    int totalMissoes = 3;

    // Atribuir missão aleatória e armazenar dinamicamente
    char* missao = (char*) malloc(100 * sizeof(char));
    if (missao == NULL) {
        fprintf(stderr, "Erro: Falha na alocação da missão.\n");
        liberarMemoria(mapa, NULL);
        return 1;
    }
    atribuirMissao(missao, missoes, totalMissoes);

    // Exibir missão somente após o mapa
    printf("\n>>> Sua missão é: ");
    exibirMissao(missao);

    // ==================== MENU PRINCIPAL ====================
    int opcao;
    do {
        printf("\n=======================");
        printf("\n=== MENU DO JOGADOR ===\n");
        printf("=======================\n");
        printf("1 - Atacar\n");
        printf("2 - Verificar missão\n");
        printf("0 - Sair\n");
        printf("Escolha uma opção: ");
        
        if (scanf("%d", &opcao) != 1) {
            int c; while ((c = getchar()) != '\n' && c != EOF) {}
            opcao = -1;
        }
        int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}

        if (opcao == 1) {
            int atacanteIdx, defensorIdx;

            printf("Escolha o território ATACANTE (1 a %d): ", n);
            scanf("%d", &atacanteIdx);
            printf("Escolha o território DEFENSOR (1 a %d): ", n);
            scanf("%d", &defensorIdx);
            while ((ch = getchar()) != '\n' && ch != EOF) {}

            if (atacanteIdx < 1 || atacanteIdx > n || defensorIdx < 1 || defensorIdx > n) {
                printf("Índices inválidos. Tente novamente.\n");
                continue;
            }
            if (atacanteIdx == defensorIdx) {
                printf("Um território não pode atacar a si mesmo.\n");
                continue;
            }

            Territorio* atacante = mapa + (atacanteIdx - 1);
            Territorio* defensor = mapa + (defensorIdx - 1);

            if (strcmp(atacante->cor, defensor->cor) == 0) {
                printf("Ataque inválido! Mesma cor.\n");
                continue;
            }
            if (atacante->tropas < 1) {
                printf("Atacante sem tropas suficientes.\n");
                continue;
            }

            atacar(atacante, defensor);

            printf("\n=== MAPA DO MUNDO - PÓS ATAQUE ===\n");
            exibirTerritorios(mapa, n);
        }
        else if (opcao == 2) {
            printf("\n>>> Verificando status da missão...\n");
            if (verificarMissao(missao, mapa, n, corJogador)) {
                printf("\nPARABÉNS! Você CUMPRIU sua missão: %s\n", missao);
                liberarMemoria(mapa, missao);
                return 0;
            } else {
                printf("\nMissão ainda não cumprida. Continue tentando!\n");
            }
        }
        else if (opcao != 0) {
            printf("Opção inválida. Tente novamente.\n");
        }

    } while (opcao != 0);

    liberarMemoria(mapa, missao);
    printf("\nJogo encerrado e memória liberada. Obrigado por jogar!\n");
    return 0;
}

// ==================== FUNÇÕES AUXILIARES ====================

// Cadastro de territórios
void cadastrarTerritorios(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\nCadastro do território %d:\n", i + 1);

        printf("Nome: ");
        fgets((mapa + i)->nome, sizeof((mapa + i)->nome), stdin);
        (mapa + i)->nome[strcspn((mapa + i)->nome, "\n")] = '\0';

        printf("Cor do exército: ");
        fgets((mapa + i)->cor, sizeof((mapa + i)->cor), stdin);
        (mapa + i)->cor[strcspn((mapa + i)->cor, "\n")] = '\0';

        printf("Quantidade de tropas: ");
        scanf("%d", &((mapa + i)->tropas));
        int ch; while ((ch = getchar()) != '\n' && ch != EOF) {}
    }
}

// Exibição do mapa
void exibirTerritorios(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\nTerritório %d\n", i + 1);
        printf("Nome: %s\n", (mapa + i)->nome);
        printf("Cor do exército: %s\n", (mapa + i)->cor);
        printf("N° de tropas: %d\n", (mapa + i)->tropas);
    }
}

// Simulação de ataque
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoAt = (rand() % 6) + 1;
    int dadoDef = (rand() % 6) + 1;

    printf("\nRolagem: Atacante(%s)=%d vs Defensor(%s)=%d\n", 
           atacante->cor, dadoAt, defensor->cor, dadoDef);

    if (dadoAt > dadoDef) {
        defensor->tropas--;
        printf("Atacante venceu! Defensor perde 1 tropa.\n");
        if (defensor->tropas <= 0) {
            printf("Território %s foi conquistado pelo exército %s!\n", defensor->nome, atacante->cor);
            strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
            defensor->cor[sizeof(defensor->cor) - 1] = '\0';
            defensor->tropas = atacante->tropas / 2;
            atacante->tropas -= defensor->tropas;
        }
    } else {
        atacante->tropas--;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("Defensor resistiu! Atacante perde 1 tropa.\n");
    }
}

// Atribui missão aleatória
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    strcpy(destino, missoes[sorteio]);
}

// Exibe missão
void exibirMissao(char* missao) {
    printf("%s\n", missao);
}

// Verifica se a missão foi cumprida
int verificarMissao(char* missao, Territorio* mapa, int tamanho, char* corJogador) {
    int cont = 0;

    if (strstr(missao, "2 territórios seguidos")) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) cont++;
            else cont = 0;
            if (cont >= 2) return 1;
        }
    }
    else if (strstr(missao, "Eliminar todas as tropas vermelhas")) {
        for (int i = 0; i < tamanho; i++)
            if (strcmp(mapa[i].cor, "vermelho") == 0) return 0;
        return 1;
    }
    else if (strstr(missao, "Controlar pelo menos 3 territórios")) {
        int total = 0;
        for (int i = 0; i < tamanho; i++)
            if (strcmp(mapa[i].cor, corJogador) == 0) total++;
        if (total >= 3) return 1;
    }

    return 0;
}

// Liberação da memória
void liberarMemoria(Territorio* mapa, char* missao) {
    if (mapa != NULL) free(mapa);
    if (missao != NULL) free(missao);
}