#include<GL/freeglut.h>
#include <windows.h>
#include <math.h>
#include <stdio.h>
#include "BMP_IO.h"

static GLfloat spin = 0.0;	//旋轉量
static GLfloat side = 16; //邊數
static int sleep = 10; //刷新間隔
static int rotate = -1; //旋轉方向
static int bmpRotate = 0; //bmp旋轉方向
static int isRotate = 1; //是否旋轉
static int isR = 1, isG = 1, isB = 1; //顏色選擇
static int main_w, w1, w2, w3; //主視窗 & 子視窗
static const char* filename = "baboon.bmp";
static BYTE* pixels[512 * 512 * 3] = {0};
static int width = 512, height = 512;

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0); //指定清除顏色（黑色），即背景顏色
	glShadeModel(GL_FLAT);
}

//繪製一個點
void drawPoint(float x, float y, float r, float g, float b)
{
	x = (x - width/2) / (width/2);
	y = (y - height/2) / (height/2);
	r /= 255;
	g /= 255;
	b /= 255;
	glColor3f(r, g, b); //繪製顏色RGB
	glVertex2f(x, y);
}

//顯示Bitmap
void bmpShow(void) {
	glClear(GL_COLOR_BUFFER_BIT);

	BMP_Read(filename,  pixels, width, height);

	glPushMatrix();
	glBegin(GL_POINTS);
	
	if(bmpRotate == 0) // 正常
		for(int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
			{
			
				float r = (*pixels)[(i * height + j) * 3 + 0];
				float g = (*pixels)[(i * height + j) * 3 + 1];
				float b = (*pixels)[(i * height + j) * 3 + 2];

				drawPoint(i, j, r, g, b);
			}
	else if(bmpRotate == 1) // 順時針
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
			{

				float r = (*pixels)[(j * width + i) * 3 + 0];
				float g = (*pixels)[(j * width + i) * 3 + 1];
				float b = (*pixels)[(j * width + i) * 3 + 2];

				drawPoint(i, j, r, g, b);
			}
	else if (bmpRotate == -1)//逆時針
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
			{

				float r = (*pixels)[(j * width + (height - 1 - i)) * 3 + 0];
				float g = (*pixels)[(j * width + (height - 1 - i)) * 3 + 1];
				float b = (*pixels)[(j * width + (height - 1 - i)) * 3 + 2];

				drawPoint(i, j, r, g, b);
			}
	else if (bmpRotate == 2) // 轉180
		for (int i = 0; i < height; i++)
			for (int j = 0; j < width; j++)
			{

				float r = (*pixels)[((height - 1 - i) * height + (width -1 - j)) * 3 + 0];
				float g = (*pixels)[((height - 1 - i) * height + (width -1 - j)) * 3 + 1];
				float b = (*pixels)[((height - 1 - i) * height + (width -1 - j)) * 3 + 2];

				drawPoint(i, j, r, g, b);
			}
	
	glEnd();
	glPopMatrix();
	glutSwapBuffers();

}

//繪製多邊形
void drawCircle(void)
{
	int num_vertice = side + 2; //點數

	GLfloat Pi2 = 3.14 * 2;
	GLfloat verticeX[200]; //點數max = 200 
	GLfloat verticeY[200];
	verticeX[0] = 0; //中心點座標
	verticeY[0] = 0;
	GLfloat radius = 0.5; //半徑
	GLfloat color = 0;

	//計算所有點的座標
	for (int i = 1; i < num_vertice; i++)
	{
		verticeX[i] = verticeX[0] + (radius * cos(i * Pi2 / side));
		verticeY[i] = verticeY[0] + (radius * sin(i * Pi2 / side));
	}

	glClear(GL_COLOR_BUFFER_BIT); //清除所有的畫素
	glPushMatrix();
	glRotatef(spin, 0, 0, rotate); //旋轉，引數含義(旋轉量,x軸,y軸,z軸)
	glShadeModel(GL_FLAT);
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < num_vertice; i++)
	{
		if(i <= num_vertice/2 && i > 0)
			color += 1 / (side/2);
		else 
			color -= 1 / (side/2);

		//繪製顏色RGB

		glColor3f(color*isR, color*isG, color*isB);
		glVertex2f(verticeX[i], verticeY[i]);
	}
	glEnd();
	glPopMatrix();
	glutSwapBuffers();
}

// 多邊形旋轉
void spinDisplay()
{
	if (spin > 360)
		spin -= 360;
	else if (isRotate == 0)
		spin = 0;
	else 
		spin += 2;

	
	glutPostRedisplay(); //標記當前視窗需要重繪，否則不會旋轉
	Sleep(sleep);
}



//右鍵選單功能
//旋轉
void subMenu1(int index)
{
	switch (index)
	{
	case 1:
		isRotate = 1;
		break;
	case 2:
		isRotate = 0;
		break;
	case 3:
		rotate = -1;
		break;
	case 4:
		rotate = 1;
		break;
	}
}

//邊數
void subMenu2(int index) //定義按鈕
{
	switch (index)
	{
	case 1:
		side = 8;
		break;
	case 2:
		side = 16;
		break;
	case 3:
		side = 32;
		break;
	case 4:
		side = 64;
		break;
	case 5:
		side = 128;
		break;
	}
}

