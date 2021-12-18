#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>
#include <GL/glut.h>
#include <string.h>
#include <math.h>

#include <stdbool.h> //para Ativar verdadeiro ou falso

GLfloat WIDTH = 600.0f; //LARGURA DA JANELA
GLfloat HEIGHT = 500.0f; //ALTURA DA JANELA

#define VELOCIDADE_BOLA 30 //VELOCIDADE DE DESLOCAMENTO DA BOLA
#define VELOCIDADE_JOGADORES 15 //VELOCIDADE DE MOVIMENTO DOS JOGADORES
#define NUM_RODADAS 3 //numero de rodadas de cada partida

GLint movebolax = 0; //movimento da bola
GLint movebolay = 0;
GLint direcao = 1;
GLint direcao_vertical= 1; //1 cima, -1 baixo
GLint move_jogador1Y = 0;
GLint move_jogador2Y = 0;
GLint paddle_boundary = 150; // limite da raquete

int janelaPlacar;
int janela;
static char label[200]; // sprintf

int pts_jogador1 = 0;//pontos do jogador1
int pts_jogador2 = 0;//pontos do jogador2

int movimento_jogador1 = 2; // 1 = cima; 2 = parado; 3 = baixo
int movimento_jogador2 = 2; // 1 = cima; 2 = parado; 3 = baixo

GLfloat angulo = 0.0f;

bool pause = true;

void init(void);
void display(void);
void bola(int passo);
void keyboard(unsigned char key, int x, int y); //capta as acoes no teclado
void KeySpecial(int key, int x, int y); // capta as acoes do teclado - teclas especiais
void DesenhaTexto(char *s);
void JanelaPlacar(void);
void moveJogador(int move_jogadorY, int movimento_jogador);
void jogador1(int passo);
void jogador2(int passo);
void displayAll(void);
int random(void);
void setPause(void);

// Função callback chamada quando o tamanho da janela é alterado 
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
     // Evita a divisao por zero
     if(h == 0) h = 1;
                           
     // Especifica as dimensões da Viewport
     glViewport(0, 0, w, h);

     // Inicializa o sistema de coordenadas
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();

     // Estabelece a janela de seleção (left, right, bottom, top)     
     if (w <= h)  {
		HEIGHT = 250.0f*h/w;
		WIDTH = 250.0f;
     }
     else  { 
		WIDTH = 250.0f*w/h;
		HEIGHT = 250.0f;
     }
     
     gluOrtho2D(0.0f, HEIGHT, 0.0f, WIDTH);
}
int main(int argc, char** argv){
	glutInit(&argc, argv);
	// glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (WIDTH, HEIGHT);
	glutInitWindowPosition (100, 100); 

	janela = glutCreateWindow("JOGO DE PING-PONG || CIENCIAS DA COMPUTACAO");
	glutIdleFunc(displayAll);
	init();
	glutDisplayFunc(display);
	
	
	glutTimerFunc(VELOCIDADE_JOGADORES,jogador1, 1);
	glutTimerFunc(VELOCIDADE_JOGADORES,jogador2, 1);

	glutTimerFunc(10,bola,1);
	
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(KeySpecial);
	/* Sub Janela - Janela Placar */
	janelaPlacar = glutCreateSubWindow(janela, 5, 5, WIDTH - 10, 85);
	glutDisplayFunc(JanelaPlacar);
	

	glutMainLoop();
	return 0;
}

void displayAll(void){
	glutSetWindow(janela);
	glutPostRedisplay();
	glutSetWindow(janelaPlacar);
	glutPostRedisplay();
}

void init(void){
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glOrtho (0, WIDTH, 0, HEIGHT, -1 ,1);
}

