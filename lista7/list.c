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

void show(int* tab_linhas, int n){
    for( int i = 0; i < n ; i++){
        printf("%d ",tab_linhas[i]);
    }
    printf("\n");
}

int checar_2d(int** tab_linhas, int n, int linha, int coluna, int altura){
    if( tab_linhas[altura][coluna] != vazio){
        //printf("Ja existe Rainha na Coluna %d\n",coluna);       // ja existe rainha na coluna
        return 0;
    }
    for( int i = 0; i < n; i++){
        if( tab_linhas[altura][i] == linha ){
            //printf("Mesma Linha\n");                 // mesma linha
            return 0;
        }          
        if( (tab_linhas[altura][i] - linha)*(tab_linhas[altura][i] - linha) == (i - coluna)*(i - coluna) ){
            //printf("Mesma Diagonal linha:%d coluna:%d\n",linha,coluna);return 0;      // mesma diagonal
            return 0;
        } 
    }
    //printf("ok linha %d coluna %d\n",linha,coluna);
    return 1;
}

void bracktracking(int* q_solucoes, int** tabela_linhas, int n, int coluna, int altura){
    if( coluna == n ){
        *q_solucoes = *q_solucoes + 1;
        printf("%d\n",*q_solucoes);
        show(tabela_linhas[altura],n);
        return;
    }
    if( altura == n){
        return;
    }
    for (int j = 0; j < n; j++){    
        if( checar_2d(tabela_linhas,n,j,coluna,altura ) ){
            tabela_linhas[altura][coluna] = j;
            coluna++;
            bracktracking(q_solucoes,tabela_linhas, n,coluna,altura);
            coluna--;
            tabela_linhas[altura][coluna] = vazio;
        }else if( tabela_linhas[altura][coluna] != vazio){
            coluna++;
            bracktracking(q_solucoes,tabela_linhas, n,coluna,altura);
            return;
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
void zerar(int* m,int n){
    for (int i = 0; i < n; i++){
       m[i] = vazio;
    }
}

int main(){
    
    int n,p,f,r,c;
    int* q_solucoes = (int*)malloc(sizeof(int));
    *q_solucoes = 0;

    //scanf(" %d %d %d",&n,&p,&f);
    int** tabulheiro = alocar(5);/*
    for (int i = 0; i < p; i++){
        scanf(" %d %d",&r,&c);
        tabulheiro[f][c] = r;
    }*/
    tabulheiro[0][1] = 3;
    //printar(tabulheiro[0],5);
    //for (int i = 0; i < n; i++){
        bracktracking(q_solucoes,tabulheiro,5,0,0);
    //}
    printf("%d\n",*q_solucoes);
     
    return 0 ;
}