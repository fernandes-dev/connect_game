#include <SDL.h>
#include <stdio.h>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <math.h>

#define ALTURA 6
#define LARGURA 6
#define QTDMAXIMO 42

//Screen dimension constants
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;
int matriz[LARGURA][ALTURA];
SDL_Event e;
bool clicou = false;

using namespace std;

bool loadMedia();
void close();
SDL_Window* gWindow = NULL;
SDL_Renderer *visualizacao;
SDL_Surface* campo = NULL;

typedef struct {
    SDL_Rect area; // cria um retângulo area.x, area.y, area.w (largurra), area.h (altura)
    SDL_Texture* textura=NULL; // textura da imagem
    SDL_Point centro;
} Objeto;

typedef struct{
    bool clicado = false;
    int mouseX = 0;
    int mouseY = 0;
    bool player1 = false;
    bool player2 = false;
    bool jogador = true;
} Mouse;

Objeto player1[LARGURA][ALTURA];
Objeto player2[LARGURA][ALTURA];
Objeto bolaVazia[LARGURA][ALTURA];
Objeto background;
Objeto player1WinsMsg;
Objeto player2WinsMsg;
Objeto indicador1;
Objeto indicador2;

Mouse mouse;

void carregaObjeto(Objeto *o, const char *caminho_da_imagem){
    o->area.w=60; //largura do objeto
    o->area.h=60; //altura do objeto
    o->area.x = 0;
    o->area.y = 0;
    SDL_Surface* imagem = SDL_LoadBMP(caminho_da_imagem); // le a imagem
    if( imagem == NULL ){
        printf( "Erro ao carregar imagem %s\n", SDL_GetError() );
    }else {
        o->textura = SDL_CreateTextureFromSurface(visualizacao, imagem); // cria a textura
        SDL_FreeSurface(imagem); // apaga a imagem da tela
    }
}

void carregaWinMsg()
{
    carregaObjeto(&player1WinsMsg,"imagens\\player1Wins.bmp");
    player1WinsMsg.area.x=1000;
    player1WinsMsg.area.y=1000;
    player1WinsMsg.area.w=300;
    player1WinsMsg.area.h=100;
    carregaObjeto(&player2WinsMsg,"imagens\\player2Wins.bmp");
    player2WinsMsg.area.x=1000;
    player2WinsMsg.area.y=1000;
    player2WinsMsg.area.w=300;
    player2WinsMsg.area.h=100;
}

void carregaBolaVazia()
{
    int x1 = 90;
    int y = 90;
    int limiteTela = 420; //define a quebra de linha para as bolinhas
    for (int i=0; i<LARGURA; i++) {
        for (int j=0; j<ALTURA; j++) {
            matriz[i][j] = 0; //matriz para verificar quem ganhou
            printf("%d ", matriz[i][j]);
            carregaObjeto(&bolaVazia[i][j],"imagens\\vazio.bmp");
            bolaVazia[i][j].area.x=x1;
            bolaVazia[i][j].area.y=y;
            x1+=60;
            if(x1 >= limiteTela) {
                y+=60;
                x1=90;
            }
        }
        printf("\n");
    }
}

void carregaBolaVerde()
{
    for (int i=0; i<LARGURA; i++) {
        for (int j=0; j<ALTURA; j++) {
            carregaObjeto(&player1[i][j],"imagens\\Player1.bmp");
            player1[i][j].area.x=1000;
            player1[i][j].area.y=1000;
        }
    }
}

void carregaBolaVermelha()
{
    for (int i=0; i<LARGURA; i++) {
        for (int j=0; j<ALTURA; j++) {
            carregaObjeto(&player2[i][j],"imagens\\Player2.bmp");
            player2[i][j].area.x=1000;
            player2[i][j].area.y=1000;
        }
    }
}

void carregaIndicador()
{
    carregaObjeto(&indicador1,"imagens\\vez1.bmp");
    indicador1.area.x=0;
    indicador1.area.y=0;
    indicador1.area.h=50;
    indicador1.area.w=300;
    carregaObjeto(&indicador2, "imagens\\vez2.bmp");
    indicador2.area.x=1000;
    indicador2.area.y=1000;
    indicador2.area.h=50;
    indicador2.area.w=300;
}

void carregaObjetos()
{
    //Carrega as imagens
    carregaObjeto(&background,"imagens\\campo.bmp");
    background.area.x=0;
    background.area.y=0;
    background.area.w=600;
    background.area.h=600;
    carregaBolaVerde();
    carregaBolaVermelha();
    carregaBolaVazia();
    carregaWinMsg();
    carregaIndicador();
}

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "Connect4", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
		    visualizacao = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		}
	}

	return success;
}

void desenhaBackgound()
{
    SDL_RenderCopy(visualizacao,background.textura,NULL, &background.area);
}

