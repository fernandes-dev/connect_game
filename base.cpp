#include "SDL2/SDL.h"
#include "SDL2/SDL_opengl.h"
#include "SDL2/SDL_image.h"
#include "SDL2/SDL_mixer.h"
#include <stdio.h>
#define LARGURA 1200
#define ALTURA 600
#define INIMIGO 20
#include <math.h>
#include <stdlib.h>
#include <locale.h>
#include <time.h>
#define PONT 23
#define NAV 4

SDL_Window *janelaprincipal = NULL; // nome da janela principal do jogo
SDL_Surface *content = NULL; // área de trabalho da janela

SDL_Renderer *visualizacao;

//usando para regenciar as teclas de interação com o jogo
typedef struct{
    bool esquerda;
    bool direita;
    bool espaco;
    bool cima;
    bool baixo;
} Teclas;

Teclas teclas;
void inicializaTeclas(){
    teclas.esquerda=false;
    teclas.direita=false;
    teclas.espaco=false;
    teclas.cima=false;
    teclas.baixo=false;
}

typedef struct {
    SDL_Rect area; // cria um retângulo area.x, area.y, area.w (largurra), area.h (altura)
    float velocidade; // velocidade que o objeto se move
    SDL_Texture* textura=NULL; // textura da imagem
    SDL_Point centro;
    int raio = 32;
} Objeto;

Objeto nave[NAV];
Objeto background;
Objeto tiro;
Objeto inimigo[INIMIGO];
Objeto chefao;
Objeto barra;
Objeto gameOver;
Objeto telaStart;
Objeto how;
Objeto um[PONT];

bool telaFim;
bool telaInicio;
bool tironatela;
bool telaHow;
int pontos;
bool pontos23;

//diretivas do jogo
bool fim=false; // encerra o jogo com true

void carregaObjeto(Objeto *o, const char *caminho_da_imagem){
    o->area.w=64; //largura do objeto
    o->area.h=64; //altura do objeto
    o->area.x = 0;
    o->area.y = 0;
    SDL_Surface* imagem = IMG_Load(caminho_da_imagem); // le a imagem
    if( imagem == NULL ){
        printf( "Erro ao carregar imagem %s\n", SDL_GetError() );
    }else {
        o->textura = SDL_CreateTextureFromSurface(visualizacao, imagem); // cria a textura
        SDL_FreeSurface(imagem); // apaga a imagem da tela
    }
}

void telaInicial() {
    carregaObjeto(&telaStart,"imagens\\telaStart.png");
    telaStart.area.x=0;
    telaStart.area.y=0;
    telaStart.area.w=LARGURA;
    telaStart.area.h=ALTURA;
}

void carregaHow() {
    carregaObjeto(&how,"imagens\\how.png");
    how.area.x=0;
    how.area.y=0;
    how.area.w=LARGURA;
    how.area.h=ALTURA;
}

