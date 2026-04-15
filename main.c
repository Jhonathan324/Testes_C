#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHUNK_SIZE 16

typedef short Tile;

typedef struct {
    int x, y; // posição do chunk no mundo
    Tile tiles[CHUNK_SIZE][CHUNK_SIZE];
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

    fwrite(c->tiles, sizeof(Tile), CHUNK_SIZE * CHUNK_SIZE, f);
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
        fread(c->tiles, sizeof(Tile), CHUNK_SIZE * CHUNK_SIZE, f);
        fclose(f);
        printf("Chunk (%d,%d) carregado do disco\n", x, y);
    } else {
        printf("Chunk (%d,%d) gerado\n", x, y);
        gerar_chunk(c);
        salvar_chunk(c);
    }
}

// 🔹 Acessar tile global
Tile get_tile(int global_x, int global_y) {
    int chunk_x = global_x / CHUNK_SIZE;
    int chunk_y = global_y / CHUNK_SIZE;

    int local_x = global_x % CHUNK_SIZE;
    int local_y = global_y % CHUNK_SIZE;

    Chunk c;
    carregar_chunk(&c, chunk_x, chunk_y);

    for(int i = 0; i < CHUNK_SIZE; i++){
        for(int j = 0; j < CHUNK_SIZE; j++){
            printf("%2d ", c.tiles[i][j]);
        }
        printf("\n" );
    }


    return c.tiles[local_x][local_y];
}

// 🔹 Modificar tile
void set_tile(int global_x, int global_y, Tile valor) {
    int chunk_x = global_x / CHUNK_SIZE;
    int chunk_y = global_y / CHUNK_SIZE;

    int local_x = global_x % CHUNK_SIZE;
    int local_y = global_y % CHUNK_SIZE;

    Chunk c;
    carregar_chunk(&c, chunk_x, chunk_y);

    c.tiles[local_x][local_y] = valor;

    salvar_chunk(&c);
}

// 🔹 Teste
int main() {
    int x=-10, y=-10;
    set_tile(x, y, 99);
    printf("Tile[%d][%d]: %d\n", x, y, get_tile(x, y));

    x = 11; y = 11;
    set_tile(x, y, 42);
    printf("Tile[%d][%d]: %d\n", x, y, get_tile(x, y));
    set_tile(x, y, 0);

    return 0;
}