void display(void){
	glClear(GL_COLOR_BUFFER_BIT);

	// glPolygonMode(GL_BACK, GL_LINE);
	glColor3f(1.0, 1.0, 1.0);
	glLineWidth(15.0);

	/*MARGENS DO CAMPO*/
	glBegin(GL_LINE_LOOP);
		glVertex2i(2,2);  glVertex2i(599,2);
		glVertex2i(599,399); glVertex2i(2,399);
	glEnd();
	
	/*JOGADORES*/
	//JOGADOR-1
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
		glVertex2i(100,190+move_jogador1Y);  glVertex2i(100,240+move_jogador1Y); //barra do jogador 1
	glEnd();

	//JOGADOR-2
	glColor3f(0.0, 1.0, 0.0);
	glBegin(GL_LINES);
		glVertex2i(500,190+move_jogador2Y);  glVertex2i(500,240+move_jogador2Y); //barra do jogador 2
	glEnd();


	//BOLA
	glColor3f(1.0, 1.0, 0.0);
	glPointSize(15.0);
	glBegin(GL_POINTS);
		glVertex2i(movebolax+200,movebolay+210);
	glEnd();

	//  glFlush();
	glutSwapBuffers();
 }

void jogador1(int passo){
	
	moveJogador(move_jogador1Y,movimento_jogador1);

	glutPostRedisplay();
	glutTimerFunc(VELOCIDADE_JOGADORES,jogador1, 1);

}

void jogador2(int passo){
	
	moveJogador(move_jogador2Y,movimento_jogador2);

	glutPostRedisplay();
	glutTimerFunc(VELOCIDADE_JOGADORES,jogador2, 1);

}

void keyboard(unsigned char key, int x, int y){
	switch (key) {
		case 13:
			pause = false;
			if(pts_jogador1==NUM_RODADAS || pts_jogador2==NUM_RODADAS){
				pts_jogador1 = pts_jogador2 = 0;
			}
			direcao=1;
		
			break;
		case 27: //ESC-27 ENTER-13
			exit(0);
			break;
		case 'w':
		case 'W':
			if( move_jogador1Y < paddle_boundary )
				move_jogador1Y += VELOCIDADE_JOGADORES;//Muda a direcao para cima
				movimento_jogador1 = 1;
			glutPostRedisplay();
			break;
		case 's':
		case 'S':
			if( move_jogador1Y > -paddle_boundary )
				move_jogador1Y -= VELOCIDADE_JOGADORES;//Muda a direcao para cima
				movimento_jogador1 = 1;
			glutPostRedisplay();
			break;
		default:
			break;
	}
}

void KeySpecial(int key, int x, int y){
	switch(key){
		case 101:
			if( move_jogador2Y < paddle_boundary )
				move_jogador2Y += VELOCIDADE_JOGADORES;//Muda a direcao para cima
				movimento_jogador2 = 1;
			glutPostRedisplay();
			break;
		case 103:
			if( move_jogador2Y > -paddle_boundary )
				move_jogador2Y -= VELOCIDADE_JOGADORES;//Muda a direcao para cima
				movimento_jogador2 = 3;
			glutPostRedisplay();
			break;
	}
}

void moveJogador(int move_jogadorY, int movimento_jogador){
	
	switch(movimento_jogador){
		case 1: //cima
			move_jogadorY +=5;
			if(move_jogadorY+240 >= 399){ 
				movimento_jogador = 2;
			}
			break;
		case 2: //parado
			break;
		case 3:	// baixo
			move_jogadorY -=5;
			if(move_jogadorY+190 <= 5) {
				movimento_jogador = 2;
			}
			break;
	}

	if(move_jogadorY+240 >= 399) //verifica se vai sair do limite do campo para cima
		move_jogadorY=156;
	else if(move_jogadorY+190 <= 5) //verifica se vai sair do limite do campo para baixo
		move_jogadorY=-184;
	
}

void bola(int passo)
{

	if(!pause){
		if(direcao==1)
		{
			movebolax += 5;
			if(movebolax==290) {
			
				//verifica se jogador1 fez ponto
				if(movebolay+210 < move_jogador2Y+190 || movebolay+200 > move_jogador2Y+240){
					pts_jogador1++;
				}else{
					direcao = 0;
					angulo = random(); //houve colisao com jogador muda o angulo
				}
				

			}else{
				if(movebolax==390){
					setPause();
				}
			}
			
			movebolay += sin(angulo) * direcao_vertical * 5;

			//colisao nas paredes
			if(movebolay+210 >= 388 || movebolay+200 <=5){
				direcao_vertical *= -1;
			}

			

		}
		else
		{
			if(direcao==0){
				movebolax -= 5;
				if(movebolax==-90){ 
			
					if(movebolay+210 < move_jogador1Y+190 || movebolay+200 > move_jogador1Y+240){
						pts_jogador2++;
					}else{
						direcao = 1;
						angulo = random(); //houve colisao com jogador muda o angulo
					}
			
				}else{
					if(movebolax==-195){
						setPause();
					}
				}

				movebolay += sin(angulo) * direcao_vertical * 5;
				if(movebolay+210 >= 388 || movebolay+200 <=5){
					direcao_vertical *= -1;
				}

				
			}
		}
	}
	glutPostRedisplay();
	glutTimerFunc(VELOCIDADE_BOLA,bola, 1);
}