//轉速
void subMenu3(int index) //定義按鈕
{
	switch (index)
	{
	case 1:
		sleep = 25;
		break;
	case 2:
		sleep = 20;
		break;
	case 3:
		sleep = 15;
		break;
	case 4:
		sleep = 10;
		break;
	case 5:
		sleep = 5;
		break;
	case 6:
		sleep = 1;
		break;
	}
}

//顏色
void subMenu4(int index) //定義按鈕
{
	switch (index)
	{
	case 1:
		isR = 1;
		isG = 0;
		isB = 0;
		break;
	case 2:
		isR = 0;
		isG = 1;
		isB = 0;
		break;
	case 3:
		isR = 0;
		isG = 0;
		isB = 1;
		break;
	case 4:
		isR = 1;
		isG = 1;
		isB = 1;
		break;
	}
}

//Bitmap
void subMenu5(int index) //定義按鈕
{
	switch (index)
	{
	case 1:
		bmpRotate = 0;
		w1 = glutCreateSubWindow(main_w, 516, 0, 512, 512);
		glutDisplayFunc(bmpShow); //顯示
		break;
	case 2:
		bmpRotate = 1;
		w1 = glutCreateSubWindow(main_w, 516, 0, 512, 512);
		glutDisplayFunc(bmpShow); //顯示
		break;
	case 3:
		bmpRotate = -1;
		w1 = glutCreateSubWindow(main_w, 516, 0, 512, 512);
		glutDisplayFunc(bmpShow); //顯示
		break;
	case 4:
		bmpRotate = 2;
		w1 = glutCreateSubWindow(main_w, 516, 0, 512, 512);
		glutDisplayFunc(bmpShow); //顯示
		break;
	}

}

//主選單
void mainMenu(int index)
{
	switch (index)
	{
	case 1:
		glutLeaveMainLoop();
	}
}

void createMenu()
{
	///setting all the submenus
	  //sub-menu 1 旋轉
	int submenu1 = glutCreateMenu(subMenu1);
	glutAddMenuEntry("Start", 1);
	glutAddMenuEntry("Stop", 2);
	glutAddMenuEntry("Clockwise", 3);
	glutAddMenuEntry("Counterclockwise", 4);

	//sub-menu 2 邊數
	int submenu2 = glutCreateMenu(subMenu2);
	glutAddMenuEntry("8", 1);
	glutAddMenuEntry("16", 2);
	glutAddMenuEntry("32", 3);
	glutAddMenuEntry("64", 4);
	glutAddMenuEntry("128", 5);

	//sub-menu 3 轉速
	int submenu3 = glutCreateMenu(subMenu3);
	glutAddMenuEntry("Speed_1", 1);
	glutAddMenuEntry("Speed_2", 2);
	glutAddMenuEntry("Speed_3", 3);
	glutAddMenuEntry("Speed_4", 4);
	glutAddMenuEntry("Speed_5", 5);
	glutAddMenuEntry("Speed_6", 6);

	//sub-menu 4 顏色
	int submenu4 = glutCreateMenu(subMenu4);
	glutAddMenuEntry("Red", 1);
	glutAddMenuEntry("Green", 2);
	glutAddMenuEntry("Blue", 3);
	glutAddMenuEntry("Black", 4);

	//sub-menu 5 Bitmap
	int submenu5 = glutCreateMenu(subMenu5);
	glutAddMenuEntry("Normal", 1);
	glutAddMenuEntry("Clockwise 90", 2);
	glutAddMenuEntry("Counterclockwise 90", 3);
	glutAddMenuEntry("Upside Down", 4);

	///main menu
	int menu = glutCreateMenu(mainMenu);
	glutAddSubMenu("Rotation", submenu1);
	glutAddSubMenu("Sides", submenu2);
	glutAddSubMenu("Speed", submenu3);
	glutAddSubMenu("Color", submenu4);
	glutAddSubMenu("Bitmap", submenu5);
	glutAddMenuEntry("EXIT", 1);

	glutAttachMenu(GLUT_RIGHT_BUTTON);

}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv); //初始化GLUT並處理命令列引數
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB); //指定模式：雙快取；RGB模式
	glutInitWindowSize(1030, 724); //指定視窗大小（畫素）
	main_w = glutCreateWindow(argv[0]); //使用OpenGL場景建立主視窗，引數為視窗名稱
	init();	//呼叫初始化函式

	createMenu();
	
	//子視窗1 - 狒狒
	w1 = glutCreateSubWindow(main_w, 516, 0, 512, 512);
	glutDisplayFunc(bmpShow); //顯示
	glClearColor(0.5, 0.0, 0.5, 1.0);
	
	//子視窗2 - 多邊形
	w2 = glutCreateSubWindow(main_w, 2, 0, 512, 512);
	glutDisplayFunc(drawCircle); //顯示
	glutIdleFunc(spinDisplay); //旋轉
	glClearColor(0.5, 0.5, 0.0, 1.0);

	glutMainLoop(); //進入主迴圈

	return 0;
}




