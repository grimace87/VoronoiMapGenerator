
/**************************
* Includes
*
**************************/

// Prevent erros which inhibit use of deprecated functions (eg. strcpy)
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <windows.h>
#include <commctrl.h>
#include <gl/gl.h>

#define GLM_FORCE_RADIANS
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/type_ptr.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"

#include "classes.hpp"
#include "glext.h"

/**************************
* Function Declarations
* and other stuff
**************************/

int SiteGenMode = 0;
int PathFindMode = 0;
int MapTypeMode = 0;

// Memory for display data
float *BoundaryData;
float *SiteBuffer;
float *Diagram;
float *DiagramSub;
float *LineData;
float *MapData;
int NoOfDiagramPolyVertices = 0;
int NoOfSubPolyVertices = 0;
int NoOfDiagramLines = 0;
int NoOfMapDataVertices;
GLuint BufferObjects[6];
GLuint TexObject;
GLubyte* TexData;

// Handles to main windows
HWND hWndMain;
HWND hWndSide;

// Handles to controls within the side window
HWND hWndButtonRegen;
HWND hWndButtonDrawMode;
HWND hWndSiteGenComboBox;
HWND hWndPathFindComboBox;
HWND hWndMapTypeComboBox;

LRESULT CALLBACK WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcRegen(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcDrawMode(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcSiteGen(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcPathFind(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProcMapType(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

WNDPROC DefProcRegen;
WNDPROC DefProcDrawMode;
WNDPROC DefProcSiteGen;
WNDPROC DefProcPathFind;
WNDPROC DefProcMapType;

HWND CreateComboBox(HINSTANCE, HWND, int, char[][16], int);

// Functions for setting up OpenGL
void EnableOpenGL(HWND hWnd, HDC *hDC, HGLRC *hRC);
void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC);
void GetGLFunctions();
int GenerateShaderProgram(HWND hWnd);
GLubyte* ReadBitmap(HWND, const char*);

// Prototypes for OpenGL 3 functions
PFNGLCREATESHADERPROC glCreateShader;
PFNGLSHADERSOURCEPROC glShaderSource;
PFNGLCOMPILESHADERPROC glCompileShader;
PFNGLGETSHADERIVPROC glGetShaderiv;
PFNGLCREATEPROGRAMPROC glCreateProgram;
PFNGLATTACHSHADERPROC glAttachShader;
PFNGLBINDATTRIBLOCATIONPROC glBindAttribLocation;
PFNGLLINKPROGRAMPROC glLinkProgram;
PFNGLGETPROGRAMIVPROC glGetProgramiv;
PFNGLGENBUFFERSPROC glGenBuffers;
PFNGLDELETEBUFFERSPROC glDeleteBuffers;
PFNGLBINDBUFFERPROC glBindBuffer;
PFNGLBUFFERDATAPROC glBufferData;
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation;
PFNGLGETATTRIBLOCATIONPROC glGetAttribLocation;
PFNGLUSEPROGRAMPROC glUseProgram;
PFNGLUNIFORMMATRIX4FVPROC glUniformMatrix4fv;
PFNGLUNIFORM1IPROC glUniform1i;
PFNGLENABLEVERTEXATTRIBARRAYPROC glEnableVertexAttribArray;
PFNGLVERTEXATTRIBPOINTERPROC glVertexAttribPointer;

// Handles to the shader programs
GLuint Program;
GLuint TexProgram;

// Handles to the shader parameters
GLuint TriMVPMatrixHandle;
GLuint TriPositionHandle;
GLuint TriColourHandle;
GLuint TexMVPMatrixHandle;
GLuint TexPositionHandle;
GLuint TexTexCoordHandle;
GLuint TexUniHandle;

// View parameters
int DisplayMode = 3;
float OffsetZ = -0.6f;
float OffsetY = 0.4f;
float Yaw = -0.4f;

/**************************
* WinMain
*
**************************/

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int iCmdShow)
{
	WNDCLASS wc;
	HDC hDC;
	HGLRC hRC;
	MSG msg;
	BOOL bQuit = FALSE;
	double theta = 0.0;

	/* register window class */
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WndProcMain;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = "StockWindow";
	RegisterClass(&wc);

	// Some local memory needed for map data
	Voronoi* Voro;

	char strText[9][16];

	// Create main window
	hWndMain = CreateWindow(
		"StockWindow", "Grimace - Voronoi Map Generator",
		WS_CAPTION | WS_POPUPWINDOW | WS_VISIBLE,
		300, 4, 808, 832,
		NULL, NULL, hInstance, NULL
		);

	// Create child window (for controls)
	hWndSide = CreateWindow(
		"StockWindow", "Grimace - Voronoi Control Panel",
		WS_CAPTION | WS_VISIBLE | WS_POPUPWINDOW | WS_CHILD,
		4, 4, 296, 832,
		hWndMain, NULL, hInstance, NULL
		);

	// Create controls (as child 'windows')
	hWndButtonRegen = CreateWindow(
		"BUTTON", "Regenerate",
		WS_DLGFRAME | WS_VISIBLE | WS_CHILD,
		10, 10, 276, 32,
		hWndSide, NULL, hInstance, NULL
		);
	DefProcRegen = (WNDPROC)SetWindowLong(hWndButtonRegen, GWL_WNDPROC, (LONG)WndProcRegen);

	hWndButtonDrawMode = CreateWindow(
		"BUTTON", "Change Draw Mode",
		WS_DLGFRAME | WS_VISIBLE | WS_CHILD,
		10, 52, 276, 32,
		hWndSide, NULL, hInstance, NULL
		);
	DefProcDrawMode = (WNDPROC)SetWindowLong(hWndButtonDrawMode, GWL_WNDPROC, (LONG)WndProcDrawMode);
	
	strcpy(strText[0], "Full random");
	strcpy(strText[1], "Grid aligned");
	strcpy(strText[2], "Sequential Y");
	hWndSiteGenComboBox = CreateComboBox(hInstance, hWndSide, 1, strText, 3);
	DefProcSiteGen = (WNDPROC)SetWindowLong(hWndSide, GWL_WNDPROC, (LONG)WndProcSiteGen);
	strcpy(strText[0], "5 from one");
	strcpy(strText[1], "5 in sequence");
	strcpy(strText[2], "10 from one");
	strcpy(strText[3], "10 in sequence");
	hWndPathFindComboBox = CreateComboBox(hInstance, hWndSide, 2, strText, 4);
	DefProcPathFind = (WNDPROC)SetWindowLong(hWndSide, GWL_WNDPROC, (LONG)WndProcPathFind);
	strcpy(strText[0], "Random map type");
	strcpy(strText[1], "Forest map");
	strcpy(strText[2], "Lava map");
	strcpy(strText[3], "Lake map");
	strcpy(strText[4], "Arid map");
	strcpy(strText[5], "Cave map");
	strcpy(strText[6], "Snow map");
	strcpy(strText[7], "Volcano map");
	strcpy(strText[8], "Flowers map");
	hWndMapTypeComboBox = CreateComboBox(hInstance, hWndSide, 3, strText, 9);
	DefProcMapType = (WNDPROC)SetWindowLong(hWndSide, GWL_WNDPROC, (LONG)WndProcMapType);

	// Transformation matrices
	glm::mat4 ModelMatrix;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	glm::mat4 MVPMatrix;

	// Enable OpenGL for the window, and obtain pointers to the newer functions
	EnableOpenGL(hWndMain, &hDC, &hRC);
	GetGLFunctions();

	// Generate shader
	GenerateShaderProgram(hWndMain);

	// Get handles into the shader
	TriMVPMatrixHandle = glGetUniformLocation(Program, "u_MVPMatrix");
	TriPositionHandle = glGetAttribLocation(Program, "a_Position");
	TriColourHandle = glGetAttribLocation(Program, "a_Colour");
	TexMVPMatrixHandle = glGetUniformLocation(TexProgram, "u_MVPMatrix");
	TexUniHandle = glGetUniformLocation(TexProgram, "u_Texture");
	TexPositionHandle = glGetAttribLocation(TexProgram, "a_Position");
	TexTexCoordHandle = glGetAttribLocation(TexProgram, "a_TexCoord");
	glGetError();
	glUseProgram(Program);

	if (glGetError() != GL_NO_ERROR) {

		MessageBox(hWndMain, "Encountered a problem\nsetting up OpenGL", "Error", MB_OK);
		PostQuitMessage(0);

	}
	else {

		// Set up display parameters
		ModelMatrix = glm::mat4();
		ViewMatrix = glm::mat4();
		ProjectionMatrix = glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 0.9f, 1000.0f);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

		char Addr[256];
		int ReadChars = GetCurrentDirectory(256, Addr);
		Voro = new Voronoi(SiteGenMode);
		BoundaryData = Voronoi::GenerateBoundaryBuffer();
		SiteBuffer = Voro->GenerateSiteBuffer(SiteGenMode);
		LineData = Voro->GenerateDiagramReturnLines(&NoOfDiagramLines, PathFindMode);
		Diagram = Voro->GenerateDiagramVertices(&NoOfDiagramPolyVertices);
		DiagramSub = Voro->GenerateSubVertices(&NoOfSubPolyVertices);
		MapData = PlainMapGen::GenerateMapData(&NoOfMapDataVertices, MapTypeMode, Voro, &Addr[ReadChars]);
		TexData = ReadBitmap(hWndMain, Addr);

		delete Voro;

		glGenBuffers(6, BufferObjects);
		glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
		glBufferData(GL_ARRAY_BUFFER, 220 * sizeof(float), BoundaryData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[1]);
		glBufferData(GL_ARRAY_BUFFER, NoOfSites * 5 * sizeof(float), SiteBuffer, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[2]);
		glBufferData(GL_ARRAY_BUFFER, NoOfDiagramPolyVertices * 5 * sizeof(float), Diagram, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[3]);
		glBufferData(GL_ARRAY_BUFFER, NoOfDiagramLines * 10 * sizeof(float), LineData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[4]);
		glBufferData(GL_ARRAY_BUFFER, NoOfSubPolyVertices * 5 * sizeof(float), DiagramSub, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[5]);
		glBufferData(GL_ARRAY_BUFFER, NoOfMapDataVertices * 5 * sizeof(float), MapData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	/* program main loop */
	while (!bQuit) {

		/* check for messages */
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {

			/* handle or dispatch messages */
			if (msg.message == WM_QUIT)
			{
				bQuit = TRUE;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

		}
		else {

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			theta += 0.002;
			if (theta > 2.0) theta -= 2.0;

			// Adjust model matrix
			ModelMatrix = glm::mat4(1.0f);
			ModelMatrix = glm::rotate(ModelMatrix, Yaw, glm::vec3(1.0f, 0.0f, 0.0f));
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, OffsetY*(BoundYMax - BoundYMin), 0.0f));
			ModelMatrix = glm::rotate(ModelMatrix, (float)(M_PI * theta), glm::vec3(0.0f, 0.0f, 1.0f));
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-0.5f*(BoundXMax - BoundXMin), -0.5f*(BoundYMax - BoundYMin), OffsetZ*(BoundXMax - BoundXMin)));

			// Calculate matrices
			MVPMatrix = ProjectionMatrix * ViewMatrix * ModelMatrix;

			if (glGetError() != GL_NO_ERROR) {
				MessageBox(hWndMain, "GL error", "Error", MB_OK);
				PostQuitMessage(0);
			}

			// Pass in uniform information
			glUniformMatrix4fv(TriMVPMatrixHandle, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
			glEnableVertexAttribArray(TriPositionHandle);
			glEnableVertexAttribArray(TriColourHandle);

			// Draw boundaries
			glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[0]);
			glVertexAttribPointer(TriPositionHandle, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
			glVertexAttribPointer(TriColourHandle, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const GLvoid*)(2 * sizeof(GLfloat)));
			glDrawArrays(GL_LINES, 0, 8);

			// Draw diagram
			if (DisplayMode == 0) {
				glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[3]);
				glVertexAttribPointer(TriPositionHandle, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
				glVertexAttribPointer(TriColourHandle, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const GLvoid*)(2 * sizeof(GLfloat)));
				glDrawArrays(GL_LINES, 0, NoOfDiagramLines * 2);
			}
			else if (DisplayMode == 1) {
				glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[2]);
				glVertexAttribPointer(TriPositionHandle, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
				glVertexAttribPointer(TriColourHandle, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const GLvoid*)(2 * sizeof(GLfloat)));
				glDrawArrays(GL_TRIANGLES, 0, NoOfDiagramPolyVertices);
			}
			else if (DisplayMode == 2) {
				glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[4]);
				glVertexAttribPointer(TriPositionHandle, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
				glVertexAttribPointer(TriColourHandle, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const GLvoid*)(2 * sizeof(GLfloat)));
				glDrawArrays(GL_TRIANGLES, 0, NoOfSubPolyVertices);
			}
			else if (DisplayMode == 3) {
				// Draw sky box
				glEnable(GL_DEPTH_TEST);
				glUseProgram(TexProgram);
				glUniformMatrix4fv(TexMVPMatrixHandle, 1, GL_FALSE, glm::value_ptr(MVPMatrix));
				glUniform1i(TexUniHandle, 0);
				glEnableVertexAttribArray(TexPositionHandle);
				glEnableVertexAttribArray(TriColourHandle);
				glVertexAttribPointer(TexPositionHandle, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
				glVertexAttribPointer(TexTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const GLvoid*)(3 * sizeof(GLfloat)));
				glDrawArrays(GL_TRIANGLES, 8, 36);
				// Draw the rest
				glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[5]);
				glVertexAttribPointer(TexPositionHandle, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
				glVertexAttribPointer(TexTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const GLvoid*)(3 * sizeof(GLfloat)));
				glDrawArrays(GL_TRIANGLES, 0, NoOfMapDataVertices);
				glUseProgram(Program);
			}

			// Draw sites
			glDisable(GL_DEPTH_TEST);
			glPointSize(5.0f);
			glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[1]);
			glVertexAttribPointer(TriPositionHandle, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
			glVertexAttribPointer(TriColourHandle, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (const GLvoid*)(2 * sizeof(GLfloat)));
			glDrawArrays(GL_POINTS, 0, NoOfSites);

			// Swap buffers and wait a bit
			SwapBuffers(hDC);
			Sleep(10);

		}
	}

	// Free memory
	delete[] BoundaryData;
	delete[] SiteBuffer;
	delete[] Diagram;
	delete[] TexData;
	glDeleteBuffers(6, BufferObjects);
	glDeleteTextures(6, &TexObject);

	/* shutdown OpenGL */
	DisableOpenGL(hWndMain, hDC, hRC);

	/* destroy the window explicitly */
	DestroyWindow(hWndMain);

	return msg.wParam;
}

