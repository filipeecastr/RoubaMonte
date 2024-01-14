#include <stdio.h>
#include <stdlib.h>
#include <time.h>


// BARALHO

typedef struct Elemento{
    int naipe;
    int valor;
    int id;
    struct Elemento* proximo;
}Elemento;

typedef struct baralho{
    Elemento carta[52];
}baralho;

void inicializaBaralho(baralho *b){
    int i, j;
    for (i=0; i<4; i++){
        for (j=0; j<13; j++){
            b->carta[i*13 + j].naipe = i;
            b->carta[i*13 + j].valor = j+1;
        }
    }
}

void embaralhar(baralho *b){
    int i, j;
    Elemento temp;
    srand(time(NULL));
    for (i=0; i<52; i++){
        j = rand()%52;
        temp = b->carta[i];
        b->carta[i] = b->carta[j];
        b->carta[j] = temp;
    }
}

// PILHA/MONTE

typedef struct pilha{
    Elemento* topo;
    int qtd;
}Pilha;

Pilha* criarPilha(){
    Pilha* novaPilha = (Pilha*)malloc(sizeof(Pilha));
    if (novaPilha==NULL){
        printf("Erro ao alocar memoria!\n");
        exit(EXIT_FAILURE);
    }
    novaPilha->topo=NULL;
    novaPilha->qtd=0;

    return novaPilha;
}

void empilhar(Pilha* p, Elemento* carta) {
    Elemento* novoElemento = (Elemento*)malloc(sizeof(Elemento));
    if (novoElemento == NULL) {
        printf("Erro ao alocar memória!\n");
        exit(EXIT_FAILURE);
    }
    novoElemento->naipe = carta->naipe;
    novoElemento->valor = carta->valor;
    novoElemento->proximo = p->topo;
    p->topo = novoElemento;
    p->qtd++;
}

void desempilhar(Pilha* p) {
    if (p->topo != NULL) {
        Elemento* temp = p->topo;
        p->topo = p->topo->proximo;
        free(temp);
        p->qtd--;
    } else {
        printf("Pilha vazia!\n");
    }
}

// LISTA/MAO DO JOGADOR

typedef struct lista{
    int qtd;
    Elemento *vetor[4];
}Lista;

Lista* criarLista(){
    Lista *l;
    l=(Lista*) malloc(sizeof(Lista));
    if(l!=NULL){
        l->qtd=0;
    }
    return l;
}

int listaCheia(Lista* l){
    if(l->qtd==4){
        return 1;
    }
    else{
        return 0;
    }
}

int listaVazia(Lista* l){
    if(l->qtd==0){
        return 1;
    }
    else{
        return 0;
    }
}

void insereFinal(Lista* l, Elemento *v){
    if(listaCheia(l)==1){
        printf("Lista Cheia!\n");
    }
    else{
        l->vetor[l->qtd]=v;
        l->qtd++;
    }
}

void removerPosicao(Lista* l, int p){
    int i;
    if(listaVazia(l)==1){
        printf("Lista Vazia!\n");
    }
    else if(p<0||p>l->qtd){
        printf("Posicao Vazia!\n");

    }
    else{
        for(i=p-1; i<l->qtd-1; i++){
            l->vetor[i]=l->vetor[i+1];
        }
        l->qtd--;
    }
}

// Jogador //

typedef struct player{
    Pilha *p;
    Lista *l;
    int id;
}Jogador;

Jogador* inicializaPlayer(){
    Jogador* play = (Jogador*)malloc(sizeof(Jogador));
    play->l=criarLista();
    play->p=criarPilha();

    return play;
}

void distribuiCarta(Jogador *j, Pilha *b) {
    for (int i = 0; i < 4; i++) {
        Elemento* carta = (Elemento*)malloc(sizeof(Elemento));
        carta->naipe = b->topo->naipe;
        carta->valor = b->topo->valor;
        insereFinal(j->l, carta);
        desempilhar(b);
    }
}

int jogadorCampeao(Jogador* j[], int qtdJogadores){
    int campeao=0;
    for(int i = 1; i < qtdJogadores; i++){
        if(j[i]->p->qtd > j[campeao]->p->qtd){
            campeao=i;
        }
    }
    return campeao;
}

