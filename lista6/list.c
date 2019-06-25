#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INFINITO 0x3f3f3f3f

typedef struct listar_caminho
{
    int* caminhos;
    int ultimo;
}listar_caminho;


typedef struct node{
    int i;
    int q;
}node;

typedef struct no{
    struct no* next;
    int v;
    int d;
    int w;
    int custo;
}no;

typedef struct graf{
    int v;
    struct no* adj;
    struct no* tail;
    int** estoque;
    listar_caminho* antecessor;
    int* q_estoque_vert;
    int* calculados;
    int* atualizado;
    int** distancias;
    int size_v;
    int size_arest;
}graf;

typedef struct vertis{
    int d;
    int v;
}vertis;

typedef struct heap{
    vertis* vertis;
    int total;
}heap;

int get_pai(int n){
    return (int)(n/2);
}

int get_filho(int n){
	return (2*n);
}

heap* criar_heap(){
    heap* h     = (heap*)malloc(sizeof(heap));
    h->vertis   = (vertis*)malloc(10000*sizeof(vertis));
    h->total    = 0;
    return h;
}

void swap(heap* heaps, int ID1, int ID2){
    vertis temp           = heaps->vertis[  ID1 ];
    heaps->vertis[  ID1 ] = heaps->vertis[  ID2 ];
    heaps->vertis[  ID2 ] = temp;
}

void bubble_up(heap *heaps, int ID){
	int ID_Pai;
    if( ID != 1){
        ID_Pai = get_pai(ID);
        if( heaps->vertis[ID_Pai].d > heaps->vertis[ID].d ){   
            swap(       heaps, ID, ID_Pai);
            bubble_up(  heaps, ID_Pai);
        }
    }
}

void Insert_heap( heap *heaps, vertis v){
    heaps->total                  += 1;
    heaps->vertis[  heaps->total ] = v;
    bubble_up(      heaps, heaps->total);
}

void bubble_down(heap *heaps, int ID){
    int ID_Filho1, ID_Filho2, min = 0; 
    ID_Filho1                   = 2*ID;
    ID_Filho2                   = ID_Filho1 + 1;
    if( ID*2 < heaps->total   ){
        if(         heaps->vertis[  ID_Filho1   ].d > heaps->vertis[    ID_Filho2   ].d ){
            min       = ID_Filho2;
        }else if(   heaps->vertis[  ID_Filho1   ].d < heaps->vertis[    ID_Filho2   ].d ){
            min       = ID_Filho1;
        }else if(   heaps->vertis[  ID_Filho1   ].d == heaps->vertis[   ID_Filho2   ].d){
            if(     heaps->vertis[  ID_Filho1   ].v < heaps->vertis[    ID_Filho2   ].v){
                min   = ID_Filho2;
            }else{
                min   = ID_Filho1;
            }
        }
        if( heaps->vertis[  ID  ].d > heaps->vertis[    min ].d ){
            swap(               heaps, ID, min);
            bubble_down(        heaps, min );
        }else if( heaps->vertis[    ID  ].d ==  heaps->vertis[   min ].d ){
            if( heaps->vertis[      ID  ].v >   heaps->vertis[   min ].v){
                swap(           heaps, ID, min);
                bubble_down(    heaps, min );
            }
        }
    }else if(ID*2 == heaps->total){
        if( heaps->vertis[  ID   ].d > heaps->vertis[    ID*2   ].d ){
            swap( heaps, ID, ID*2);
        }
    }
}

vertis min_heap( heap* heaps){
    vertis valor, min;
    if( heaps->total > 0 ){
        min                                 = heaps->vertis[    1               ];// salvar para retornar min
        heaps->vertis[  1               ]   = heaps->vertis[    heaps->total    ];
        //heaps->vertis[  heaps->total    ]   = valor;//apagar
        heaps->total--;
        bubble_down(    heaps, 1    );
        return min;
    }else{
        return valor;
    }
}

no* cria_no(){
    no* n   = (no*)malloc(sizeof(no));
    return n;
}

node* cria_lista(int t){
    node* n = (node*)malloc(t*sizeof(node));
    return n;
}