LRESULT CALLBACK WndProcMain(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {

	switch (message)
	{
	case WM_CREATE:
		return 0;
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0;

	case WM_DESTROY:
		return 0;

	case WM_KEYDOWN:
		switch (wParam) {
		case VK_ESCAPE:
			PostQuitMessage(0);
			return 0;
		case VK_UP:
			Yaw += 0.05f;
			return 0;
		case VK_DOWN:
			Yaw -= 0.05f;
			return 0;
		case VK_HOME:
			OffsetZ -= 0.05f;
			return 0;
		case VK_END:
			OffsetZ += 0.05f;
			return 0;
		case VK_INSERT:
			OffsetY += 0.05f;
			return 0;
		case VK_DELETE:
			OffsetY -= 0.05f;
			return 0;
		}
		return 0;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

// Create a combo box to fit within the tab control
HWND CreateComboBox(HINSTANCE hInstance, HWND hWndParent, int iBoxNo, char cTextItems[][16], int nItems) {

	// Variables
	RECT rcClient;
	HWND hWndCombo;
	int n;

	// Set up window area to fit into
	GetClientRect(hWndParent, &rcClient);
	TabCtrl_AdjustRect(hWndParent, FALSE, &rcClient);
	rcClient.left += 10;
	rcClient.right -= 10;
	rcClient.top = 55 + 35 * iBoxNo;

	// Create the window
	hWndCombo = CreateWindow(
		WC_COMBOBOX,
		NULL,
		CBS_DROPDOWNLIST | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
		rcClient.left,
		rcClient.top,
		rcClient.right,
		rcClient.bottom,
		hWndParent,
		NULL,
		hInstance,
		NULL
		);
	if (hWndCombo == NULL)
		return NULL;

	// Add text components
	for (n = 0; n < nItems; n++)
		SendMessage(hWndCombo, (UINT)CB_ADDSTRING, (WPARAM)0, (LPARAM)cTextItems[n]);

	// Finish the box by selecting an initial element, and return
	SendMessage(hWndCombo, (UINT)CB_SETCURSEL, (WPARAM)0, (LPARAM)0);
	return hWndCombo;

}

void EnableOpenGL(HWND hWnd, HDC *hDC, HGLRC *hRC) {
	PIXELFORMATDESCRIPTOR pfd;
	int iFormat;

	/* get the device context (DC) */
	*hDC = GetDC(hWnd);

	/* set the pixel format for the DC */
	ZeroMemory(&pfd, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	pfd.cDepthBits = 16;
	pfd.iLayerType = PFD_MAIN_PLANE;
	iFormat = ChoosePixelFormat(*hDC, &pfd);
	SetPixelFormat(*hDC, iFormat, &pfd);

	/* create and enable the render context (RC) */
	*hRC = wglCreateContext(*hDC);
	wglMakeCurrent(*hDC, *hRC);

}

void DisableOpenGL(HWND hWnd, HDC hDC, HGLRC hRC) {
	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRC);
	ReleaseDC(hWnd, hDC);
}

void GetGLFunctions() {

	glCreateShader = (PFNGLCREATESHADERPROC)wglGetProcAddress("glCreateShader");
	glShaderSource = (PFNGLSHADERSOURCEPROC)wglGetProcAddress("glShaderSource");
	glCompileShader = (PFNGLCOMPILESHADERPROC)wglGetProcAddress("glCompileShader");
	glGetShaderiv = (PFNGLGETSHADERIVPROC)wglGetProcAddress("glGetShaderiv");
	glCreateProgram = (PFNGLCREATEPROGRAMPROC)wglGetProcAddress("glCreateProgram");
	glAttachShader = (PFNGLATTACHSHADERPROC)wglGetProcAddress("glAttachShader");
	glBindAttribLocation = (PFNGLBINDATTRIBLOCATIONPROC)wglGetProcAddress("glBindAttribLocation");
	glLinkProgram = (PFNGLLINKPROGRAMPROC)wglGetProcAddress("glLinkProgram");
	glGetProgramiv = (PFNGLGETPROGRAMIVPROC)wglGetProcAddress("glGetProgramiv");
	glGenBuffers = (PFNGLGENBUFFERSPROC)wglGetProcAddress("glGenBuffers");
	glDeleteBuffers = (PFNGLDELETEBUFFERSPROC)wglGetProcAddress("glDeleteBuffers");
	glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	glGetUniformLocation = (PFNGLGETUNIFORMLOCATIONPROC)wglGetProcAddress("glGetUniformLocation");
	glGetAttribLocation = (PFNGLGETATTRIBLOCATIONPROC)wglGetProcAddress("glGetAttribLocation");
	glUseProgram = (PFNGLUSEPROGRAMPROC)wglGetProcAddress("glUseProgram");
	glUniformMatrix4fv = (PFNGLUNIFORMMATRIX4FVPROC)wglGetProcAddress("glUniformMatrix4fv");
	glUniform1i = (PFNGLUNIFORM1IPROC)wglGetProcAddress("glUniform1i");
	glEnableVertexAttribArray = (PFNGLENABLEVERTEXATTRIBARRAYPROC)wglGetProcAddress("glEnableVertexAttribArray");
	glVertexAttribPointer = (PFNGLVERTEXATTRIBPOINTERPROC)wglGetProcAddress("glVertexAttribPointer");

}

int GenerateShaderProgram(HWND hWnd) {

	const char* VertexShaderSource =
		"uniform mat4 u_MVPMatrix;"
		"attribute vec2 a_Position;"
		"attribute vec3 a_Colour;"

		"void main() {"
		"    gl_FrontColor = vec4(a_Colour, 1.0f);"
		"    gl_Position = u_MVPMatrix * vec4(a_Position, 0.0f, 1.0f);"
		"}";

	const char* FragmentShaderSource =
		"precision mediump float;"

		"void main() {"
		"    gl_FragColor = gl_Color;"
		"}";

	const char* TexVertexShaderSource =
		"uniform mat4 u_MVPMatrix;"
		"attribute vec4 a_Position;"
		"attribute vec2 a_TexCoord;"
		"varying vec2 v_TexCoord;"

		"void main() {"
		"    v_TexCoord = a_TexCoord;"
		"    gl_Position = u_MVPMatrix * a_Position;"
		"}";

	const char* TexFragmentShaderSource =
		"precision mediump float;"
		"uniform sampler2D u_Texture;"
		"varying vec2 v_TexCoord;"

		"void main() {"
		"    gl_FragColor = texture2D(u_Texture, v_TexCoord);"
		"}";

	const GLint VertexShaderLength = strlen(VertexShaderSource);
	const GLint FragmentShaderLength = strlen(FragmentShaderSource);
	const GLint TexVertexShaderLength = strlen(TexVertexShaderSource);
	const GLint TexFragmentShaderLength = strlen(TexFragmentShaderSource);

	GLuint VertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLuint TexVertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint TexFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(VertexShader, 1, &VertexShaderSource, &VertexShaderLength);
	glShaderSource(FragmentShader, 1, &FragmentShaderSource, &FragmentShaderLength);
	glShaderSource(TexVertexShader, 1, &TexVertexShaderSource, &TexVertexShaderLength);
	glShaderSource(TexFragmentShader, 1, &TexFragmentShaderSource, &TexFragmentShaderLength);

	GLint Compiled;
	glCompileShader(VertexShader);
	glGetShaderiv(VertexShader, GL_COMPILE_STATUS, &Compiled);
	if (Compiled != GL_TRUE) {
		MessageBox(hWnd, "Failed compilation of vertex shader.", "Error", MB_OK);
		return 0;
	}
	glCompileShader(FragmentShader);
	glGetShaderiv(FragmentShader, GL_COMPILE_STATUS, &Compiled);
	if (Compiled != GL_TRUE) {
		MessageBox(hWnd, "Failed compilation of fragment shader.", "Error", MB_OK);
		return 0;
	}

	Program = glCreateProgram();
	glAttachShader(Program, VertexShader);
	glAttachShader(Program, FragmentShader);
	glBindAttribLocation(Program, 0, "a_Position");
	glBindAttribLocation(Program, 1, "a_Colour");
	glLinkProgram(Program);
	glGetProgramiv(Program, GL_LINK_STATUS, &Compiled);
	if (Compiled != GL_TRUE) {
		MessageBox(hWnd, "Failed to link shader program.", "Error", MB_OK);
		return 0;
	}

	glCompileShader(TexVertexShader);
	glGetShaderiv(TexVertexShader, GL_COMPILE_STATUS, &Compiled);
	if (Compiled != GL_TRUE) {
		MessageBox(hWnd, "Failed compilation of 2nd vertex shader.", "Error", MB_OK);
		return 0;
	}
	glCompileShader(TexFragmentShader);
	glGetShaderiv(TexFragmentShader, GL_COMPILE_STATUS, &Compiled);
	if (Compiled != GL_TRUE) {
		MessageBox(hWnd, "Failed compilation of 2nd fragment shader.", "Error", MB_OK);
		return 0;
	}

	TexProgram = glCreateProgram();
	glAttachShader(TexProgram, TexVertexShader);
	glAttachShader(TexProgram, TexFragmentShader);
	glBindAttribLocation(TexProgram, 0, "a_Position");
	glBindAttribLocation(TexProgram, 1, "a_TexCoord");
	glLinkProgram(TexProgram);
	glGetProgramiv(TexProgram, GL_LINK_STATUS, &Compiled);
	if (Compiled != GL_TRUE) {
		MessageBox(hWnd, "Failed to link 2nd shader program.", "Error", MB_OK);
		return 0;
	}

	return 1;

}

GLubyte* ReadBitmap(HWND hWnd, const char *Filename) {

	GLubyte *Data1 = NULL;

	BITMAPFILEHEADER BMFH;
	BITMAPINFOHEADER BMIH;


	FILE *InputBMP = fopen(Filename, "rb");
	if (InputBMP == NULL) {
		MessageBox(hWnd, Filename, "Missing bitmap:", MB_OK);
		return 0;
	}

	fread(&BMFH, sizeof(BITMAPFILEHEADER), 1, InputBMP);
	if (BMFH.bfType != 19778) {
		MessageBox(hWnd, "File appears not to be a bitmap file.", "Error", MB_OK);
		return 0;
	}

	fread(&BMIH, sizeof(BITMAPINFOHEADER), 1, InputBMP);
	if (BMIH.biBitCount != 24) {
		MessageBox(hWnd, "I'm not reading non-24-bit bitmaps.", "Error", MB_OK);
		return 0;
	}

	DWORD Size = BMFH.bfSize - BMFH.bfOffBits;

	Data1 = new GLubyte[Size];
	if (Data1 == 0) {
		MessageBox(hWnd, "Couldn't allocate memory to load bitmap.", "Error", MB_OK);
		return 0;
	}

	fread(Data1, sizeof(GLubyte), Size, InputBMP);

	fclose(InputBMP);

	glGenTextures(1, &TexObject);
	glBindTexture(GL_TEXTURE_2D, TexObject);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(
		GL_TEXTURE_2D, 0, 3, BMIH.biWidth, BMIH.biHeight, 0,
		GL_BGR, GL_UNSIGNED_BYTE, Data1
		);

	return Data1;

}

LRESULT CALLBACK WndProcRegen(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam) {

	Voronoi* Voro;

	switch (message) {

	case WM_LBUTTONDOWN:

		delete[] SiteBuffer;
		delete[] Diagram;
		delete[] LineData;
		delete[] DiagramSub;
		delete[] MapData;
		delete[] TexData;

		char Addr[256];
		int ReadChars = GetCurrentDirectory(256, Addr);
		Voro = new Voronoi(SiteGenMode);
		SiteBuffer = Voro->GenerateSiteBuffer(SiteGenMode);
		LineData = Voro->GenerateDiagramReturnLines(&NoOfDiagramLines, PathFindMode);
		Diagram = Voro->GenerateDiagramVertices(&NoOfDiagramPolyVertices);
		DiagramSub = Voro->GenerateSubVertices(&NoOfSubPolyVertices);
		MapData = PlainMapGen::GenerateMapData(&NoOfMapDataVertices, MapTypeMode, Voro, &Addr[ReadChars]);
		TexData = ReadBitmap(hWndMain, Addr);
		delete Voro;

		glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[1]);
		glBufferData(GL_ARRAY_BUFFER, NoOfSites * 5 * sizeof(float), SiteBuffer, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[2]);
		glBufferData(GL_ARRAY_BUFFER, NoOfDiagramPolyVertices * 5 * sizeof(float), Diagram, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[3]);
		glBufferData(GL_ARRAY_BUFFER, NoOfDiagramLines * 10 * sizeof(float), LineData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[4]);
		glBufferData(GL_ARRAY_BUFFER, NoOfSubPolyVertices * 5 * sizeof(float), DiagramSub, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, BufferObjects[5]);
		glBufferData(GL_ARRAY_BUFFER, NoOfMapDataVertices * 5 * sizeof(float), MapData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

	}

	return DefProcRegen(hWnd, message, wParam, lParam);

}

LRESULT CALLBACK WndProcDrawMode(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam) {

	switch (message) {

	case WM_LBUTTONDOWN:

		DisplayMode++;
		if (DisplayMode > 3)
			DisplayMode = 0;

	}

	return DefProcDrawMode(hWnd, message, wParam, lParam);

}

LRESULT CALLBACK WndProcSiteGen(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam) {

	if (message == WM_COMMAND && HIWORD(wParam) == CBN_SELCHANGE) {
		SiteGenMode = (int)SendMessage(hWndSiteGenComboBox, CB_GETCURSEL, 0, 0);
	}

	return DefProcSiteGen(hWnd, message, wParam, lParam);

}

LRESULT CALLBACK WndProcPathFind(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam) {

	if (message == WM_COMMAND && HIWORD(wParam) == CBN_SELCHANGE) {
		PathFindMode = (int)SendMessage(hWndPathFindComboBox, CB_GETCURSEL, 0, 0);
	}

	return DefProcPathFind(hWnd, message, wParam, lParam);

}

LRESULT CALLBACK WndProcMapType(HWND hWnd, UINT message,
	WPARAM wParam, LPARAM lParam) {

	if (message == WM_COMMAND && HIWORD(wParam) == CBN_SELCHANGE) {
		MapTypeMode = (int)SendMessage(hWndMapTypeComboBox, CB_GETCURSEL, 0, 0);
	}

	return DefProcMapType(hWnd, message, wParam, lParam);

}
