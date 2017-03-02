#pragma once


#include <stdio.h>
//#include <gl/glut.h>
#include "glut.h"

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <list>

#include <time.h>
#include <iostream>
#include <assert.h>
#include <fstream>
#include <math.h>
#include <algorithm> 
#include <thread>

using std::vector;

//#define TOOLS
#define CHAPTER_7
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