graf* criar_grap( int v){
    graf* g                 = (graf*)malloc(sizeof(graf));
    g->adj                  = (no*)malloc(v*sizeof(no));
    g->tail                 = (no*)malloc(v*sizeof(no));
    g->antecessor           = (listar_caminho*)malloc(v*sizeof(listar_caminho));
    g->calculados           = (int*)malloc(v*sizeof(int));
    g->atualizado           = (int*)malloc(v*sizeof(int));
    g->distancias           = (int**)malloc(v*sizeof(int*));
    for( int i = 0 ; i < v ; i++){
        g->adj[i].next                  = NULL;
        g->tail[i].next                 = cria_no();
        g->antecessor[i].caminhos       = (int*)malloc(v*sizeof(int));
        g->distancias[i]                = (int*)malloc(v*sizeof(int));
        g->calculados[i]                = 0;
        g->atualizado[i]                = 0;
    }
    g->size_v               = v;
    g->size_arest           = 0;
    return g;
}

void add_arest(graf* g, int v1, int v2, int peso, int W){
    no* aux1;
    if( g->adj[v1].next ==  NULL ){
        g->adj[v1].next     = cria_no();
        aux1                = g->adj[v1].next;
        aux1->v             = v2;
        aux1->d             = peso;
        aux1->w             = W;
        aux1->custo         = (     peso*(  100 +   W   )   )/100;
        g->tail[v1].next    =  aux1;
        g->size_arest++;
    }else{
        aux1                = g->tail[v1].next;
        aux1->next          = cria_no();
        aux1->next->v       = v2;
        aux1->next->d       = peso;
        aux1->next->w       = W;
        aux1->next->custo   = (     peso*(  100 +   W   )   )/100;
        g->tail[v1].next    =  aux1->next;
    }
}

void dikst(graf* g, int inicio, int M, node* pedidos, int q_produtos){
    int index = 0;
    heap* heaps = criar_heap();
    vertis v_atual, v_adj;
    int comparador = INFINITO, custo, cursor;
    int mark_visit[     g->size_v   ];
    int mark_comparado[ g->size_v   ];
    for (int i = 0; i < g->size_v; i++){
        g->distancias[inicio][i]                    = INFINITO;
        mark_visit[i]                               = 0;
        mark_comparado[i]                           = 0;
        g->antecessor[inicio].caminhos[i]           = -1;
    }
    g->distancias[inicio][inicio]                   = 0;
    v_atual.v                                       = inicio;
    v_atual.d                                       = 0;
    Insert_heap(heaps, v_atual );
    while (heaps->total > 0 ){
        v_atual = min_heap(heaps);
        if( mark_visit[v_atual.v] == 0 ){
            mark_visit[v_atual.v]     = 1;
            if( mark_comparado[v_atual.v] != 1 ){
                comparador          = M;
                if( g->q_estoque_vert[v_atual.v] >= q_produtos){
                    for(     register int i = 0 ; i < M && comparador > 0 && g->estoque[    v_atual.v   ][  pedidos[i].i    ] >= pedidos[i].q  ; i++){
                        comparador--;
                    }
                }
                mark_comparado[v_atual.v] = 1;
                if( comparador == 0){
                    // atualizar estoque
                    comparador  = M;
                    for( register int i = 0 ; i < M && comparador > 0 && g->estoque[    v_atual.v   ][  pedidos[i].i    ] >= pedidos[i].q  ; i++){
                        comparador--;
                        g->estoque[v_atual.v][pedidos[i].i] -= pedidos[i].q;
                    }
                    g->q_estoque_vert[v_atual.v]   -= q_produtos;
                    printf("%d ",v_atual.v);
                    cursor                          = v_atual.v;
                    while (g->antecessor[inicio].caminhos[cursor] != -1){
                        printf("%d ",g->antecessor[inicio].caminhos[cursor]);
                        cursor                          = g->antecessor[inicio].caminhos[cursor];
                    }
                    printf("%d\n",g->distancias[inicio][ v_atual.v]);
                    g->antecessor[inicio].ultimo = v_atual.v;
                    return;
                }
            }
            for( no* cur = g->adj[v_atual.v].next ; cur != NULL ; cur = cur->next){
                v_adj.v                 = cur->v;
                v_adj.d                 = cur->custo;
                custo                   = cur->custo;
                if( g->distancias[inicio][v_adj.v] > g->distancias[inicio][v_atual.v] + custo  ){
                    g->distancias[inicio][v_adj.v]                  = g->distancias[inicio][v_atual.v]  + custo;
                    v_adj.d                                         = g->distancias[inicio][v_adj.v];
                    g->antecessor[inicio].caminhos[v_adj.v]   = v_atual.v;
                    Insert_heap(heaps,v_adj);
                } 
            }
        }
    }  
    if( comparador != 0){
        printf("OOS\n");
    }
}