void DesenhaTexto(char *s){
	unsigned int i;
	for (i = 0; i < strlen (s); i++)
		glutBitmapCharacter (GLUT_BITMAP_HELVETICA_12, s[i]);
}

void DesenhaPtsPlacar(char *s){
	unsigned int i;
	for (i = 0; i < strlen (s); i++)
		glutBitmapCharacter (GLUT_BITMAP_TIMES_ROMAN_24, s[i]);
}

void JanelaPlacar(void){
	/* Limpa Sub Janela*/
	glutSetWindow(janelaPlacar);
	glClearColor(0.25, 0.25, 0.25, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	/* MARGENS DA JANELA */
	glColor3f(0.0F, 1.0F, 0.0F);
	glBegin (GL_LINE_LOOP);
		glVertex2f (-0.999F,  0.99F);
		glVertex2f (-0.999F, -0.99F);
		glVertex2f (0.999F, -0.99F);
		glVertex2f (0.999F,  0.99F);
	glEnd();


	/* Texto Estatico */
	glColor3f (1.0F, 1.0F, 1.0F);
	sprintf(label, "Desenvolvedor: IHURY FERREIRA ");
	glRasterPos2f (-0.98F, 0.55F);
	DesenhaTexto (label);

	//COMANDOS DO JOGO
	sprintf (label, "Tecle ESC - Sair");
	glRasterPos2f (-0.98F, 0.0F);
	DesenhaTexto (label);
	sprintf (label, "Tecle ENTER - Iniciar");
	glRasterPos2f (-0.98F, -0.55F);
	DesenhaTexto (label);


	//PLACAR
	sprintf (label, "PLACAR");
	glRasterPos2f (0.6F, 0.55F);
	DesenhaTexto (label);

	//PONTOS JOGADOR1
	glColor3f(1.0, 0.0, 0.0);
	sprintf (label,  "%i",pts_jogador1);
	glRasterPos2f (0.5F, -0.2F);
	DesenhaPtsPlacar (label);

	if(pts_jogador1==NUM_RODADAS){
		glColor3f(1.0, 1.0, 1.0);
		sprintf (label,  "Vencedor");
		glRasterPos2f (0.43F, -0.41F);
		DesenhaTexto (label);
	}

	//PONTOS JOGADOR2
	glColor3f(0.0, 1.0, 0.0);
	sprintf (label, "%i",pts_jogador2);
	glRasterPos2f (0.8F, -0.2F);
	DesenhaPtsPlacar (label);
	
	if(pts_jogador2==NUM_RODADAS){
		glColor3f(1.0, 1.0, 1.0);
		sprintf (label,  "Vencedor");
		glRasterPos2f (0.72F, -0.41F);
		DesenhaTexto (label);
	}

	//glFlush();
	glutSwapBuffers ();
}

int random(){
	return (rand() % 80)+1;
}

void setPause(){
	
	movebolax = movebolay = 0; // volta a bola para posicao inicial
	movimento_jogador1 = movimento_jogador2 = 2; //deixa inerte
	move_jogador1Y = move_jogador2Y = 0; // volta os jogadores para a posicao inicial
	pause = true;//flag pra saber se esta pausado
	direcao_vertical = 1; //aponta a direcao vertical para cima
	direcao = 2; //deixa a bolinha sem direcao, faz com que ela fica parada
	angulo = 0; //seta o angulo para quando a bolinha sair, ir reto pois ainda nao houve colisao
}
