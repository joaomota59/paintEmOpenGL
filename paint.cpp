//*********************************************************************
//  Disciplina: Computação Gráfica
//  Aluno: JOÃO LUCAS SILVA MOTA
//	Professor: LAURINDO DE SOUSA BRITTO NETO
//*********************************************************************
//PEQUENO TUTORIAL
//PRIMEIRO CLICA COM O BOTÃO DIREITO E SELECIONA ALGUMA FIGURA PARA SER DESENHADA
//PARA IR DESENHADO ESSA FIGURA É SÓ IR CLICANDO COM O BOTAO ESQUERDO PARA IR MARCANDO OS PONTOS DO DESENHO
//NO DESENHO DE POLIGONOS(COM 4 PONTOS OU MAIS) QUANDO FINALIZAR O DESENHO CLIQUE COM O BOTAO DIREITO NA TELA
//SE QUISER PREENCHER A FIGURA CLIQUE NO FLOOD FILL E CLIQUE SOBRE A AREA QUE DESEJA PREENCHER
///SE FOR PREENCHER COM OUTRA COR SELECIONE A COR E DEPOIS CLIQUE EM FLOOD FILL 
//OBS LETRA F E H DO TRABALHO EXISTE POSSÍVEIS FUNÇÕES PORÉM NAO FORAM FINALIZADAS!


// Bibliotecas utilizadas pelo OpenGL
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Biblioteca com funcoes matematicas
#include <cmath>
#include <cstdio>
#include <cstdlib>

// Biblioteca para vetor
#include <vector>
#include <algorithm>
#include <iostream>

// Variaveis Globais
bool click1 = false, click2 = false, click3 = false, click4 = false, btn_direito = false;
bool reta = false, quadrilateros = false, triangulos = false, poligonos = false, circunferencias = false;
bool menuPressionado = false, aux = false, floodFill_aux = false;

float color[] = {0, 0, 1}; //cor q deseja mudar por padrão é azul
double raio = 100;
int cont1 = 4, cont2 = 4;
float cor1 = 0, cor2 = 0, cor3 = 0;

std::vector<double> arr;//contém os elementos a partir do 1 para tds as funções menos a bresenham e reta imediata
int x_1, y_1, x_2, y_2; //coordenadas para bresenham e reta imediata

int width = 512, height = 512; //Largura e altura da janela



// Estrututa de dados para o armazenamento dinamico dos pontos
// selecionados pelos algoritmos de rasterizacao
struct ponto
{
	int x;
	int y;
	float corR;
	float corG;
	float corB;
	ponto * prox;
};


// Lista encadeada de pontos
// indica o primeiro elemento da lista
ponto * pontos = NULL;


// Funcao para armazenar um ponto na lista
// Armazena como uma Pilha (empilha)
ponto * pushPonto(int x, int y)
{
	ponto * pnt;
	pnt = new ponto;
	pnt->x = x;
	pnt->y = y;
	pnt->prox = pontos;
	pnt->corR = cor1;
	pnt->corG = cor2;
	pnt->corB = cor3;
	pontos = pnt;
	return pnt;
}

// Funcao para desarmazenar um ponto na lista
// Desarmazena como uma Pilha (desempilha)
ponto * popPonto()
{
	ponto * pnt;
	pnt = NULL;
	if(pontos != NULL)
	{

		pnt = pontos->prox;
		delete pontos;
		pontos = pnt;
	}
	return pnt;
}

// Declaracoes forward das funcoes utilizadas
void init(void);
void reshape(int w, int h);
void display(void);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void CriaMenu();

// Funcao que implementa o Algoritmo Imediato para rasterizacao de retas
void retaImediata(double x1, double y1, double x2, double y2);

void bresenham(double x1, double y1, double x2, double y2);
void desenhaQuadrilatero(std::vector<double> arr);
void desenhaTriangulo(std::vector<double> arr);
void desenhaPoligono(std::vector<double> arr);
void rasterizaCircunferencia(double x1, double y1, double R);
void rasterizaPoligono(double x1, double y1, double x2, double y2);
void floodFill(int x, int y, float* fillColor, float* bc);
// Funcao que percorre a lista de pontos desenhando-os na tela
void drawPontos();

