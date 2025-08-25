#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h> // Para medir tempo, se desejado

#define MAX_ITENS 10 // Capacidade máxima da mochila com vetor

// --- ESTRUTURAS GERAIS ---

/**
 * @brief Estrutura que representa um item na mochila.
 */
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
} Item;

/**
 * @brief Estrutura para o nó da lista encadeada.
 */
typedef struct No {
    Item dados;
    struct No* proximo;
} No;

// Função auxiliar para limpar o buffer de entrada
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// =============================================================================
// === IMPLEMENTAÇÃO COM VETOR (LISTA SEQUENCIAL) ==============================
// =============================================================================

typedef struct {
    Item itens[MAX_ITENS];
    int totalItens;
    int ordenado; // 0 para não ordenado, 1 para ordenado
} MochilaVetor;

void inicializarVetor(MochilaVetor* mochila) {
    mochila->totalItens = 0;
    mochila->ordenado = 1; // Uma lista vazia está, por definição, ordenada
    printf("Mochila com vetor inicializada.\n");
}

void inserirItemVetor(MochilaVetor* mochila) {
    if (mochila->totalItens >= MAX_ITENS) {
        printf("Erro: A mochila (vetor) está cheia!\n");
        return;
    }

    Item novoItem;
    printf("Nome do item: ");
    fgets(novoItem.nome, 30, stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = 0;

    printf("Tipo do item: ");
    fgets(novoItem.tipo, 20, stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = 0;

    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);
    limparBuffer();

    mochila->itens[mochila->totalItens] = novoItem;
    mochila->totalItens++;
    mochila->ordenado = 0; // A inserção pode quebrar a ordenação
    printf("Item '%s' inserido no vetor.\n", novoItem.nome);
}

void removerItemVetor(MochilaVetor* mochila, const char* nome) {
    int i, j;
    int encontrado = -1;

    for (i = 0; i < mochila->totalItens; i++) {
        if (strcmp(mochila->itens[i].nome, nome) == 0) {
            encontrado = i;
            break;
        }
    }

    if (encontrado != -1) {
        // Desloca os itens para cobrir o espaço removido
        for (j = encontrado; j < mochila->totalItens - 1; j++) {
            mochila->itens[j] = mochila->itens[j + 1];
        }
        mochila->totalItens--;
        // A remoção não quebra a ordenação, então não precisa mexer na flag 'ordenado'
        printf("Item '%s' removido do vetor.\n", nome);
    } else {
        printf("Item '%s' não encontrado no vetor.\n", nome);
    }
}

void listarItensVetor(const MochilaVetor* mochila) {
    printf("\n--- Itens na Mochila (Vetor) [%d/%d] ---\n", mochila->totalItens, MAX_ITENS);
    if (mochila->totalItens == 0) {
        printf("A mochila está vazia.\n");
        return;
    }
    for (int i = 0; i < mochila->totalItens; i++) {
        printf("- %s (Tipo: %s, Qtd: %d)\n", mochila->itens[i].nome, mochila->itens[i].tipo, mochila->itens[i].quantidade);
    }
    printf("----------------------------------------\n");
}

/**
 * @brief Ordena o vetor de itens por nome usando Bubble Sort.
 * Este é um algoritmo simples, mas ineficiente para grandes listas.
 */
void ordenarVetor(MochilaVetor* mochila) {
    if (mochila->ordenado) {
        printf("Vetor já está ordenado.\n");
        return;
    }

    int i, j;
    Item temp;
    for (i = 0; i < mochila->totalItens - 1; i++) {
        for (j = 0; j < mochila->totalItens - 1 - i; j++) {
            // Compara os nomes e troca se estiverem fora de ordem
            if (strcmp(mochila->itens[j].nome, mochila->itens[j + 1].nome) > 0) {
                temp = mochila->itens[j];
                mochila->itens[j] = mochila->itens[j + 1];
                mochila->itens[j + 1] = temp;
            }
        }
    }
    mochila->ordenado = 1;
    printf("Mochila (vetor) ordenada com sucesso!\n");
}

int buscarSequencialVetor(const MochilaVetor* mochila, const char* nome, int* comparacoes) {
    *comparacoes = 0;
    for (int i = 0; i < mochila->totalItens; i++) {
        (*comparacoes)++; // Incrementa o contador a cada comparação
        if (strcmp(mochila->itens[i].nome, nome) == 0) {
            return i; // Retorna o índice se encontrou
        }
    }
    return -1; // Retorna -1 se não encontrou
}

int buscarBinariaVetor(const MochilaVetor* mochila, const char* nome, int* comparacoes) {
    *comparacoes = 0;
    if (!mochila->ordenado) {
        printf("Erro: A busca binária requer que o vetor esteja ordenado!\n");
        return -2; // Código de erro para vetor não ordenado
    }

    int inicio = 0;
    int fim = mochila->totalItens - 1;
    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;
        (*comparacoes)++; // Incrementa o contador a cada comparação no laço
        int res = strcmp(nome, mochila->itens[meio].nome);

        if (res == 0) {
            return meio; // Encontrou
        }
        if (res > 0) {
            inicio = meio + 1; // Busca na metade direita
        } else {
            fim = meio - 1; // Busca na metade esquerda
        }
    }
    return -1; // Não encontrou
}


