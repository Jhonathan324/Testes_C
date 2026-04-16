#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <conio.h>

#define CHUNK_SIZE 16


typedef struct {
    int x, y; // posição do chunk no mundo
    uint8_t tiles[CHUNK_SIZE][CHUNK_SIZE];
} Chunk;

// 🔹 Geração procedural simples (substitui depois por Perlin)
void gerar_chunk(Chunk *c) {
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            c->tiles[i][j] = 0 ;
        }
    }
}

// 🔹 Nome do arquivo do chunk
void nome_arquivo(char *buffer, int x, int y) {
    sprintf(buffer, "chunk_%d_%d.bin", x, y);
}

// 🔹 Salvar chunk
void salvar_chunk(Chunk *c) {
    char nome[64];
    nome_arquivo(nome, c->x, c->y);

    FILE *f = fopen(nome, "wb");
    if (!f) return;

    fwrite(c->tiles, sizeof(uint8_t), CHUNK_SIZE * CHUNK_SIZE, f);
    fclose(f);
}

// 🔹 Carregar chunk (ou gerar se não existir)
void carregar_chunk(Chunk *c, int x, int y) {
    c->x = x;
    c->y = y;

    char nome[64];
    nome_arquivo(nome, x, y);

    FILE *f = fopen(nome, "rb");

    if (f) {
        fread(c->tiles, sizeof(uint8_t), CHUNK_SIZE * CHUNK_SIZE, f);
        fclose(f);
        printf("Chunk (%d,%d) carregado do disco\n", x, y);
    } else {
        printf("Chunk (%d,%d) gerado\n", x, y);
        gerar_chunk(c);
        salvar_chunk(c);
    }
}

// 🔹 Acessar tile global
uint8_t get_tile(int global_x, int global_y) {
    int local_x,local_y, chunk_x, chunk_y;
    if(global_x < 0){
        chunk_x = (global_x + 1) / CHUNK_SIZE - 1;
        local_x = CHUNK_SIZE * chunk_x * -1 + global_x;
    }
    else{
       chunk_x = global_x / CHUNK_SIZE;
       local_x = global_x % CHUNK_SIZE;
    }

    if(global_y < 0){
        chunk_y = (global_y + 1) / CHUNK_SIZE - 1;
        local_y = CHUNK_SIZE * chunk_y * -1 + global_y;
    }
    else{
       chunk_y = global_y / CHUNK_SIZE;
       local_y = global_y % CHUNK_SIZE;
    }

    Chunk c;
    carregar_chunk(&c, chunk_x, chunk_y);

    for(int i = 0; i < CHUNK_SIZE; i++){
        for(int j = 0; j < CHUNK_SIZE; j++){
            printf("%3d ", c.tiles[j][i]);
        }
        printf("\n" );
    }


    return c.tiles[local_x][local_y];
}

// 🔹 Modificar tile
void set_tile(int global_x, int global_y, uint8_t valor) {
    int local_x,local_y, chunk_x, chunk_y;
    if(global_x < 0){
        chunk_x = (global_x + 1) / CHUNK_SIZE - 1;
        local_x = CHUNK_SIZE * chunk_x * -1 + global_x;
    }
    else{
       chunk_x = global_x / CHUNK_SIZE;
       local_x = global_x % CHUNK_SIZE;
    }

    if(global_y < 0){
        chunk_y = (global_y + 1) / CHUNK_SIZE - 1;
        local_y = CHUNK_SIZE * chunk_y * -1 + global_y;
    }
    else{
       chunk_y = global_y / CHUNK_SIZE;
       local_y = global_y % CHUNK_SIZE;
    }
    printf("local_x: %d\nlocal_y: %d\n", local_x,local_y);
    Chunk c;
    carregar_chunk(&c, chunk_x, chunk_y);

    c.tiles[local_x][local_y] = valor;

    salvar_chunk(&c);
}

// 🔹 Teste
int main() {
    uint8_t valor = 0;
    int x=0, y=0;
    while(valor < 255){
        int c = _getch();
        valor++;
        if (c == 0 || c == 224) { // tecla especial
            int tecla = _getch();
            switch (tecla) {
                case 72:
                    y--;
                    set_tile(x, y, valor);
                    printf("Tile[%d][%d]: %d\n", x, y, get_tile(x, y));
                    break;
                case 80:
                    y++;
                    set_tile(x, y, valor);
                    printf("Tile[%d][%d]: %d\n", x, y, get_tile(x, y));
                    break;
                case 75:
                    x--;
                    set_tile(x, y, valor);
                    printf("Tile[%d][%d]: %d\n", x, y, get_tile(x, y));
                    break;
                case 77:
                    x++;
                    set_tile(x, y, valor);
                    printf("Tile[%d][%d]: %d\n", x, y, get_tile(x, y));
                    break;
            }
        }
        else if (c == 'p'){
            printf("teste terminado\n");
            break;
        }

    }


    return 0;
}