int lineHight = 25, lineMargin = 10, currentHight = 25;



// Funcao Principal do C
int main(int argc, char** argv)
{
	glutInit(&argc, argv); // Passagens de parametro C para o glut
	//glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB); // Selecao do Modo do Display e do Sistema de cor utilizado
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize (width, height);  // Tamanho da janela do OpenGL
	glutInitWindowPosition (100, 100); //Posicao inicial da janela do OpenGL
	glutCreateWindow ("Rasterizacao"); // Da nome para uma janela OpenGL
	//CriaMenu();

	glutReshapeFunc(reshape); //funcao callback para redesenhar a tela
	glutKeyboardFunc(keyboard); //funcao callback do teclado
	glRasterPos2f(lineMargin, currentHight);
	glutMouseFunc(mouse); //funcao callback do mouse

	glutDisplayFunc(display); //funcao callback de desenho
	init(); // Chama funcao init();
	glutMainLoop(); // executa o loop do OpenGL
	return 0; // retorna 0 para o tipo inteiro da funcao main();
}

// Funcao com alguns comandos para a inicializacao do OpenGL;
void init(void)
{
	glClearColor(1.0, 1.0, 1.0, 1.0); //Limpa a tela com a cor branca;
}

void reshape(int w, int h)
{
	// Reinicializa o sistema de coordenadas
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// Definindo o Viewport para o tamanho da janela
	glViewport(0, 0, w, h);

	width = w;
	height = h;
	glOrtho (0, w, 0, h, -1 , 1);

	// muda para o modo GL_MODELVIEW (não pretendemos alterar a projecção
	// quando estivermos a desenhar na tela)
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	click1 = true; //Para redesenhar os pixels selecionados
	click2 = true;
	click3 = true;
	click4 = true;
	btn_direito = true;
}

// Funcao usada na funcao callback para utilizacao das teclas normais do teclado
void keyboard(unsigned char key, int x, int y)
{
	switch (key)   // key - variavel que possui valor ASCII da tecla precionada
	{
	case 27: // codigo ASCII da tecla ESC
		exit(0); // comando pra finalizacao do programa
		break;
	}
}

//Funcao usada na funcao callback para a utilizacao do mouse
void mouse(int button, int state, int x, int y)
{
	switch (button)
	{
	case GLUT_LEFT_BUTTON:

		if (state == GLUT_DOWN && menuPressionado)
		{
			if(click1 && !click2)
			{
				click2 = true;
				arr.push_back(x);
				x_2 = x;
				arr.push_back(height - y);
				y_2 = height - y;
				printf("x2y2(%d,%d)\n", x, height - y);
				glutPostRedisplay();
			}
			else if(click1 && click2 && !click3)
			{
				click3 = true;
				arr.push_back(x);
				arr.push_back(height - y);
				printf("x3y3(%d,%d)\n", x, height - y);
				glutPostRedisplay();
			}
			else if( click1 && click2 && click3 && !click4)
			{
				click4 = true;
				arr.push_back(x);
				arr.push_back(height - y);
				printf("x%dy%d(%d,%d)\n", cont1, cont2, x, height - y);
				cont1++;
				cont2++;
				glutPostRedisplay();
			}
			else
			{
				click1 = true;
				arr.push_back(x);
				x_1 = x;
				arr.push_back(height - y);
				y_1 = height - y;
				printf("x1y1(%d,%d)\n", x, height - y);
				glutPostRedisplay();
			}

		}
		break;
		/*
		      case GLUT_MIDDLE_BUTTON:
		         if (state == GLUT_DOWN) {
		            glutPostRedisplay();
		         }
		         break;
		*/
	case GLUT_RIGHT_BUTTON://quando clicar com o botão direito...
		if (state == GLUT_DOWN)
		{
			//floodFill(x, height-y,velha,nova);
			if(!poligonos)
				CriaMenu();
			else
			{
				if(click1 && click2 && click3 && arr.size() > 6 && !click4)
				{
					btn_direito = true;
					aux = true;
				}
				click4 = true;
				glutPostRedisplay();
			}
		}
		break;
	default:
		break;
	}
}