void carregaObjetos()
{
    //Carrega as imagens
    carregaObjeto(&background,"imagens\\background.png");
    background.area.x=0;
    background.area.y=0;
    background.area.w=LARGURA;
    background.area.h=ALTURA;
    carregaObjeto(&nave[0],"imagens\\minhanave.png");
    carregaObjeto(&nave[1],"imagens\\minhanave2.png");
    carregaObjeto(&nave[2],"imagens\\minhanave3.png");
    carregaObjeto(&nave[3],"imagens\\minhanave4.png");
    for (int i=0; i <NAV; i++) {
    nave[i].area.x=20; //posicao x do meio da janela
    nave[i].area.y=ALTURA - nave[i].area.h - 80; // posiciona no fim da tela
    nave[i].velocidade = 6.0f;
    nave[i].area.w=128;
    nave[i].area.h=128;
    }
    carregaObjeto(&tiro,"imagens\\tiro.png");
    tiro.area.w=60;
    tiro.area.h=10;
    srand(time(NULL));
    tiro.velocidade=10.0f;
    for (int i = 0; i < INIMIGO; i++) {
    carregaObjeto(&inimigo[i],"imagens\\tuku.png");
    inimigo[i].area.x= 1200; //posicao x do meio da janela
    inimigo[i].area.y=40+rand()%400; // posiciona no fim da tela
    inimigo[i].velocidade= 2.1f;
    inimigo[i].area.w=128;
    inimigo[i].area.h=128;
    }
    carregaObjeto(&chefao,"imagens\\chefao.png");
    chefao.area.x= 1200; //posicao x do meio da janela
    chefao.area.y=40+rand()%450; // posiciona no fim da tela
    chefao.area.w=200;
    chefao.area.h=200;
    chefao.velocidade= 2.1f;
    carregaObjeto(&gameOver, "imagens\\gameOver.png");
    gameOver.area.x=0;
    gameOver.area.y=0;
    gameOver.area.w=LARGURA;
    gameOver.area.h=ALTURA;
    carregaObjeto(&barra, "imagens\\barra.png");
    barra.area.x = 300;
    barra.area.y = 490;
    barra.area.w = 256;
    barra.area.h = 20;
    carregaObjeto(&um[0],"imagens\\um.png");
    carregaObjeto(&um[1],"imagens\\dois.png");
    carregaObjeto(&um[2],"imagens\\tres.png");
    carregaObjeto(&um[3],"imagens\\quatro.png");
    carregaObjeto(&um[4],"imagens\\cinco.png");
    carregaObjeto(&um[5],"imagens\\seis.png");
    carregaObjeto(&um[6],"imagens\\sete.png");
    carregaObjeto(&um[7],"imagens\\oito.png");
    carregaObjeto(&um[8],"imagens\\nove.png");
    carregaObjeto(&um[9],"imagens\\dez.png");
    carregaObjeto(&um[10],"imagens\\onze.png");
    carregaObjeto(&um[11],"imagens\\doze.png");
    carregaObjeto(&um[12],"imagens\\treze.png");
    carregaObjeto(&um[13],"imagens\\quatorze.png");
    carregaObjeto(&um[14],"imagens\\quinze.png");
    carregaObjeto(&um[15],"imagens\\dezesseis.png");
    carregaObjeto(&um[16],"imagens\\dezessete.png");
    carregaObjeto(&um[17],"imagens\\dezoito.png");
    carregaObjeto(&um[18],"imagens\\dezenove.png");
    carregaObjeto(&um[19],"imagens\\vinte.png");
    carregaObjeto(&um[20],"imagens\\vinteum.png");
    carregaObjeto(&um[21],"imagens\\vintedois.png");
    carregaObjeto(&um[22],"imagens\\vintetres.png");
    for (int i = 0; i < PONT; i++) {
    um[i].area.x=85;
    um[i].area.y=35;
    um[i].area.w=70;
    um[i].area.h=16;
    }
}

int posicaoAtual = nave[0].area.y+430;
int posicaoBarra = barra.area.x+430;