// =============================================================================
// === IMPLEMENTAÇÃO COM LISTA ENCADEADA =======================================
// =============================================================================

typedef struct {
    No* inicio;
    int totalItens;
} MochilaLista;

void inicializarLista(MochilaLista* mochila) {
    mochila->inicio = NULL;
    mochila->totalItens = 0;
    printf("Mochila com lista encadeada inicializada.\n");
}

void inserirItemLista(MochilaLista* mochila) {
    Item novoItem;
    printf("Nome do item: ");
    fgets(novoItem.nome, 30, stdin);
    novoItem.nome[strcspn(novoItem.nome, "\n")] = 0;

    printf("Tipo do item: ");
    fgets(novoItem.tipo, 20, stdin);
    novoItem.tipo[strcspn(novoItem.tipo, "\n")] = 0;

    printf("Quantidade: ");
    scanf("%d", &novoItem.quantidade);
    limparBuffer();

    No* novoNo = (No*)malloc(sizeof(No));
    if (!novoNo) {
        printf("Erro de alocação de memória!\n");
        return;
    }
    novoNo->dados = novoItem;
    novoNo->proximo = mochila->inicio; // Insere no início da lista (mais rápido)
    mochila->inicio = novoNo;
    mochila->totalItens++;
    printf("Item '%s' inserido na lista.\n", novoItem.nome);
}

void removerItemLista(MochilaLista* mochila, const char* nome) {
    No* atual = mochila->inicio;
    No* anterior = NULL;

    while (atual != NULL && strcmp(atual->dados.nome, nome) != 0) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (atual == NULL) {
        printf("Item '%s' não encontrado na lista.\n", nome);
        return;
    }

    if (anterior == NULL) { // O item a ser removido é o primeiro
        mochila->inicio = atual->proximo;
    } else { // O item está no meio ou no fim
        anterior->proximo = atual->proximo;
    }

    free(atual);
    mochila->totalItens--;
    printf("Item '%s' removido da lista.\n", nome);
}

void listarItensLista(const MochilaLista* mochila) {
    printf("\n--- Itens na Mochila (Lista) [%d itens] ---\n", mochila->totalItens);
    if (mochila->inicio == NULL) {
        printf("A mochila está vazia.\n");
        return;
    }
    No* atual = mochila->inicio;
    while (atual != NULL) {
        printf("- %s (Tipo: %s, Qtd: %d)\n", atual->dados.nome, atual->dados.tipo, atual->dados.quantidade);
        atual = atual->proximo;
    }
    printf("-------------------------------------------\n");
}

No* buscarSequencialLista(const MochilaLista* mochila, const char* nome, int* comparacoes) {
    *comparacoes = 0;
    No* atual = mochila->inicio;
    while (atual != NULL) {
        (*comparacoes)++; // Incrementa a cada comparação
        if (strcmp(atual->dados.nome, nome) == 0) {
            return atual; // Retorna o ponteiro para o nó encontrado
        }
        atual = atual->proximo;
    }
    return NULL; // Não encontrou
}