// Funcao usada na funcao callback para desenhar na tela
void display(void)
{
	cor1 = 0, cor2 = 0, cor3 = 0;
	glColor3f (0.0, 0.0, 0.0); // Seleciona a cor default como preto
	glClear(GL_COLOR_BUFFER_BIT); //Limpa o Buffer de Cores
	glPointSize(4);//tamanho de cada ponto formado pelo breseham

	if(reta && !quadrilateros && !triangulos && !poligonos && !circunferencias)  //bresenham e quadrilatero
	{
		if(click1 && click2)
		{
			//retaImediata(x_1,y_1,x_2,y_2);
			bresenham(x_1, y_1, x_2, y_2);

			//rasterizaPoligono(x_1, y_1, x_2, y_2);
			drawPontos();
			click1 = false;
			click2 = false;
			click3 = false;
			arr.clear();//limpa o vetor/array


		}
	}
	else if(!reta && quadrilateros && !triangulos && !poligonos && !circunferencias)  //bresenham e quadrilatero
	{
		if(click1 && click2)
		{
			desenhaQuadrilatero(arr);
			drawPontos();
			click1 = false;
			click2 = false;
			click3 = false;
			arr.clear();//limpa o vetor/array


		}

	}

	else if(!reta && !quadrilateros && triangulos && !poligonos && !circunferencias)
	{
		if(click1 && click2 && click3)  //habitar para triangulo!
		{
			desenhaTriangulo(arr);
			drawPontos();
			click1 = false;
			click2 = false;
			click3 = false;
			click4 = false;
			btn_direito = false;
			arr.clear();//limpa o vetor/array


		}
	}
	else if(!reta && !quadrilateros && !triangulos && poligonos && !circunferencias)
	{
		if(click1 && click2 && click3 && click4 && btn_direito)
		{
			click1 = false;
			click2 = false;
			click3 = false;
			click4 = false;
			btn_direito = false;
			cont1 = 4;
			cont2 = 4;
			desenhaPoligono(arr);
			drawPontos();
			arr.clear();//limpa o vetor/array

		}

		if(click1 && click2 && click3 && click4 && !btn_direito)
		{
			click4 = false;
		}
		if(aux) //cria menu novamente apos o desenho de poligonos
		{
			aux = false;
			poligonos = false;
			menuPressionado = false;
		}
	}
	else if(!reta && !quadrilateros && !triangulos && !poligonos && circunferencias)
	{
		if(click1)
		{
			rasterizaCircunferencia(x_1, y_1, raio); //especificar raio!
			drawPontos();
			click1 = false;
			click2 = false;
			arr.clear();//limpa o vetor/array

		}

	}
	else if(floodFill_aux && !reta && !quadrilateros && !triangulos && !poligonos && !circunferencias)
	{
		if(click1)
		{

			float bCol[] = {0, 0, 0}; //cor do pixel clicado no caso clicou dentro da figura então é branco

			glPointSize(2);//tamanho do ponto de pintura
			floodFill(x_1, y_1, color, bCol);
			click1 = false;
			click2 = false;

			arr.clear();//limpa o vetor/array

		}
	}
	glutSwapBuffers();// manda o OpenGl renderizar as primitivas


}

//Funcao que desenha os pontos contidos em uma lista de pontos
void drawPontos()
{
	ponto * pnt;
	pnt = pontos;
	glBegin(GL_POINTS); // Seleciona a primitiva GL_POINTS para desenhar
	while(pnt != NULL)
	{
		glColor3f(pnt->corR, pnt->corG, pnt->corB);
		glVertex2i(pnt->x, pnt->y);
		pnt = pnt->prox;
	}
	glEnd();  // indica o fim do desenho
}


