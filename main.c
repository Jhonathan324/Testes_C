#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <conio.h>

#define CHUNK_SIZE 16
#define CHUNK_RANDER_X 3
#define CHUNK_RANDER_Y 3


typedef struct {
    int x, y;
    uint8_t tiles[CHUNK_SIZE][CHUNK_SIZE];
} Chunk;


void GerarChunk(Chunk *c) {
    for (int i = 0; i < CHUNK_SIZE; i++) {
        for (int j = 0; j < CHUNK_SIZE; j++) {
            c->tiles[i][j] = 0 ;
        }
    }
}


void NomeArquivo(char *buffer, int x, int y) {
    sprintf(buffer, "map/chunk_%d_%d.bin", x, y);
}


void GerarCordenada(int global,int *local, int *chunk){
    if(global < 0){
        *chunk = (global + 1) / CHUNK_SIZE - 1;
        *local = CHUNK_SIZE * *chunk * -1 + global;
    }
    else{
       *chunk = global / CHUNK_SIZE;
       *local = global % CHUNK_SIZE;
    }

    return;
}

// 🔹 Salvar chunk
void SalvarChunk(Chunk *c) {
    char nome[64];
    NomeArquivo(nome, c->x, c->y);

    FILE *f = fopen(nome, "wb");
    if (!f) return;

    fwrite(c->tiles, sizeof(uint8_t), CHUNK_SIZE * CHUNK_SIZE, f);
    fclose(f);
}


void CarregarChunk(Chunk *c, int x, int y) {
    c->x = x;
    c->y = y;

    char nome[64];
    NomeArquivo(nome, x, y);

    FILE *f = fopen(nome, "rb");

    if (f) {
        fread(c->tiles, sizeof(uint8_t), CHUNK_SIZE * CHUNK_SIZE, f);
        fclose(f);
        printf("Chunk (%d,%d) carregado do disco\n", x, y);
    } else {
        printf("Chunk (%d,%d) gerado\n", x, y);
        GerarChunk(c);
        SalvarChunk(c);
    }
}


int main() {
    uint8_t valor = 0;
    Chunk matriz_chunk[CHUNK_RANDER_Y][CHUNK_RANDER_X];
    int subx, suby,x=0, y=0, chunk_x = 0, chunk_y = 0, chunk_x_temp=0, chunk_y_temp=0, lixo;

    for(int i = 0; i < CHUNK_RANDER_Y; i ++) for (int j = 0; j < CHUNK_RANDER_X; j++){
        int Chunk_x,Chunk_y;
        GerarCordenada((x + (CHUNK_SIZE*(j-(CHUNK_RANDER_X/2)))), &lixo, &Chunk_x);
        GerarCordenada((y + (CHUNK_SIZE*(i-(CHUNK_RANDER_Y/2)))), &lixo, &Chunk_y);
        CarregarChunk(&matriz_chunk[i][j],Chunk_x,Chunk_y);
    }

    while(valor < 255){
        int c = _getch();
        printf("cordenada x: %d y: %d\n", x, y);
        printf("chunk x: %d y: %d\n", chunk_x,chunk_y);
        valor++;
        if (c == 0 || c == 224) { // tecla especial
            int tecla = _getch();
            switch (tecla) {
                case 72:
                    y--;
                    break;
                case 80:
                    y++;
                    break;
                case 75:
                    x--;
                    break;
                case 77:
                    x++;
                    break;
            }
        }
        else if (c == 'p'){
            printf("teste terminado\n");
            break;
        }
        GerarCordenada(x, &subx, &chunk_x_temp);
        GerarCordenada(y, &suby, &chunk_y_temp);


        if(chunk_x_temp != chunk_x || chunk_y_temp != chunk_y){
            chunk_x = chunk_x_temp;
            chunk_y = chunk_y_temp;
            for(int i = 0; i < CHUNK_RANDER_Y; i ++) for (int j = 0; j < CHUNK_RANDER_X; j++){
                int Chunk_x,Chunk_y,lixo;
                GerarCordenada((x + (CHUNK_SIZE*(j-CHUNK_RANDER_X/2))), &lixo, &Chunk_x);
                GerarCordenada((y + (CHUNK_SIZE*(i-CHUNK_RANDER_X/2))), &lixo, &Chunk_y);
                CarregarChunk(&matriz_chunk[i][j],Chunk_x,Chunk_y);
            }
        }
        matriz_chunk[CHUNK_RANDER_Y/2][CHUNK_RANDER_X/2].tiles[suby][subx] = valor;
        for(int i = 0; i < CHUNK_RANDER_Y; i ++) for (int j = 0; j < CHUNK_RANDER_X; j++) SalvarChunk(&matriz_chunk[i][j]);
        for(int i = 0; i < CHUNK_RANDER_Y * CHUNK_SIZE; i ++) {
            for (int j = 0; j < CHUNK_RANDER_X*CHUNK_SIZE; j++){
                printf("%3d ", matriz_chunk[i/CHUNK_SIZE][j/CHUNK_SIZE].
                               tiles[i%CHUNK_SIZE][j%CHUNK_SIZE]);
            }
            printf("\n");
        }

    }


    return 0;
}
