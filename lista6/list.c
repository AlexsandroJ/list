#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define INFINITO 0x3f3f3f3f

typedef struct node{

    struct node* next;
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

void show_heap( heap* heap){
    int i = 1,n = 1, resultado = 0;
    printf("\n");

    for( i = 1 ; i <= heap->total  ; i++){

        printf(" |%d|V:%d|D:%d ",i,heap->vertis[i].v,heap->vertis[i].d);
        n++;
        if( n >= resultado){
            
            if( resultado == 0){
                //printf("-%d-",resultado);
                resultado = 2;
            }else{
                resultado = (resultado*resultado);
            }
            
            n = 0;
            printf("\n");
        }  
    }
    printf("\n");
}

int get_pai(int n){
    return (int)(n/2);
}

int get_filho(int n){
	return (2*n);
}

heap* criar_heap(int v){
    heap* h = (heap*)malloc(sizeof(heap));
    h->vertis = (vertis*)malloc(v*sizeof(vertis));
    h->total = 0;
}

void swap(heap* heap, int ID1, int ID2){
    vertis temp = heap->vertis[ID1];
    heap->vertis[ID1] = heap->vertis[ID2];
    heap->vertis[ID2] = temp;
}

int bubble_up(heap *heap, int ID){
	int ID_Pai, Criterio;
    if( ID == 1){
        return ID;
    }else{
        ID_Pai = get_pai(ID);
        
        if( heap->vertis[ID_Pai].d > heap->vertis[ID].d )
        {   
            swap( heap, ID, ID_Pai);
            bubble_up(heap, ID_Pai);
        }
    }
}

int Insert_heap( heap *heap, vertis v){
    heap->total += 1;
    heap->vertis[heap->total ] = v;
    return bubble_up(heap, heap->total);
}

void bubble_down(heap *heap, int ID){
    int ID_Filho1, ID_Filho2, min_ou_max_ID; 
    ID_Filho1 = 2*ID;
    ID_Filho2 = ID_Filho1 + 1;
    if( ID*2 < heap->total   ){
        if( heap->vertis[ID_Filho1].d > heap->vertis[ID_Filho2].d ){
            min_ou_max_ID = ID_Filho2;
        }else if( heap->vertis[ID_Filho1].d < heap->vertis[ID_Filho2].d ){
            min_ou_max_ID = ID_Filho1;
        }else if( heap->vertis[ID_Filho1].d == heap->vertis[ID_Filho2].d)
        {
            if( heap->vertis[ID_Filho1].v < heap->vertis[ID_Filho2].d){
                min_ou_max_ID = ID_Filho2;
            }else
            {
                min_ou_max_ID = ID_Filho1;
            }
        }
        swap( heap, ID, min_ou_max_ID);
        bubble_down( heap, min_ou_max_ID );
    }else if(ID < heap->total){
        if( heap->vertis[1].d > heap->vertis[2].d ){
            swap( heap, 1, 2);
        }
    }
}

vertis min_heap( heap* heap){
    int ID_Che;
    vertis valor, min;
    if( heap->total > 0 ){
        min = heap->vertis[1];
        heap->vertis[1] = heap->vertis[heap->total];
        heap->vertis[heap->total] = valor;
        heap->total -=1;
        bubble_down(heap, 1);
        return min;
    }
}

no* cria_no(){
    no* n = (no*)malloc(sizeof(no));
    return n;
}
node* cria_node(){
    node* n = (node*)malloc(sizeof(node));
    return n;
}

graf* criar_grap(graf* g, int v){
    g = (graf*)malloc(sizeof(graf));
    g->adj = (no*)malloc(v*sizeof(no));
    g->tail = (no*)malloc(v*sizeof(no));
    for( int i = 0 ; i < v ; i++){
        g->adj[i].next = NULL;
        g->tail[i].next = cria_no();
    }
    g->size_v = v;
    g->size_arest= 0;
}

void add_arest(graf* g, int v1, int v2, int peso, int W){
    no* aux1;
    if( g->adj[v1].next ==  NULL ){
        g->adj[v1].next     = cria_no();
        aux1                = g->adj[v1].next;
        aux1->v             = v2;
        aux1->d             = peso;
        aux1->w             = W;
        //aux1->custo         = peso;
        aux1->custo         = (     peso*(  100 +   W   )   )/100;
        g->tail[v1].next    =  aux1;
        g->size_arest++;

    }else{
        aux1                = g->tail[v1].next;
        aux1->next          = cria_no();
        aux1->next->v       = v2;
        aux1->next->d       = peso;
        aux1->next->w       = W;
        //aux1->next->custo   = peso;
        aux1->next->custo   = (     peso*(  100 +   W   )   )/100;
        g->tail[v1].next    =  aux1->next;
    }
    
}

void show_grafro(graf* g){
    no* cur;

    for(int i = 0 ; i < g->size_v ; i++){
        if( g->adj[i].next != NULL){
            cur = g->adj[i].next ;
        }
        printf("|%d|",i);
        while ( cur != NULL)
        {
            printf("--%d-->|%d|",cur->custo,cur->v);
            cur = cur->next;
        }
        printf("\n");
    }
}

int dikst(graf* g, int inicio, int M, node* pedidos){

    heap* heap = criar_heap(g->size_v);
    vertis aux, v_atual, v_adj;
    no* cur = g->adj[inicio].next;
    int mark[g->size_v], custo, dist[g->size_v], comparado[g->size_v], cur_caminho = 0, comparador = INFINITO;
    int anterior[g->size_v];
    for (int i = 0; i < g->size_v; i++){
        dist[i]         = INFINITO;
        mark[i]         = 0;
        comparado[i]      = 0;
        anterior[i]     = -1;
    }
    dist[inicio]        = 0;
    v_atual.v           = inicio;
    v_atual.d           = 0;
    //printf("\nInicio %d\n",inicio);
    Insert_heap(heap, v_atual );
    while (heap->total > 0 )
    {
        v_atual = min_heap(heap);
        //printf("\n\t\t\t Saio da Heap:%d anterior %d\n",v_atual.v,anterior[v_atual.v]);
        if( mark[v_atual.v] == 0 ){
            mark[v_atual.v]     = 1;

            if( comparado[v_atual.v] != 1 ){
                comparador = M;
                for( node* i = pedidos ; i != NULL && comparador > 0 && g->estoque[v_atual.v][i->i] >= i->q  ; i = i->next){
                    //printf("|%d|%d-%d ",comparador,g->estoque[v_atual.v][i->i] , i->q);
                    comparador--;
                }
                comparado[v_atual.v] = 1;
                //printf("Caminho ADD %d D:%d\n",v_atual.v,dist[ v_atual.v]  );
                if( comparador == 0){
                    // atualizar estoque
                    comparador = M;
                    for( node* j = pedidos ; j != NULL && comparador > 0 && g->estoque[v_atual.v][j->i] >= j->q  ; j = j->next){
                        g->estoque[v_atual.v][j->i] = g->estoque[v_atual.v][j->i] - j->q ;
                    }
                    printf("%d ",v_atual.v);
                    int cursor = v_atual.v;
                    while (anterior[cursor] != -1){
                        printf("%d ",anterior[cursor]);
                        cursor = anterior[cursor];
                    }
                    printf("%d\n",dist[ v_atual.v]);
                    return 1;
                }
            }
            for( no* cur = g->adj[v_atual.v].next ; cur != NULL ; cur = cur->next){
                v_adj.v                 = cur->v;
                v_adj.d                 = cur->custo;
                custo                   = cur->custo;
                //printf("| atual:%d ajd %d| ",v_atual.v,v_adj.v);
                if( dist[v_adj.v] > dist[v_atual.v] + custo ){
                    dist[v_adj.v]       = dist[v_atual.v]  + custo;
                    v_adj.d             = dist[v_adj.v];
                    Insert_heap(heap,v_adj);
                    anterior[v_adj.v]   = v_atual.v;
                    //printf("V:%d Vajd:%d AntV:%d AntAdj:%d\n",v_atual.v,v_adj.v,anterior[v_atual.v],anterior[v_adj.v] ); 
                } 
            }
        }
        //show_heap(heap);
    }  
    if( comparador != 0){
        printf("OOS\n");
        return 0;
    }
}
void testarHeap(heap* h){
    vertis t;
    for( int i = 0 ; i < 50 ; i++){
        t.v = i;
        t.d = i;
        Insert_heap(h,t);
    }
    show_heap(h);
    
}
/*
int main(){

    graf* g = criar_grap(   g, 5);
    heap* h = criar_heap(   5);
    add_arest(g, 0,3,4,1 );
    add_arest(g, 1,0,2,1 );
    add_arest(g, 1,2,3,1 );
    add_arest(g, 1,3,5,1 );
    add_arest(g, 2,3,1,1 );
    add_arest(g, 3,4,1,1 );
    add_arest(g, 4,2,1,1 );
    show_grafro(g);
    dikst(g,h,1,4);
    return 0;
}
*/
int main(){

    register int i,j;
    int V, E, B, X, Y, D, W, T, M, I, Q, aux1,aux2;
    char Op[4];
    scanf(" %d %d %d ",&V, &E, &B);
    //printf("%d %d %d\n",V,E,B);
    graf* g = criar_grap(   g, V);
    for( i = 0; i < E ; i++){
        scanf(" %d %d %d %d ",&X, &Y, &D, &W);
        //printf("%d %d %d %d\n",X, Y, D, W);
        add_arest(g, X, Y, D, W);
        add_arest(g, Y, X, D, W);
    }
    //show_grafro(g);
    g->estoque = (int**)malloc(V*sizeof(int*));

    for( i = 0; i < V ; i++){

        g->estoque[i] = (int*)malloc(B*sizeof(int));

        for (int j = 0; j < B; j++)
        {
            scanf(" %d ",&Q);
            //printf("%d ",aux1);
            g->estoque[i][j] = Q;
        }
    }

    do{
        scanf(" %s",Op);
        if( strcmp(Op, "ORD") == 0 ){
            scanf(" %d %d ",&T, &M);
            //printf("ORD %d %d",T,M);
            node* pedidos = cria_node();
            node* aux = pedidos;
            for( int  i = 0 ; i < M; i++){
                scanf(" %d %d ",&I, &Q);
                //printf(" %d %d",I,Q);
                aux->i = I;
                aux->q = Q;
                aux->next = cria_node();
                aux = aux->next;
            }
            /*printf("\n");
            aux = pedidos;
            for( int i = 0 ; i < M; i++){
                printf("%d %d ",aux->i,aux->q);
                aux = aux->next;
            }
            printf("\n");
            */
            dikst(g,T,M,pedidos);
            free(pedidos);

            
        }else if( strcmp(Op, "UPD") == 0 ){
            scanf(" %d %d %d ",&X, &Y, &W);
            for( no* cur = g->adj[X].next ; cur != NULL ; cur = cur->next ){
                if( cur->v == Y){
                    cur->w = W;
                    cur->custo = (     cur->d*(  100 +   W   )   )/100;
                    break;
                }
            }
            for( no* cur = g->adj[Y].next ; cur != NULL ; cur = cur->next ){
                if( cur->v == X){
                    cur->w = W;
                    cur->custo = (     cur->d*(  100 +   W   )   )/100;
                    break;
                }
            }
        }else if( strcmp(Op, "STK") == 0 ){
            scanf(" %d %d %d ",&X, &I, &Q);
            g->estoque[X][I] = g->estoque[X][I] + Q;
        }
    }while( strcmp(Op, "END") != 0 );
    return 0;
}