void bresenham(double x1, double y1, double x2, double y2)
{
	double dx, dy, d, aux;
	double incE, incNE;
	double x, y;
	bool declive = false, simetrico = false;

	dx = x2 - x1; //delta x = x2-x1
	dy = y2 - y1; //delta y = y2-y1

	if (dx * dy < 0) //transformar declives negativos em positivos
	{
		y1 = -y1;
		y2 = -y2;
		dy = -dy;
		simetrico = true;
	}
	if(abs(dx) < abs(dy)) //declive superior a 1 troca coordenadas x e y do segmento de reta
	{
		aux = y1;
		y1 = x1;
		x1 = aux;

		aux = y2;
		y2 = x2;
		x2 = aux;

		aux = dx;
		dx = dy;
		dy = aux;
		declive = true;
	}
	if(x1 > x2) //se x1>x2 troca a ordem dos extremos
	{

		aux = x1;
		x1 = x2;
		x2 = aux;

		aux = y1;
		y1 = y2;
		y2 = aux;

		dx = -dx;
		dy = -dy;
	}

	d = 2 * dy - dx;
	incE = 2 * dy;
	incNE = 2 * (dy - dx);

	//pontos = pushPonto(x1, y1); //inicializar x=x1 e y=y1 e marcar pixel com estas coordenadas
	while(x1 < x2) //enquanto x<x2
	{
		x = x1;//x = x1
		y = y1;//y = y1
		if(declive == true)
		{
			aux = x;
			x = y;
			y = aux;
		}
		if(simetrico == true)
		{
			y = -y;
		}
		if (d <= 0)
			d += incE;
		else
		{
			d += incNE;
			y1++;
		}
		x1 += 1;
		pontos = pushPonto(x, y);
	}


}

void desenhaQuadrilatero(std::vector<double> arr)
{
	for(int i = 0; i < arr.size(); i = i + 2)
	{
		bresenham((int)arr[i], (int)arr[i + 1], (int)arr[i], (int)arr[i + 3]);
		bresenham((int)arr[i], (int)arr[i + 3], (int)arr[i + 2], (int)arr[i + 3]);
		bresenham((int)arr[i + 2], (int)arr[i + 3], (int)arr[i + 2], (int)arr[i + 1]);
		bresenham((int)arr[i + 2], (int)arr[i + 1], (int)arr[i], (int)arr[i + 1]);
		break;
	}

}

void desenhaTriangulo(std::vector<double> arr)
{
	for (int i = 0 ; i < arr.size(); i = i + 2) //o array contem as coordenadas a partir do 4 click p frente
	{
		if(arr.size() - 2 == i) //se for o ultimo elemento
		{
			bresenham((int)arr[i], (int)arr[i + 1], (int)arr[0], (int)arr[1]);
		}
		else
			bresenham((int)arr[i], (int)arr[i + 1], (int)arr[i + 2], (int)arr[i + 3]);
	}

}

void desenhaPoligono(std::vector<double> arr)
{
	for (int i = 0 ; i < arr.size(); i = i + 2) //o array contem as coordenadas a partir do 4 click p frente
	{
		if(arr.size() - 2 == i) //se for o ultimo elemento
		{
			bresenham((int)arr[i], (int)arr[i + 1], (int)arr[0], (int)arr[1]);
		}
		else
			bresenham((int)arr[i], (int)arr[i + 1], (int)arr[i + 2], (int)arr[i + 3]);
	}
}