bool init()
{
    bool success = true;

    //Inicializa a SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Erro ao carregar a SDL: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Cria a janela principal
        janelaprincipal = SDL_CreateWindow( "GAME", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   LARGURA, ALTURA, SDL_WINDOW_SHOWN );
        if( janelaprincipal == NULL )
        {
            printf( "Erro na criação da janela: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            visualizacao = SDL_CreateRenderer(janelaprincipal, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        }
    }
    return success;

}

void som(){

    int frequencia = 22050;
    Uint16 formato  =   AUDIO_S16SYS;
    int canal = 1;
    int buffer = 4096;

    Mix_OpenAudio(frequencia,formato,canal,buffer);

    Mix_Chunk *son;

    son = Mix_LoadWAV("sirene1.wav");

    Mix_PlayChannel(-1,son,-1);

    Mix_FreeChunk(son);


}

void desenhaNave(){

    for (int i=0;i<NAV;i++){
       if (nave[i].area.y < posicaoAtual) {
        nave[i].area.y++;
        nave[i].area.y++;//faz ele voltar ao chão para efeito de pulo
        nave[i].area.y++;
        nave[i].area.y++;
        }
    }
    if(teclas.direita) {
        SDL_RenderCopy(visualizacao,nave[1].textura,NULL, &nave[1].area);
        }
    else if(teclas.cima) {
        SDL_RenderCopy(visualizacao,nave[2].textura,NULL, &nave[2].area);
        }
    else if(teclas.esquerda) {
        SDL_RenderCopy(visualizacao,nave[3].textura,NULL, &nave[3].area);
        }
    else if (tironatela) {
        SDL_RenderCopy(visualizacao,nave[3].textura,NULL, &nave[3].area);
        for (int i=0;i<NAV;i++) {
            if (nave[i].area.x > 0) {
            nave[i].area.x--;
            }
        }
    }
    else {
        SDL_RenderCopy(visualizacao,nave[0].textura,NULL, &nave[0].area);
    }
}

void desenhaInicio() {
    SDL_RenderCopy(visualizacao,telaStart.textura,NULL, &telaStart.area);
}

void desenhaPlacar() {
    if (pontos == 1){
        SDL_RenderCopy(visualizacao,um[0].textura,NULL, &um[0].area);
    }
    if (pontos == 2){
        SDL_RenderCopy(visualizacao,um[1].textura,NULL, &um[1].area);
    }
    if (pontos == 3){
        SDL_RenderCopy(visualizacao,um[2].textura,NULL, &um[2].area);
    }
    if (pontos == 4){
        SDL_RenderCopy(visualizacao,um[3].textura,NULL, &um[3].area);
    }
    if (pontos == 5){
        SDL_RenderCopy(visualizacao,um[4].textura,NULL, &um[4].area);
    }
    if (pontos == 6){
        SDL_RenderCopy(visualizacao,um[5].textura,NULL, &um[5].area);
    }
    if (pontos == 7){
        SDL_RenderCopy(visualizacao,um[6].textura,NULL, &um[6].area);
    }
    if (pontos == 8){
        SDL_RenderCopy(visualizacao,um[7].textura,NULL, &um[7].area);
    }
    if (pontos == 9){
        SDL_RenderCopy(visualizacao,um[8].textura,NULL, &um[8].area);
    }
    if (pontos == 10){
        SDL_RenderCopy(visualizacao,um[9].textura,NULL, &um[9].area);
    }
    if (pontos == 11){
        SDL_RenderCopy(visualizacao,um[10].textura,NULL, &um[10].area);
    }
    if (pontos == 12){
        SDL_RenderCopy(visualizacao,um[11].textura,NULL, &um[1].area);
    }
    if (pontos == 13){
        SDL_RenderCopy(visualizacao,um[12].textura,NULL, &um[12].area);
    }
    if (pontos == 14){
        SDL_RenderCopy(visualizacao,um[13].textura,NULL, &um[13].area);
    }
    if (pontos == 15){
        SDL_RenderCopy(visualizacao,um[14].textura,NULL, &um[14].area);
    }
    if (pontos == 16){
        SDL_RenderCopy(visualizacao,um[15].textura,NULL, &um[15].area);
    }
    if (pontos == 17){
        SDL_RenderCopy(visualizacao,um[16].textura,NULL, &um[16].area);
    }
    if (pontos == 18){
        SDL_RenderCopy(visualizacao,um[17].textura,NULL, &um[17].area);
    }
    if (pontos == 19){
        SDL_RenderCopy(visualizacao,um[18].textura,NULL, &um[18].area);
    }
    if (pontos == 20){
        SDL_RenderCopy(visualizacao,um[19].textura,NULL, &um[19].area);
    }
    if (pontos == 21){
        SDL_RenderCopy(visualizacao,um[20].textura,NULL, &um[20].area);
    }
    if (pontos == 22){
        SDL_RenderCopy(visualizacao,um[22].textura,NULL, &um[22].area);
    }
    if (pontos == 23){
        SDL_RenderCopy(visualizacao,um[23].textura,NULL, &um[23].area);
    }
}

void desenhaHow() {
    SDL_RenderCopy(visualizacao,how.textura,NULL, &how.area);
}

void desenhaBackgound(){
    SDL_RenderCopy(visualizacao,background.textura,NULL, &background.area);
}

void desenhaTiro() {
    SDL_RenderCopy(visualizacao,tiro.textura,NULL, &tiro.area);
    tiro.area.x+=tiro.velocidade; // movimenta o tiro
    if(tiro.area.x>1150) { // ate sair da tela
        tironatela=false;
        tiro.area.x= 30000;
    }
}

void desenhaFim() {
    for (int i=0;i<PONT;i++) {
    um[i].area.x= 530;
    um[i].area.y= 280;
    um[i].area.w= 150;
    um[i].area.h= 30;
    }
    SDL_RenderCopy(visualizacao,gameOver.textura,NULL, &gameOver.area);
}

/*void desenhaBarra() {
    SDL_RenderCopy(visualizacao,barra.textura,NULL, &barra.area);
}*/

void desenhaChefao() {
    chefao.area.x += chefao.velocidade;
    if (chefao.area.x < -300) {
        chefao.velocidade =+rand()%12;
        chefao.area.y=50+rand()%400;
    }
    else if (chefao.area.x+chefao.area.w > 1500) {
        chefao.velocidade =-rand()%12;
        chefao.area.y=50+rand()%400;
    }
    SDL_RenderCopy(visualizacao,chefao.textura,NULL, &chefao.area);
}

void desenhaInimigo() {
    for (int i = 0; i < INIMIGO; i++) {
        inimigo[i].area.x += inimigo[i].velocidade;
    if (inimigo[i].area.x < -500) {
        inimigo[i].area.x=1400;
    }
    else if (inimigo[i].area.x+inimigo[i].area.w > 2000) {
        inimigo[i].velocidade =-rand()%8;
    }
    }
    for (int a = 0; a < INIMIGO; a++) {
        SDL_RenderCopy(visualizacao,inimigo[a].textura,NULL, &inimigo[a].area);
    }
}

void close()
{
    SDL_FreeSurface( content );
    content = NULL;
    SDL_DestroyWindow( janelaprincipal );
    janelaprincipal= NULL;
    IMG_Quit();
    SDL_DestroyRenderer(visualizacao);
    for (int i=0;i<NAV;i++) {
    SDL_DestroyTexture(nave[i].textura);
    }
    SDL_DestroyTexture(background.textura);
    SDL_DestroyTexture(tiro.textura);
    for (int i=0;i<PONT;i++) {
    SDL_DestroyTexture(um[i].textura);
    }
    for (int i = 0; i < INIMIGO; i++) {
    SDL_DestroyTexture(inimigo[i].textura);
    }
    SDL_Quit(); // fecha a SDL
}

void display(){
    SDL_RenderClear(visualizacao); //limpa a tela
    desenhaBackgound(); // desenha o fundo
    //desenhaBarra();
    desenhaNave(); // mostra os objetos
    if(tironatela)
    desenhaTiro();; // animação do tiro
    if (telaFim)
    desenhaFim();;
    desenhaInimigo();
    if (pontos==20)
    desenhaChefao();;
    if (telaInicio)
    desenhaInicio();;
    if (telaHow)
    desenhaHow();;
    if (pontos23)
    desenhaPlacar();;
    SDL_RenderPresent(visualizacao);
}

void destroyinimigo(){
    int distcentro;
    for (int i = 0; i < INIMIGO; i++) {
    inimigo[i].centro.x=inimigo[i].area.x+64;
    inimigo[i].centro.y=inimigo[i].area.y+64;
    tiro.centro.x=tiro.area.x+58;
    tiro.centro.y=tiro.area.y+10;
    distcentro=sqrt(((inimigo[i].centro.x-tiro.centro.x)*(inimigo[i].centro.x-tiro.centro.x))+((inimigo[i].centro.y-tiro.centro.y)*(inimigo[i].centro.y-tiro.centro.y)));
    if(distcentro<50){
        SDL_DestroyTexture(inimigo[i].textura);
        pontos++;
        pontos23 = true;
        inimigo[i].area.x = -20000;
        inimigo[i].area.y = -20000;
        tironatela=false;
        tiro.area.x = 3000;
        tiro.area.y = 3000;
        }
    }
}

void destroyChefao(){
    int distcentro;
    for (int i = 0; i < INIMIGO; i++) {
    chefao.centro.x=chefao.area.x+50;
    chefao.centro.y=chefao.area.y+50;
    tiro.centro.x=tiro.area.x+16;
    tiro.centro.y=tiro.area.y+32;
    distcentro=sqrt(((chefao.centro.x-tiro.centro.x)*(chefao.centro.x-tiro.centro.x))+((chefao.centro.y-tiro.centro.y)*(chefao.centro.y-tiro.centro.y)));
    if(distcentro<50){
        pontos+=3;
        tironatela=false;
        tiro.area.x = 3000;
        tiro.area.y = 3000;
            }
    if (pontos == 23) {
            telaFim = true;
            SDL_DestroyTexture(inimigo[i].textura);
            SDL_DestroyTexture(chefao.textura);
            chefao.area.x = -20000;
            chefao.area.y = -20000;
            for (int i=0;i>NAV;i++) {
               SDL_DestroyTexture(nave[i].textura);
            }
            pontos23 = true;
                }
        }
    }

void personagemMorre() {
    int distcentro;
    for (int i = 0; i < INIMIGO; i++) {
    for (int j = 0; j < NAV; j++) {
    inimigo[i].centro.x=inimigo[i].area.x+64;
    inimigo[i].centro.y=inimigo[i].area.y+64;
    nave[j].centro.x=nave[j].area.x+32;
    nave[j].centro.y=nave[j].area.y+32;
    distcentro=sqrt(((inimigo[i].centro.x-nave[j].centro.x)*(inimigo[i].centro.x-nave[j].centro.x))+((inimigo[i].centro.y-nave[j].centro.y)*(inimigo[i].centro.y-nave[j].centro.y)));
    if(distcentro<50){
        SDL_DestroyTexture(nave[j].textura);
        nave[j].area.y = 10000.0f;
        teclas.espaco = false;
        telaFim = true;
        pontos23 = true;
        for (int i = 0; i < INIMIGO; i++){
           SDL_DestroyTexture(inimigo[i].textura);
           SDL_DestroyTexture(chefao.textura);
                }
            }
        }
    }
    for (int i=0;i<NAV;i++) {
    chefao.centro.x=chefao.area.x+50;
    chefao.centro.y=chefao.area.y+50;
    nave[i].centro.x=nave[i].area.x+50;
    nave[i].centro.y=nave[i].area.y+50;
    distcentro=sqrt(((chefao.centro.x-nave[i].centro.x)*(chefao.centro.x-nave[i].centro.x))+((chefao.centro.y-nave[i].centro.y)*(chefao.centro.y-nave[i].centro.y)));
    if(distcentro<50){
        SDL_DestroyTexture(nave[i].textura);
        nave[i].area.y = 10000.0f;
        teclas.espaco = false;
        telaFim = true;
        for (int i = 0; i < INIMIGO; i++){
           SDL_DestroyTexture(inimigo[i].textura);
           SDL_DestroyTexture(chefao.textura);
        }
    }
    if (nave[i].area.y <= 0) {
        SDL_DestroyTexture(nave[i].textura);
        nave[i].area.y = 10000.0f;
        teclas.espaco = false;
        telaFim = true;
        for (int i = 0; i < INIMIGO; i++){
           SDL_DestroyTexture(inimigo[i].textura);
           SDL_DestroyTexture(chefao.textura);
        }
    }
  }
}

//função pra subir na plataforma
/*void subir() {
    int larg, alt; //largura e altura da plataforma
    larg=256+barra.area.x;
    alt=10+barra.area.y;
    for (int i=0;i<NAV;i++) {
    int altNave = nave[i].area.y+64;
    //se a area da nave estiver contida na area da plataforma o personagem
    //sobe nela
    if (nave[i].area.x < larg && nave[i].area.x > 256 && altNave < alt) {
                posicaoAtual = posicaoBarra;
            }
            else {
                posicaoAtual = 530;
            }
    }
}*/


void executaAcao() {
    for (int i=0;i<NAV;i++) {
    if(teclas.direita && nave[i].area.x < 1127)
        nave[i].area.x+=nave[i].velocidade;
    if(teclas.esquerda && nave[i].area.x > 0)
        nave[i].area.x-=nave[i].velocidade;
    if(teclas.cima && nave[i].area.y > 0) {
        nave[i].area.y-=nave[i].velocidade*2.5;
    }
    if(teclas.cima && nave[i].area.y < 600) {
        nave[i].area.y+=nave[i].velocidade;
    }
    if(teclas.baixo && nave[i].area.y < posicaoAtual)
        nave[i].area.y+=nave[i].velocidade;
    if(teclas.espaco){
        tironatela=true;
        teclas.espaco=false; // apenas um tiro de cada vez
        tiro.area.x=nave[i].area.x+80;
        tiro.area.y=nave[i].area.y+70;
        tiro.velocidade=10;
    }
}
    if (fim) {
        printf("\nPONTUAÇÃO FINAL %d ", pontos);
    }
    destroyinimigo();
    destroyChefao();
    personagemMorre();
    //subir();
}

int main( int argc, char* args[] ) {
    setlocale(LC_ALL, "Portuguese");
    inicializaTeclas();
    tironatela=false;
    telaFim = false;
    telaInicio = true;
    telaHow = false;
    pontos23 = true;
    //Eventos
    SDL_Event evento;

    //Inicializa a SDL
    if( !init() )
    {
        printf( "Falha na inicialização!\n" );
    }
    else
    {
        telaInicial();
        carregaHow();
        carregaObjetos();
        while(!fim){
            while( SDL_PollEvent( &evento ))
            {
                switch(evento.type){
                    case SDL_QUIT :
                        fim = true;
                        break;
                    case SDL_KEYDOWN: // tecla pressionada
                        if (evento.key.keysym.sym == SDLK_ESCAPE)
                            fim = true;
                        if (evento.key.keysym.sym == SDLK_F10) {
                            telaHow = false;
                            SDL_DestroyTexture(telaStart.textura);
                            SDL_DestroyTexture(how.textura);
                            carregaObjetos();
                        }
                        if (evento.key.keysym.sym == SDLK_F9) {
                            SDL_DestroyTexture(telaStart.textura);
                            carregaObjetos();
                            telaHow = true;
                        }
                        if(evento.key.keysym.sym == SDLK_LEFT)
                            teclas.esquerda=true;
                        if (evento.key.keysym.sym == SDLK_RIGHT)
                            teclas.direita=true;
                        if (evento.key.keysym.sym == SDLK_UP)
                            teclas.cima=true;
                        /*if (evento.key.keysym.sym == SDLK_DOWN)
                            teclas.baixo=true;*/
                        if (evento.key.keysym.sym == SDLK_SPACE)
                            if(!tironatela)
                                teclas.espaco=true;
                        break;
                    case SDL_KEYUP: // tecla solta
                        if(evento.key.keysym.sym == SDLK_LEFT)
                            teclas.esquerda=false;
                        if (evento.key.keysym.sym == SDLK_RIGHT)
                            teclas.direita=false;
                        if (evento.key.keysym.sym == SDLK_UP)
                            teclas.cima=false;
                        if (evento.key.keysym.sym == SDLK_DOWN)
                            teclas.baixo=false;
                        if (evento.key.keysym.sym == SDLK_SPACE)
                            teclas.espaco=false;
                        break;
                }
            }
            executaAcao();
            display();
        }
    }
    close();
    Mix_CloseAudio();
    return 0;
}