void desenhaWinsMsg()
{
    SDL_RenderCopy(visualizacao,player1WinsMsg.textura,NULL, &player1WinsMsg.area);
    SDL_RenderCopy(visualizacao,player2WinsMsg.textura,NULL, &player2WinsMsg.area);
}

void desenhaIndicadores()
{
    SDL_RenderCopy(visualizacao,indicador1.textura,NULL, &indicador1.area);
    SDL_RenderCopy(visualizacao,indicador2.textura,NULL, &indicador2.area);
}

void desenhaBolasVazias()
{
    for (int i=0; i<LARGURA; i++) {
        for (int j=0; j<ALTURA; j++) {
            SDL_RenderCopy(visualizacao,bolaVazia[i][j].textura,NULL, &bolaVazia[i][j].area);
        }
    }
}

void desenhaBolas1()
{
    for (int i=0; i<LARGURA; i++) {
        for (int j=0; j<ALTURA; j++) {
            SDL_RenderCopy(visualizacao,player1[i][j].textura,NULL, &player1[i][j].area);
        }
    }
}

void desenhaBolas2()
{
    for (int i=0; i<LARGURA; i++) {
        for (int j=0; j<ALTURA; j++) {
            SDL_RenderCopy(visualizacao,player2[i][j].textura,NULL, &player2[i][j].area);
        }
    }
}
bool gameOver = false;
checkWin(int larg, int alt, int value, int jogador)
{
    bool player1Wins = false, player2Wins = false;
    //system("cls");
    matriz[larg][alt] = value;
    for (int i=0; i<LARGURA; i++) {
        for (int j=0; j<ALTURA; j++) {
            printf("%d ", matriz[i][j]);
            if(matriz[j][i]==1 && matriz[j+1][i+1]==1 && matriz[j+2][i+2]==1 && matriz[j+3][i+3]==1)
            {
            player1Wins = true;
            }
            if(matriz[j][i]==1 && matriz[j+1][i-1]==1 && matriz[j+2][i-2]==1 && matriz[j+3][i-3]==1)
            {
            player1Wins = true;
            }
        if(matriz[j][i]==2 && matriz[j+1][i-1]==2 && matriz[j+2][i-2]==2 && matriz[j+3][i-3]==2)
            {
            player2Wins = true;
            }
        else if(matriz[j][i]==2 && matriz[j-1][i-1]==2 && matriz[j-2][i-2]==2 && matriz[j-3][i-3]==2)
            {
            player2Wins = true;
            }
        else if(matriz[j][i]==1 && matriz[j][i-1]==1 && matriz[j][i-2]==1 && matriz[j][i-3]==1)
            {
            player1Wins = true;
            }
        else if(matriz[j][i]==1 && matriz[j-1][i]==1 && matriz[j-2][i]==1 && matriz[j-3][i]==1)
            {
            player1Wins = true;
            }
        else if(matriz[j][i]==2 && matriz[j][i-1]==2 && matriz[j][i-2]==2 && matriz[j][i-3]==2)
            {
            player2Wins = true;
            }
        else if(matriz[j][i]==2 && matriz[j-1][i]==2 && matriz[j-2][i]==2 && matriz[j-3][i]==2)
            {
            player2Wins = true;
            }
        }
        printf("\n");
    }
    if (player1Wins == true) {
        printf("Player 1 Wins\n");
        player1WinsMsg.area.x=150;
        player1WinsMsg.area.y=0;
        gameOver = true;
    } else if (player2Wins == true) {
        printf("Player 2 Wins\n");
        player2WinsMsg.area.x=150;
        player2WinsMsg.area.y=0;
        gameOver = true;
    }
}

