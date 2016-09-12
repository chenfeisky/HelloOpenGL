#pragma once


#include <stdio.h>
//#include <gl/glut.h>
#include "glut.h"
#include <vector>
#include <map>
#include <time.h>
#include <iostream>
#include <assert.h>
#include <string>
#include <fstream>  
#include <math.h>
#include <algorithm> 


using std::vector;

#define CHAPTER_6
#define WIN_DEBUG

using namespace std;

#define PI 3.1415926

#ifdef WIN_DEBUG
#include <Windows.h>
inline void outPut(const char* s)
{
	OutputDebugString(s);
}
#endif
