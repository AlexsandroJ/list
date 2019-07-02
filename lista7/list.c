#include <stdio.h>
#include <stdlib.h>
#define vazio -999999
void printar(int* tab_linhas, int n){
    for( int i = 0; i < n ; i++){
        for (int j = 0; j < n; j++){
            if( tab_linhas[j] == i){
                printf("1 ");
            }else{
                printf("0 ");
            }
        }
        printf("\n");
    }
    printf("\n\n");
}

void printar_matriz(int** tab_linhas, int n){
    for( int i = 0; i < 10 ; i++){
        for (int j = 0; j < n; j++){
            printf("%d ",tab_linhas[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}
void printar_solucoes(int**solucoes , int* solucoes_para_printar, int n){
    for (int j = 0; j < n; j++){
        show( solucoes [ solucoes_para_printar[j] ], n );
    }
    printf("\n");
}

void show(int* tab_linhas, int n){
    for( int i = 0; i < n ; i++){
        printf("%d ",tab_linhas[i]);
    }
    printf("\n");
}

int checar_2d(int* tab_linhas, int n, int linha, int coluna){
    if( tab_linhas[coluna] != vazio){
        //printf("Ja existe Rainha na Coluna %d\n",coluna);       // ja existe rainha na coluna
        return 0;
    }
    for( int i = 0; i < n; i++){
        if( tab_linhas[i] == linha ){
            //printf("Mesma Linha\n");                 // mesma linha
            return 0;
        }          
        if( (tab_linhas[i] - linha)*(tab_linhas[i] - linha) == (i - coluna)*(i - coluna) ){
            //printf("Mesma Diagonal linha:%d coluna:%d\n",linha,coluna);return 0;      // mesma diagonal
            return 0;
        }
    }
    //printf("ok linha %d coluna %d\n",linha,coluna);
    return 1;
}

void bracktracking(int* q_solucoes, int* tabela_linhas, int n, int coluna, int** solucoes){
    if( coluna == n ){
        for( int i = 0 ; i < n ; i++){
            solucoes[*q_solucoes][i] = tabela_linhas[i];
        }
        *q_solucoes = *q_solucoes + 1;
        printf("%d\n",*q_solucoes);
        show(tabela_linhas,n);
        return;
    }
    for (int j = 0; j < n; j++){    
        if( checar_2d(tabela_linhas,n,j,coluna ) ){
            tabela_linhas[coluna]   = j;
            coluna++;
            bracktracking(q_solucoes,tabela_linhas, n, coluna, solucoes);
            coluna--;
            tabela_linhas[coluna]   = vazio;
        }else if( tabela_linhas[coluna] != vazio){
            coluna++;
            bracktracking(q_solucoes,tabela_linhas, n, coluna, solucoes);
            return;
        }
    }
}
/*
int checar_3d(int** solucoes, int* q_solucoes, int n, int altura, int** matriz_ocupacao){
    int c_solu;
    for( int i = 0 ; i < n ; i++){
        c_solu = solucoes[* q_solucoes][i];
        if( matriz_ocupacao[c_solu][i] != vazio){
            printf("nao esta vazio\n");
            return 0;
        }
    }
    printf("esta vazio\n");
    return 1;
}*/
void adicionar_solucao( int** solucoes, int** matriz_ocupacao, int id, int n, int altura){
    int c_solu;
    for(int i = 0 ; i < n ; i++){
        c_solu                          = solucoes[id][i];
        matriz_ocupacao[c_solu][i]      = altura;
    }
}
void zerar_ocupacao( int** solucoes, int** matriz_ocupacao, int id, int n, int altura){
    int c_solu;
    for(int i = 0 ; i < n ; i++){
        c_solu                          = solucoes[id][i];
        matriz_ocupacao[c_solu][i]      = vazio;
    }
}

int checar_3d(int ** matriz_ocupacao, int** solucoes, int id, int n){
    int c_solu;
    for (int i = 0; i < n; i++){ 
        c_solu = solucoes[id][i];  
        if( matriz_ocupacao[c_solu][i] != vazio ){
            return 0;
        }
    }
    return 1;
}

void bracktracking2(int** solucoes, int* q_solucoes, int n, int altura, int** matriz_ocupacao, int* solucoes_para_printar, int* q_total){
    if( altura == n ){
        *q_total = *q_total + 1;
        printar_solucoes(solucoes,solucoes_para_printar, n);
        return;
    }
    for( int i = 0 ; i < * q_solucoes; i++){
        if( checar_3d(matriz_ocupacao, solucoes, i, n) ){
            adicionar_solucao(solucoes, matriz_ocupacao, i , n, altura);
            solucoes_para_printar[altura] = i;
            bracktracking2(solucoes, q_solucoes, n, altura + 1, matriz_ocupacao, solucoes_para_printar, q_total);
            zerar_ocupacao(solucoes, matriz_ocupacao, i , n, altura);
           // solucoes_para_printar[altura] = vazio;
        }
    }
}

int** alocar(int n){
    int** m = (int**)malloc(n*sizeof(int*));
    for (int i = 0; i < n; i++){
       m[i] = (int*)malloc(n*sizeof(int));
       for (int j = 0; j < n; j++){
           m[i][j] = vazio;
       }
    }
    return m;
}
int* alocar_n(int n){
    int* m = (int*)malloc(n*sizeof(int));
    for (int i = 0; i < n; i++){
        m[i] = vazio;
    }
    return m;
}
void zerar(int* m,int n){
    for (int i = 0; i < n; i++){
       m[i] = vazio;
    }
}

int main(){
    
    int n,p,f,r,c;
    int* q_solucoes     = (int*)malloc(sizeof(int));
    int* q_total        = (int*)malloc(sizeof(int));
    int** solucoes      = (int**)malloc(10*sizeof(int*));
    *q_solucoes         = 0;
    *q_total            = 0;
    for (int i = 0; i < 11; i++){
       solucoes[i] = (int*)malloc(5*sizeof(int));
       for (int j = 0; j < 5; j++){
           solucoes[i][j] = vazio;
       }
    }
    

    //scanf(" %d %d %d",&n,&p,&f);
    int* tabulheiro         = alocar_n(5);/*
    for (int i = 0; i < p; i++){
        scanf(" %d %d",&r,&c);
        tabulheiro[f][c] = r;
    }*/
    int** matriz_ocupacao   = alocar(5);
    //printar(tabulheiro[0],5);
    //for (int i = 0; i < n; i++){
        bracktracking(q_solucoes,tabulheiro,5,0, solucoes);
        //bracktracking(q_solucoes,tabulheiro,5,0,1,matriz_ocupacao);
        //printar_matriz(matriz_ocupacao,5);
        //bracktracking(q_solucoes,tabulheiro,5,0,2,matriz_ocupacao);
    //}
    printf("%d\n",*q_solucoes);
    //printar_matriz(solucoes,5);
    solucoes[4][1] = 3;
    //checar_3d(solucoes,q_solucoes,5,0,matriz_ocupacao);
    int* solucoes_para_printar = alocar_n(*q_solucoes);

    bracktracking2(solucoes, q_solucoes, 5, 0, matriz_ocupacao, solucoes_para_printar, q_total);
    printf("%d\n",*q_total);
    return 0 ;
}