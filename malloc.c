#include<stdio.h>
#include<stddef.h>
static unsigned char memoria[1024*1024];

// malloc
struct bloco{
    size_t tamanho;
    int free;
    struct bloco *proximo;
};

struct bloco *livre = (void*) memoria;

void inicio(){
    livre->tamanho = (1024*1024) - sizeof(struct bloco);
    livre->free = 1;
    livre->proximo = NULL;
}

void dividir(struct bloco *atual, size_t tamanho){
    struct bloco *novo = (void*)((void*)atual+tamanho+sizeof(struct bloco));
    novo->tamanho = (atual->tamanho) - tamanho - sizeof(struct bloco);
    novo->free=1;
    novo->proximo = atual->proximo;
    atual->tamanho = tamanho;
    atual->free = 0;
    atual->proximo = novo;
}

void* aloca(size_t tamanho){
    struct bloco *atual, *ant;
    void *result;
    if(!(livre->tamanho)){
        inicio();
    }
    atual = livre;
    while((((atual->tamanho)<tamanho)||((atual->free)==0))&&(atual->proximo!=NULL)){
        ant = atual;
        atual=atual->proximo;
    }
    if((atual->tamanho) == tamanho){
        atual->free = 0;
        result = (void*)(++atual);
        return result;
    }else if((atual->tamanho) > (tamanho + sizeof(struct bloco))){
        dividir(atual, tamanho);
        result = (void*)(++atual);
        return result;
    }else{
        result = NULL;
        return result;
    }
    return NULL;
}

void print_memoria(){
    struct bloco *atual;
    atual = livre;
    printf("inicio\n");
    while(1==1){
        printf("Tamanaho: %lu\n", atual->tamanho);
        printf("Endereco: %d\n", atual);
        printf("livre: %d\n", atual->free);
        printf("proximo: %d\n", atual->proximo);
        printf("\n\n", atual->proximo);
        if(atual->proximo==NULL){
            break;
        }
        atual=atual->proximo;
        
    }
}

void liberar(void *liberar){
    struct bloco *atual;
    if(((void*)memoria<=liberar) && (liberar<=(void*)(memoria + (1024*1024)))){
        atual = liberar;
        --atual;
        atual->free=1;
        atual = livre;
        while(atual->proximo != NULL){
            if(atual->free){
                if(atual->proximo->free){
                    atual->tamanho += atual->proximo->tamanho + sizeof(struct bloco);
                    atual->proximo = atual->proximo->proximo;
                }else{
                    atual=atual->proximo;
                }
            }else{
                atual=atual->proximo;
            }
        }
    }else{
        printf("Invalido\n");
    }
}


// lista duplamente encadeada
struct lenc{
    int valor;
    struct lenc *proximo;
    struct lenc *anterior;
};

void* criar_lenc(int valor){
    struct lenc *novo = aloca(sizeof(struct lenc));
    novo->proximo = NULL;
    novo->anterior = NULL;
    novo->valor = valor;
    return novo;
}

void inserir_fim_lenc(struct lenc *atual, int valor){
    struct lenc *novo = aloca(sizeof(struct lenc));
    struct lenc *fim;
    fim = atual;
    while(fim->proximo != NULL){
        fim = fim->proximo;
    }
    fim->proximo = novo;
    novo->valor = valor;
    novo->anterior = fim;
}

void print_lenc(struct lenc *usado){
    struct lenc *atual = usado;
    while(1==1){
        printf("valor: %d\n", atual->valor);
        if(atual->proximo==NULL){
            break;
        }
        atual=atual->proximo;
        
    }
}

// main
int main(){
    struct lenc *lista_enc;
    print_memoria();
    int *teste = (int*) aloca(sizeof(int));
    print_memoria();
    int *teste2 = (int*) aloca(sizeof(int));
    print_memoria();
    *teste = 10;
    *teste2 = 15;
    printf("%d %d\n", *teste, *teste2);
    *teste = 1;
    printf("%d %d\n", *teste, *teste2);
    liberar(teste);
    liberar(teste2);
    printf("encadeada\n");
    print_memoria();
    lista_enc = criar_lenc(10);
    inserir_fim_lenc(lista_enc, 1);
    inserir_fim_lenc(lista_enc, 2);
    print_memoria();
    print_lenc(lista_enc);
    return 0;   
}