//Rouba o monte
int encontraIgual(Jogador* j[], int n, int jAtual, Elemento* cartaSelecionada, int card){
    for (int i = 0; i < n; i++) {
        if (i != jAtual && j[i]->p->qtd != 0) {
            if(cartaSelecionada->valor == j[i]->p->topo->valor){
                Pilha* temp = criarPilha();

                while (j[i]->p->topo != NULL) {
                    empilhar(temp, j[i]->p->topo);
                    desempilhar(j[i]->p);
                }

                while (temp->topo != NULL) {
                    empilhar(j[jAtual]->p, temp->topo);
                    desempilhar(temp);
                }
                empilhar(j[jAtual]->p, cartaSelecionada);

                free(temp);
                removerPosicao(j[jAtual]->l, card);

                return 1;
            }
        }
    }
    return 0;
}

// Impressao

void impressaoL(Elemento* l){
    switch (l->naipe){
        case 0:
            if(l->valor==1){
                printf("As de copas // \n");
            }
            else if(l->valor==11){
                printf("Valete de Copas // \n");
            }
            else if(l->valor==12){
                printf("Dama de Copas // \n");
            }
            else if(l->valor==13){
                printf("Rei de Copas // \n");
            }else{
                printf("%d de Copas // \n", l->valor);
            }
            break;

        case 1:
            if(l->valor==1){
                printf("As de Espadas // \n");
            }
            else if(l->valor==11){
                printf("Valete de Espadas // \n");
            }
            else if(l->valor==12){
                printf("Dama de Espadas //\n");
            }
            else if(l->valor==13){
                printf("Rei de Espadas //\n");
            }else{
                printf("%d de Espadas //\n", l->valor);
            }
            break;

        case 2:
            if(l->valor==1){
                printf("As de Ouros //\n");
            }
            else if(l->valor==11){
                printf("Valete de Ouros //\n");
            }
            else if(l->valor==12){
                printf("Dama de Ouros //\n");
            }
            else if(l->valor==13){
                printf("Rei de Ouros //\n");
            }else{
                printf("%d de Ouros //\n", l->valor);
            }
            break;

        case 3:
            if(l->valor==1){
                printf("As de Paus //\n");
            }
            else if(l->valor==11){
                printf("Valete de Paus //\n");
            }
            else if(l->valor==12){
                printf("Dama de Paus //\n");
            }
            else if(l->valor==13){
                printf("Rei de Paus //\n");
            }else{
                printf("%d de Paus //\n", l->valor);
            }
            break;
    }
}

void imprimirPilha(Pilha* p) {
    Elemento* atual = p->topo;

    if(p->topo == NULL){
        printf("Pilha vazia!\n");
        return;
    }

    int qtdCopiada = p->qtd;

    while (atual != NULL) {
        printf("%d - ", qtdCopiada);
        impressaoL(atual);
        atual = atual->proximo;
        qtdCopiada--;
    }

    printf("\n");
}

void imprimeMao(Jogador *j){
    for(int i=0; i<j->l->qtd; i++){
        impressaoL(j->l->vetor[i]);
    }
}

// MESA

typedef struct listaMesa{
    Elemento *inicio;
    int tam;
} ListaMesa;

ListaMesa* criaListaMesa(){
    ListaMesa* l = (ListaMesa*) malloc(sizeof(ListaMesa));
    l->inicio = NULL;
    l->tam = 0;
    return l;
}

void insereFim(ListaMesa *l, Elemento* carta){
    Elemento *novo=(Elemento*) malloc(sizeof(Elemento));
    novo = carta;
    novo->proximo = NULL;

    if (l->inicio == NULL){
        l->inicio = novo;
        l->tam++;
    }
    else{
        Elemento *no = l->inicio;
        while (no->proximo != NULL){
            no = no->proximo;
        }
        no->proximo = novo;
        l->tam++;
    }
}

void imprime(ListaMesa* l){
    Elemento *inicio = l->inicio;
    printf("\nLista:\n");
    while (inicio != NULL){
        impressaoL(inicio);
        inicio = inicio->proximo;
    }
    printf("\n");
}