void translacao(double *x, double *y, double tx, double ty)
{
	double k[3] = {*x, *y, 1};
	double matriz[3][3] = {{1, 0, 0},
		{0, 1, 0},
		{tx, ty, 1}
	};
	double aux = 0;
	int i, j;
	double resultado[3];
	for (i = 0; i < 3; i++)
	{
		aux = 0;
		for(j = 0; j < 3; j++)
		{
			aux += k[j] * matriz[j][i];
		}
		resultado[i] = aux;
	}
	*x = resultado[0];
	*y = resultado[1];
}
void escala(double *x, double *y, double sx, double sy)
{
	double k[3] = {*x, *y, 1};
	double matriz[3][3] = {{sx, 0, 0},
		{0, sy, 0},
		{0, 0, 1}
	};
	double aux = 0;
	int i, j;
	double resultado[3];
	for (i = 0; i < 3; i++)
	{
		aux = 0;
		for(j = 0; j < 3; j++)
		{
			aux += k[j] * matriz[j][i];
		}
		resultado[i] = aux;
	}
	*x = resultado[0];
	*y = resultado[1];
}
void cisalhamento(double *x, double *y, double C, double direcao) //direcao 1 - eixo x 2- eixo y
{
	double k[3] = {*x, *y, 1};
	double matriz[3][3];
	if (direcao == 1)
	{
		int matriz[3][3] = {{1, 0, 0},
			{C, 1, 0},
			{0, 0, 1}
		};
	}
	else if(direcao == 2)
	{
		int matriz[3][3] = {{1, C, 0},
			{0, 1, 0},
			{0, 0, 1}
		};
	}
	double aux = 0;
	int i, j;
	double resultado[3];
	for (i = 0; i < 3; i++)
	{
		aux = 0;
		for(j = 0; j < 3; j++)
		{
			aux += k[j] * matriz[j][i];
		}
		resultado[i] = aux;
	}
	*x = resultado[0];
	*y = resultado[1];
}
void reflexao(double *x, double *y, int direcao) //direcao 1 - eixo x 2- eixo y 3-origem
{
	double k[3] = {*x, *y, 1};
	double matriz[3][3];
	if (direcao == 1)
	{
		double	matriz[3][3] = {{1, 0, 0},
			{0, -1, 0},
			{0, 0, 1}
		};
	}
	else if(direcao == 2)
	{
		double	matriz[3][3] = {{ -1, 0, 0},
			{0, 1, 0},
			{0, 0, 1}
		};
	}
	else if(direcao == 3)
	{
		double matriz[3][3] = {{ -1, 0, 0},
			{0, -1, 0},
			{0, 0, 1}
		};
	}
	double aux = 0;
	int i, j;
	double resultado[3];
	for (i = 0; i < 3; i++)
	{
		aux = 0;
		for(j = 0; j < 3; j++)
		{
			aux += k[j] * matriz[j][i];
		}
		resultado[i] = aux;
	}
	*x = resultado[0];
	*y = resultado[1];
}
void rotacao(double *x, double *y, double grau)
{

	int k[3] = {*x, *y, 1};
	double cosseno, seno;
	cosseno = cos( grau * M_PI / 180.0 );
	seno = sin(grau * M_PI / 180.0);
	double matriz[3][3] = {{cosseno, seno, 0},
		{ -seno, cosseno, 0},
		{0, 0, 1}
	};
	int aux = 0;
	int i, j;
	double resultado[3];
	for (i = 0; i < 3; i++)
	{
		aux = 0;
		for(j = 0; j < 3; j++)
		{
			aux += k[j] * matriz[j][i];
		}
		resultado[i] = aux;
	}
	*x = resultado[0];
	*y = resultado[1];

}


void rasterizaCircunferencia(double x1, double y1, double R) //(x1,y1) centro da circunferência R-raio
{
	double d, deltaE, deltaSE, x, y;
	if(x1 != 0 && y1 != 0)
	{
		d = 1 - R;
		deltaE = 3;
		deltaSE = -2 * R + 5;
		x = 0, y = R;
		//marcando os 4 pixels iniciais
		pontos = pushPonto(x1, y1 + (int)R);
		pontos = pushPonto(x1, y1 - (int)R);
		pontos = pushPonto(x1 + (int)R, y1);
		pontos = pushPonto(x1 - (int)R, y1);
		while(y > x)
		{
			if (d < 0)
			{
				d += deltaE;
				deltaE += 2;
				deltaSE += 2;
			}
			else
			{
				d += deltaSE;
				deltaE += 2;
				deltaSE += 4;
				y--;
			}
			x++;
			pontos = pushPonto((int)x + x1, (int)y + y1);
			pontos = pushPonto((int)y + x1, (int)x + y1);
			pontos = pushPonto((int)y + x1, (int) - x + y1);
			pontos = pushPonto((int)x + x1, (int) - y + y1);
			pontos = pushPonto((int) - x + x1, (int) - y + y1);
			pontos = pushPonto((int) - y + x1, (int) - x + y1);
			pontos = pushPonto((int) - y + x1, (int)x + y1);
			pontos = pushPonto((int) - x + x1, (int)y + y1);
		}
	}

}

