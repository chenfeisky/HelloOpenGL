#pragma once
#pragma warning(disable:4996)

#include <stdio.h>
//#include <gl/glut.h>
#include "GL/glew.h"
#include "glut.h"

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <list>
#include <bitset>

#include <time.h>
#include <iostream>
#include <sstream>  
#include <assert.h>
#include <fstream>
#include <math.h>
#include <algorithm> 
#include <thread>

#include "stb_image.h"
#include "stb_image_write.h"
#include "stb_image_resize.h"

using std::vector;

//#define TOOLS
#define CHAPTER 22

#define WIN_DEBUG


#ifdef CHAPTER
#define CHAPTERHEAD(x) opengl##x
#define _CHAPTERNAME(x) CHAPTERHEAD(x)h.h
#define CHAPTERNAME _CHAPTERNAME(CHAPTER)
#define _FILENAME(x) #x
#define FILENAME(x) _FILENAME(x)
#include FILENAME(CHAPTERNAME)
#endif

using namespace std;

#define PI 3.141592653589793

#ifdef WIN_DEBUG
#include <Windows.h>
inline void outPut(const char* s)
{
	OutputDebugString(s);
}
#endif