void removerPosicaoMesa(ListaMesa *lista, int posicao) {
    if (posicao < 1 || posicao > lista->tam) {
        printf("Posicao invalida!\n");
        return;
    }

    Elemento *atual = lista->inicio;
    Elemento *anterior = NULL;

    for (int i = 1; i < posicao; i++) {
        anterior = atual;
        atual = atual->proximo;
    }

    if (anterior == NULL) {
        lista->inicio = atual->proximo;
    } else {
        anterior->proximo = atual->proximo;
    }

    lista->tam--;

    free(atual);

    Elemento *temp = lista->inicio;
    int novaPosicao = 1;
    while (temp != NULL) {
        temp = temp->proximo;
        novaPosicao++;
    }
}


int encontraPorValor(ListaMesa* l, Elemento* valor) {
    Elemento *atual = l->inicio;
    int posicao=0;

    while (atual != NULL) {
        if (atual->valor == valor->valor) {
            return posicao;
        }

        atual = atual->proximo;
        posicao++;
    }

    return -1;
}

Elemento* encontraPorCarta(ListaMesa* l, Elemento* valor){
    Elemento *atual = l->inicio;

    while (atual != NULL) {
        if (atual->valor == valor->valor) {
            return atual;
        }
        else{
        atual = atual->proximo;
        }
    }
}

void distribuiCartaMesa(ListaMesa* l, Pilha *b) {
    for (int i = 0; i < 4; i++) {
        Elemento* carta = (Elemento*)malloc(sizeof(Elemento));
        carta->naipe = b->topo->naipe;
        carta->valor = b->topo->valor;
        insereFim(l, carta);
        desempilhar(b);
    }
}

// Ordenacao

void insertionSort(Pilha* p) {
    if (p->topo == NULL || p->topo->proximo == NULL) {
        return;
    }

    Elemento* sorted = NULL;
    Elemento* current = p->topo;

    while (current != NULL) {
        Elemento* next = current->proximo;

        if (sorted == NULL || sorted->valor >= current->valor) {
            current->proximo = sorted;
            sorted = current;
        } else {
            Elemento* search = sorted;
            while (search->proximo != NULL && search->proximo->valor < current->valor) {
                search = search->proximo;
            }
            current->proximo = search->proximo;
            search->proximo = current;
        }
        current = next;
    }
    p->topo = sorted;
}

void ranking(Jogador* jog[], int qtd){
    int i, j;
    int qtdAux;
    Jogador* temp;
    for(i=0; i<qtd; i++){
        qtdAux = jog[i]->p->qtd;
        for(j = i+1; j<qtd; j++){
            if(jog[j]->p->qtd > qtdAux){
                temp=jog[i];
                jog[i]=jog[j];
                jog[j]=temp;
                qtdAux=jog[i]->p->qtd;
            }
        }
    }

    for(i = 0; i < qtd; i++){
        printf("\n%d lugar: Jogador %d - Cartas na pilha: %d\n", i+1, jog[i]->id+1, jog[i]->p->qtd);
    }
}

// MAIN