int main(){

    int V, E, B, X, Y, D, W, T, M, I, Q, comparador, cursor;
    char Op[4];
    scanf(" %d %d %d ",&V, &E, &B);
    graf* g = criar_grap(V);
    for( register int i = 0; i < E ; i++){
        scanf(" %d %d %d %d ",&X, &Y, &D, &W);
        add_arest(g, X, Y, D, W);
        add_arest(g, Y, X, D, W);
    }
    g->estoque                  = (int**)malloc(V*sizeof(int*));
    g->q_estoque_vert           = (int*)malloc(V*sizeof(int));
    for( register int i = 0; i < V ; i++){
        g->estoque[i]           = (int*)malloc(B*sizeof(int));
        for ( register int j = 0; j < B; j++){
            scanf(" %d ",&Q);
            g->estoque[i][j]    = Q;
            g->q_estoque_vert[i]+= Q;
        }
    }
    do{
        scanf(" %s",Op);
        if( strcmp(Op, "ORD") == 0 ){
            scanf(" %d %d ",&T, &M);
            node* pedidos       = cria_lista(M);
            int q_produtos      = 0;
            for( register int i = 0 ; i < M; i++){
                scanf(" %d %d ",&I, &Q);
                pedidos[i].i    = I;
                pedidos[i].q    = Q;
                q_produtos     += Q;
            }
            if( g->calculados[T] == 0 || g->atualizado[T] == 1 ){
                dikst(g,T,M,pedidos,q_produtos);
                free(pedidos);
                g->calculados[T] = 1;
                if( g->atualizado[T] == 1){
                    g->atualizado[T] = 0;
                }
            }else {
                // laço para percorrer menor caminho
                    comparador  = M;
                    if( g->q_estoque_vert[T] >= q_produtos){
                        for( register int j = 0 ; j < M && g->estoque[  cursor  ][  pedidos[j].i    ] >= pedidos[j].q && cursor > 0  ; j++){
                            comparador--;
                        }
                    }
                    if( comparador == 0){
                        for( register int j = 0 ; j < M && g->estoque[   cursor   ][  pedidos[j].i    ] >= pedidos[j].q && cursor > 0 ; j++){
                            g->estoque[   cursor   ][  pedidos[j].i    ] -= pedidos[j].q;
                        }
                        g->q_estoque_vert[ T ] -= q_produtos;
                        //cursor = g->antecessor[T].ultimo;
                        printf("%d ",cursor);
                        
                        //cursor = g->antecessor[T].ultimo;
                        //cursor = g->antecessor[T].caminhos[cursor];
                        printf("---->%d\n",g->distancias[T][ cursor ]);
                        break;
                    } 
                }
                if( comparador > 0){
                    dikst(g,T,M,pedidos,q_produtos);
                    free(pedidos);
                }  
            
        }else if( strcmp(Op, "UPD") == 0 ){
            scanf(" %d %d %d ",&X, &Y, &W);
            for( no* cur = g->adj[X].next ; cur != NULL ; cur = cur->next ){
                if( cur->v == Y){
                    cur->w      = W;
                    cur->custo  = (     cur->d*(  100 +   W   )   )/100;
                    break;
                }
            }
            for( no* cur2 = g->adj[Y].next ; cur2 != NULL ; cur2 = cur2->next ){
                if( cur2->v == X){
                    cur2->w     = W;
                    cur2->custo = (     cur2->d*(  100 +   W   )   )/100;
                    break;
                }
            }
            g->atualizado[X] = 1;
            g->atualizado[Y] = 1;
        }else if( strcmp(Op, "STK") == 0 ){
            scanf(" %d %d %d ",&X, &I, &Q);
            g->estoque[X][I]   += Q;
        }
    }while( strcmp(Op, "END") != 0 );
    return 0;
}