void liberarLista(MochilaLista* mochila) {
    No* atual = mochila->inicio;
    No* proximo;
    while (atual != NULL) {
        proximo = atual->proximo;
        free(atual);
        atual = proximo;
    }
    mochila->inicio = NULL;
    mochila->totalItens = 0;
}

// =============================================================================
// === MENUS E FUNÇÃO PRINCIPAL ================================================
// =============================================================================

void menuVetor() {
    MochilaVetor mochila;
    inicializarVetor(&mochila);
    int opcao;
    char nomeBusca[30];
    int comparacoes;

    do {
        printf("\n--- Menu Mochila (VETOR) ---\n");
        printf("1. Inserir item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Ordenar itens (para busca binária)\n");
        printf("5. Buscar (Sequencial)\n");
        printf("6. Buscar (Binária)\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                inserirItemVetor(&mochila);
                break;
            case 2:
                printf("Nome do item a remover: ");
                fgets(nomeBusca, 30, stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = 0;
                removerItemVetor(&mochila, nomeBusca);
                break;
            case 3:
                listarItensVetor(&mochila);
                break;
            case 4:
                ordenarVetor(&mochila);
                break;
            case 5:
                printf("Nome do item a buscar (sequencial): ");
                fgets(nomeBusca, 30, stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = 0;
                int indiceSeq = buscarSequencialVetor(&mochila, nomeBusca, &comparacoes);
                if (indiceSeq != -1) {
                    printf(">> Item encontrado na posição %d.\n", indiceSeq);
                } else {
                    printf(">> Item não encontrado.\n");
                }
                printf(">> Comparações realizadas: %d\n", comparacoes);
                break;
            case 6:
                printf("Nome do item a buscar (binária): ");
                fgets(nomeBusca, 30, stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = 0;
                int indiceBin = buscarBinariaVetor(&mochila, nomeBusca, &comparacoes);
                if (indiceBin >= 0) {
                    printf(">> Item encontrado na posição %d.\n", indiceBin);
                } else if (indiceBin == -1) {
                    printf(">> Item não encontrado.\n");
                }
                // Se indiceBin for -2, a mensagem de erro já foi impressa na função.
                if (indiceBin != -2) {
                    printf(">> Comparações realizadas: %d\n", comparacoes);
                }
                break;
            case 0:
                printf("Voltando ao menu principal...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);
}

void menuLista() {
    MochilaLista mochila;
    inicializarLista(&mochila);
    int opcao;
    char nomeBusca[30];
    int comparacoes;

    do {
        printf("\n--- Menu Mochila (LISTA ENCADEADA) ---\n");
        printf("1. Inserir item\n");
        printf("2. Remover item\n");
        printf("3. Listar itens\n");
        printf("4. Buscar (Sequencial)\n");
        printf("0. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                inserirItemLista(&mochila);
                break;
            case 2:
                printf("Nome do item a remover: ");
                fgets(nomeBusca, 30, stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = 0;
                removerItemLista(&mochila, nomeBusca);
                break;
            case 3:
                listarItensLista(&mochila);
                break;
            case 4:
                printf("Nome do item a buscar (sequencial): ");
                fgets(nomeBusca, 30, stdin);
                nomeBusca[strcspn(nomeBusca, "\n")] = 0;
                No* encontrado = buscarSequencialLista(&mochila, nomeBusca, &comparacoes);
                if (encontrado != NULL) {
                    printf(">> Item encontrado!\n");
                } else {
                    printf(">> Item não encontrado.\n");
                }
                printf(">> Comparações realizadas: %d\n", comparacoes);
                break;
            case 0:
                printf("Liberando memória da lista e voltando...\n");
                liberarLista(&mochila);
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);
}

int main() {
    int opcao;
    do {
        printf("\n====== COMPARANDO ESTRUTURAS DE DADOS ======\n");
        printf("Escolha o tipo de mochila para gerenciar:\n");
        printf("1. Mochila com Vetor (Rápida para busca ordenada)\n");
        printf("2. Mochila com Lista Encadeada (Flexível)\n");
        printf("0. Sair do programa\n");
        printf("============================================\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                menuVetor();
                break;
            case 2:
                menuLista();
                break;
            case 0:
                printf("Até a próxima aventura!\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opcao != 0);

    return 0;
}