#ifndef __BASE_H__
#define __BASE_H__

#define interface struct

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <cmath>
#include <float.h>
#include <stdarg.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>

#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL.h>

#ifdef _WIN32
	//#pragma inline_depth( 255 )
	//#pragma inline_recursion( on )
	//#pragma auto_inline( on )
	#pragma warning(disable:4786)

	#define STRICT
	#define WIN32_LEAN_AND_MEAN
	#include <crtdbg.h>
	#include <windows.h>
	#include <mmsystem.h>
#endif



extern "C" {
	#include <png.h>
};

#define PI					3.14159f
#define HALF_PI				1.57080f
#define TWO_PI				6.28318f
#define INV_PI				0.318310f
#define INV_TWO_PI			0.159155f
#define INV_HALF_PI			0.636618f

#define LOGHALF				-0.693147f
#define LOGHALFI			-1.442695f
#define DELTA				1e-6f 
#define MAX_DIMENSIONS		4
#define MAX_OCTAVES			128

#define HALF_RAND			(RAND_MAX/2)

#include "Log.h"

#define LOG_GLERROR() {\
	GLenum err = glGetError();\
	if(err != GL_NO_ERROR)\
		LogError("[%s line %d] GL Error: %s", __FILE__, __LINE__, gluErrorString(err));\
}


template <class T> void Swap(T &a, T &b)		{ T t = a; a = b; b = t; }

#ifndef WIN32
	#define DWORD unsigned long
	#define _MAX_PATH 512
	inline char* itoa(int val, char *buf, int base) {
		int i = 30;
		for (; val && i; --i, val /= base)
			buf[i] = "0123456789abcdef"[val % base];
		return &buf[i + 1];
	}
#endif

#endif /* __BASE_H__ */