void rasterizaPoligono(double x1, double y1, double x2, double y2)
{
	double tamanho, Dx, Dy, x, y, ymin, xmin;
	int i = 1;
	if (abs(x2 - x1) >= abs(y2 - y1))
	{
		tamanho = abs(x2 - x1);

	}
	else
	{
		tamanho = abs(y2 - y1);

	}

	Dx = (x2 - x1) / tamanho;
	Dy = (y2 - y1) / tamanho;
	ymin = (y1 < y2) ? y1 : y2;
	xmin = (x1 < x2) ? x1 : x2;
	x = xmin;
	y = ymin;
	while(i <= tamanho)
	{
		pushPonto((int)x, (int)y);
		x = x + Dx;
		y = y + Dy;
		i++;
	}

}


void floodFill(GLint x, GLint y, float* fillColor, float* bc)
{
	float color[3];
	glReadPixels(x, y, 1.0, 1.0, GL_RGB, GL_FLOAT, color);
	if((color[0] != bc[0] || color[1] != bc[1] || color[2] != bc[2]) && (
				color[0] != fillColor[0] || color[1] != fillColor[1] || color[2] != fillColor[2]))
	{
		glColor3f(fillColor[0], fillColor[1], fillColor[2]);
		cor1 = fillColor[0];
		cor2 = fillColor[1];
		cor3 = fillColor[2];
		glBegin(GL_POINTS);
		glVertex2i(x, y);
		glEnd();
		//glFlush();
		pushPonto(x, y);
		glutSwapBuffers();
		floodFill(x + 1, y, fillColor, bc);
		floodFill(x - 2, y, fillColor, bc);
		floodFill(x, y + 2, fillColor, bc);
		floodFill(x, y - 2, fillColor, bc);
	}
}



void retaImediata(double x1, double y1, double x2, double y2)
{
	double m, b, yd, xd;
	double xmin, xmax, ymin, ymax;
	//Armazenando os extremos para desenho
	pontos = pushPonto((int)x1, (int)y1);
	pontos = pushPonto((int)x2, (int)y2);

	if(x2 - x1 != 0) //Evita a divisão por zero
	{
		m = (y2 - y1) / (x2 - x1);
		b = y1 - (m * x1);

		if(m >= -1 && m <= 1) // Verifica se o declive da reta tem tg de -1 a 1, se verdadeira calcula incrementando x
		{
			xmin = (x1 < x2) ? x1 : x2;
			xmax = (x1 > x2) ? x1 : x2;

			for(int x = (int)xmin + 1; x < xmax; x++)
			{
				yd = (m * x) + b;
				yd = floor(0.5 + yd);
				pontos = pushPonto(x, (int)yd);
			}
		}
		else   // Se menor que -1 ou maior que 1, calcula incrementado os valores de y
		{
			ymin = (y1 < y2) ? y1 : y2;
			ymax = (y1 > y2) ? y1 : y2;

			for(int y = (int)ymin + 1; y < ymax; y++)
			{
				xd = (y - b) / m;
				xd = floor(0.5 + xd);
				pontos = pushPonto((int)xd, y);
			}
		}

	}
	else   // se x2-x1 == 0, reta perpendicular ao eixo x
	{
		ymin = (y1 < y2) ? y1 : y2;
		ymax = (y1 > y2) ? y1 : y2;
		for(int y = (int)ymin + 1; y < ymax; y++)
		{
			pontos = pushPonto((int)x1, y);
		}
	}

}

void menuCores(int op)
{
	menuPressionado = false;
	switch (op)
	{
		reta = false;
		quadrilateros = false;
		triangulos = false;
		poligonos = false;
		circunferencias = false;
	case 0:
		color[0] = 0, color[1] = 0, color[2] = 0; //preto
		break;
	case 1:
		color[0] = 1, color[1] = 0, color[2] = 0; //vermelho
		break;
	case 2:
		color[0] = 1, color[1] = 1, color[2] = 0; //amarelo
		break;
	case 3:
		color[0] = 0, color[1] = 1, color[2] = 0; //verde
		break;
	case 4:
		color[0] = 0, color[1] = 1, color[2] = 1; //ciano
		break;
	case 5:
		color[0] = 0, color[1] = 0, color[2] = 1; //azul
		break;
	case 6:
		color[0] = 1, color[1] = 0, color[2] = 1; //magenta
		break;
	case 7:
		printf("Valor R: ");
		scanf("%f",&color[0]);
		printf("Valor G: ");
		scanf("%f",&color[1]);
		printf("Valor B: ");
		scanf("%f",&color[2]);
		printf("Cor alterada!\n");
	default:
		break;
	}
	//glutPostRedisplay();
}