int main(){

    int jogadores, i=0;
    Pilha* pilhaBaralho=criarPilha();
    printf("\tBem Vindo ao\n---------ROUBA MONTE---------\n\nFeito por Filipe de Castro\n\n");
    printf("Digite a quantidade de jogadores: \n");
    scanf("%d", &jogadores);

    Jogador* p[jogadores];

    while (jogadores<2 || jogadores>4){
        printf("Digite a quantidade de jogadores entre 2 e 4: ");
        scanf("%d", &jogadores);       
            if (jogadores<2 || jogadores>4) {
                printf("Digite uma quantidade valida:\n");
            }
    }
        
    if(jogadores==2){
        baralho baralhoP;
        inicializaBaralho(&baralhoP);
        embaralhar(&baralhoP);

        for (int j = 0; j < 52; j++) {
            empilhar(pilhaBaralho, &baralhoP.carta[j]);
        }
        
    }else if(jogadores==3||jogadores==4){
        baralho baralhoP1, baralhoP2;
        inicializaBaralho(&baralhoP1);
        inicializaBaralho(&baralhoP2);

        embaralhar(&baralhoP1);
        embaralhar(&baralhoP2);

        for (int j = 0; j < 52; j++){
            empilhar(pilhaBaralho, &baralhoP1.carta[j]);
        }
        for (int j = 0; j < 52; j++){
            empilhar(pilhaBaralho, &baralhoP2.carta[j]);
        }

    }

    while(i<jogadores){
            p[i] = inicializaPlayer();
            distribuiCarta(p[i], pilhaBaralho);

            i++;
        }
    ListaMesa* listMesa=criaListaMesa();

    int winner=0;
    int cont;
    int jaGanhou=0;

    if(jogadores==2){
        distribuiCartaMesa(listMesa, pilhaBaralho);
    }else if(jogadores==3||jogadores==4){
        distribuiCartaMesa(listMesa, pilhaBaralho);
        distribuiCartaMesa(listMesa, pilhaBaralho);
    }

    system("cls");

    do{
        for(cont=0; cont<jogadores; cont++){
            if(listaVazia(p[cont]->l)==0){
                printf("Jogador %d\n", cont+1);
                imprimeMao(p[cont]);
                printf("\n");

                imprime(listMesa);
                printf("\n");

                printf("Tamanho do seu monte: %d\n", p[cont]->p->qtd);
                printf("Topo: ");
                if(p[cont]->p->topo==NULL){
                    printf("Monte Vazio\n\n");
                }
                else{
                    impressaoL(p[cont]->p->topo);
                    printf("\n");
                }

                int cartaSelecionada;
                printf("Selecione a carta que deseja jogar: ");
                scanf("%d", &cartaSelecionada);

                if(cartaSelecionada<0 || cartaSelecionada>4){
                    printf("Posicao Invalida!\n");
                    cont--;
                }else{

                    int posicaoTemp = encontraPorValor(listMesa, p[cont]->l->vetor[cartaSelecionada - 1]);
                    Elemento* cartaS = p[cont]->l->vetor[cartaSelecionada - 1];

                    if (posicaoTemp != -1) {
                        Elemento* cartaTemp = encontraPorCarta(listMesa, p[cont]->l->vetor[cartaSelecionada - 1]);
                        empilhar(p[cont]->p, cartaTemp);
                        empilhar(p[cont]->p, p[cont]->l->vetor[cartaSelecionada - 1]);
                        removerPosicao(p[cont]->l, cartaSelecionada);
                        removerPosicaoMesa(listMesa, posicaoTemp + 1);
                    }else {

                        int variavel = encontraIgual(p, jogadores, cont, cartaS, cartaSelecionada);

                        if(p[cont]->p->topo!=NULL&&cartaS->valor==p[cont]->p->topo->valor&&variavel!=1){
                            empilhar(p[cont]->p, cartaS);
                            removerPosicao(p[cont]->l, cartaSelecionada);
                            variavel=-1;
                        }

                        else if(variavel==0){
                            insereFim(listMesa, p[cont]->l->vetor[cartaSelecionada - 1]);
                            removerPosicao(p[cont]->l, cartaSelecionada);
                        }

                    }
                }
                imprimirPilha(p[cont]->p);
                imprime(listMesa);
                printf("\n");

            }else{
                if(pilhaBaralho->topo==NULL){
                    jaGanhou++;
                }
                else{
                    distribuiCarta(p[cont], pilhaBaralho);
                    cont--;
                }
            }
            system("cls");
        }
        if(jaGanhou==jogadores&&pilhaBaralho->topo == NULL){
            winner=1;
        }
    }while(winner==0);

    for(int qTemp=0; qTemp<jogadores; qTemp++){
        p[qTemp]->id=qTemp;
    }

    int campeao=jogadorCampeao(p, jogadores);

    printf("\n|--------- Jogador campeao: %d ---------|\n", campeao+1);

    ranking(p, jogadores);

    insertionSort(p[0]->p);

    printf("\nCartas do jogador %d ordenadas:\n", campeao+1);

    printf("\n");

    imprimirPilha(p[0]->p);
    
    for (int i = 0; i < jogadores; i++) {
        free(p[i]->l);
        free(p[i]->p);
        free(p[i]);
    }

    free(pilhaBaralho);
    free(listMesa);

    return 0;
}