int distX = 0; //flag para verificar o vencedor
void destroiBolinha()
{
    int tamanhoX[QTDMAXIMO];
    int tamanhoY[QTDMAXIMO];
    if (mouse.clicado == true) {
        printf("Clicou\n");
        int mouseX = e.motion.x;
        int mouseY = e.motion.y;
        for (int i=0; i<LARGURA;i++) {
            for (int j=0; j<ALTURA; j++) {
                tamanhoX[i] = bolaVazia[i][j].area.x + 60;
                tamanhoY[i] = bolaVazia[i][j].area.y + 60;
                //system("cls");
                if (mouseY >= bolaVazia[i][j].area.y && mouseY <= tamanhoY[i] && mouseX >= bolaVazia[i][j].area.x && mouseX <= tamanhoX[i]) {
                    printf("Clicou na bolinha %d, %d\n", bolaVazia[i][j].area.x, bolaVazia[i][j].area.y);
                    if (mouse.player1 == true) {
                        indicador2.area.x=0;
                        indicador2.area.y=0;
                        indicador1.area.x=1000;
                        indicador1.area.y=1000;
                        SDL_RenderCopy(visualizacao,player1[i][j].textura,NULL, &player1[i][j].area);
                        player1[i][j].area.x=bolaVazia[i][j].area.x;
                        player1[i][j].area.y=bolaVazia[i][j].area.y;
                        SDL_DestroyTexture(bolaVazia[i][j].textura);
                        bolaVazia[i][j].area.x = 10000;
                        printf("Bola verde %d, %d\n", player1[i][j].area.x, player1[i][j].area.y);
                        distX += player1[i][j].area.x;
                        checkWin(i, j, 1, 1);
                    } else if (mouse.player2 == true) {
                        indicador1.area.x=0;
                        indicador1.area.y=0;
                        indicador2.area.x=1000;
                        indicador2.area.y=1000;
                        SDL_RenderCopy(visualizacao,player2[i][j].textura,NULL, &player2[i][j].area);
                        player2[i][j].area.x=bolaVazia[i][j].area.x;
                        player2[i][j].area.y=bolaVazia[i][j].area.y;
                        printf("Bola vermelha %d, %d\n", player2[i][j].area.x, player2[i][j].area.y);
                        SDL_DestroyTexture(bolaVazia[i][j].textura);
                        bolaVazia[i][j].area.x = 10000;
                        checkWin(i, j, 2, 2);
                    }
                }
            }
        }
    }
}

void display()
{
    SDL_RenderClear(visualizacao); //limpa a tela
    desenhaBackgound(); //desenha o fundo
    desenhaBolasVazias(); // desenha o campo vazio
    desenhaBolas1();
    desenhaBolas2();
    desenhaWinsMsg();
    desenhaIndicadores();
    SDL_RenderPresent(visualizacao);
}

void close()
{
	//Deallocate surface
	SDL_FreeSurface( campo );
	campo = NULL;
	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;
    //IMG_Quit();
    SDL_DestroyRenderer(visualizacao);
    for (int i=0;i<LARGURA;i++) {
        for (int j=0; j<ALTURA; j++) {
            SDL_DestroyTexture(bolaVazia[i][j].textura);
            SDL_DestroyTexture(player1[i][j].textura);
            SDL_DestroyTexture(player2[i][j].textura);
        }
    }
    SDL_DestroyTexture(background.textura);
	//Quit SDL subsystems
	SDL_Quit();
}

int main( int argc, char* args[] )
{
    bool podeJogar1 = true; //jogador 1 começa
    bool podeJogar2 = false;
	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
        //carregar os objetos
        carregaObjetos();
        //Main loop flag
        bool quit = false;
        //While application is running
        while( !quit )
        {
            SDL_WaitEvent(&e);
                switch(e.type){
                    case SDL_QUIT:
                        quit = false;
                        break;
                    case SDL_MOUSEBUTTONDOWN:
                        if (gameOver == false) {
                            mouse.clicado = true;
                            if (mouse.clicado == true) {
                                switch (e.button.button)
                                {
                                case SDL_BUTTON_LEFT:
                                    if (podeJogar1 == true) {
                                        mouse.player2 = false;
                                        printf("jogador 1 clicou\n");
                                        mouse.player1 = true;
                                        podeJogar1 =  false;
                                    } else {
                                        mouse.player1 = false;
                                    }
                                    break;
                                case SDL_BUTTON_RIGHT:
                                    if (podeJogar2 == true) {
                                        mouse.player1 = false;
                                        printf("jogador 2 clicou\n");
                                        mouse.player2 = true;
                                        podeJogar2 = false;
                                    } else if (podeJogar2 == false) {
                                        mouse.player2 = false;
                                        podeJogar1 = true;
                                    }
                                    break;
                                }
                            }
                        }else {
                            mouse.clicado = false;
                            close();
                            return 0;
                        }
                        break;
                        case SDL_MOUSEBUTTONUP:
                        if (gameOver == false) {
                            mouse.clicado = false;
                            if (mouse.clicado == false) {
                            switch (e.button.button)
                            {
                            case SDL_BUTTON_LEFT:
                                podeJogar1 = false;
                                podeJogar2 = true;
                                break;
                            case SDL_BUTTON_RIGHT:
                                podeJogar1 = true;
                                podeJogar2 = false;
                                break;
                            }
                            //mouse.clicado = false;
                        }
                        } else {
                            mouse.clicado = false;
                            indicador1.area.x=1000;
                            indicador1.area.y=1000;
                            indicador2.area.x=1000;
                            indicador2.area.y=1000;
                        }
                        break;
                    case SDL_MOUSEMOTION: //captura a posição x e y do mouse
                        int mouseX = e.motion.x;
                        int mouseY = e.motion.y;
                        std::stringstream ss;
                        ss << "X: " << mouseX << " Y: " << mouseY;
                        SDL_SetWindowTitle(gWindow, ss.str().c_str());
                        break;
                }
            destroiBolinha();
            display();
        }
    }
	close();
	return 0;
}