void menuCircunferencia(int op)
{
	menuPressionado = true;
	switch (op)
	{
	case 0:
		floodFill_aux = false;
		reta = false;
		quadrilateros = false;
		triangulos = false;
		poligonos = false;
		circunferencias = true;
		break;
	case 1:
		printf("Digite o valor do novo Raio!\n->");
		scanf("%lf", &raio);
		printf("\nRaio Mudou!\n");
	default:
		break;
	}
	glutPostRedisplay();
}


// Gerenciamento do menu com as opções de cores
void preencher(int op)
{
	menuPressionado = true;
	switch (op)
	{
	/*case 0:
		floodFill_aux = true;
		reta = false;
		quadrilateros = false;
		triangulos = false;
		poligonos = false;
		circunferencias = false;
		break;*/
	case 1:
		floodFill_aux = true;
		reta = false;
		quadrilateros = false;
		triangulos = false;
		poligonos = false;
		circunferencias = false;
		break;
	default:
		break;

	}
	//glutPostRedisplay();
}

void menuFiguras(int op)
{
	menuPressionado = true;
	switch (op)
	{
	case 0:
		floodFill_aux = false;
		reta = true;
		quadrilateros = false;
		triangulos = false;
		poligonos = false;
		circunferencias = false;
		break;
	case 1:
		reta = false;
		quadrilateros = true;
		triangulos = false;
		poligonos = false;
		circunferencias = false;
		break;
	case 2:
		floodFill_aux = false;
		triangulos = true;
		reta = false;
		quadrilateros = false;
		poligonos = false;
		circunferencias = false;
		break;
	case 3:
		floodFill_aux = false;
		reta = false;
		quadrilateros = false;
		triangulos = false;
		poligonos = true;
		circunferencias = false;
		glutDetachMenu(GLUT_RIGHT_BUTTON);
		break;

	default:
		break;


	}
	glutPostRedisplay();

}
void menuPrincipal(int op)
{
	glutPostRedisplay();
}
// Criacao do Menu
void CriaMenu(void)
{
	int menu, submenu0, submenu1, submenu2, submenu3;

	submenu3 = glutCreateMenu(menuCores);
	glutAddMenuEntry("Preto", 0);
	glutAddMenuEntry("Vermelho", 1);
	glutAddMenuEntry("Amarelo", 2);
	glutAddMenuEntry("Verde", 3);
	glutAddMenuEntry("Ciano", 4);
	glutAddMenuEntry("Azul", 5);
	glutAddMenuEntry("Magenta", 6);
	glutAddMenuEntry("Entrar com uma cor no prompt", 7);

	
	submenu1 = glutCreateMenu(menuCircunferencia);
	glutAddMenuEntry("Desenhar", 0);
	glutAddMenuEntry("Mudar Raio no prompt", 1);

	submenu0 = glutCreateMenu(menuFiguras);
	glutAddMenuEntry("Retas", 0);
	glutAddMenuEntry("Quadriláteros", 1);
	glutAddMenuEntry("Triângulos", 2);
	glutAddMenuEntry("Polígonos", 3);

	submenu2 = glutCreateMenu(preencher);
	glutAddSubMenu("Selecionar Cor", submenu3);
	//glutAddMenuEntry("Polígonos", 0);
	glutAddMenuEntry("Flood Fill", 1);

	menu = glutCreateMenu(menuPrincipal);
	glutAddSubMenu("Figuras", submenu0);
	glutAddSubMenu("Preencher", submenu2);
	glutAddSubMenu("Circunferêcias", submenu1);
	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

