#pragma once

#if defined _WIN32

	#if !defined WINGDIAPI

		#define WINGDIAPI __declspec( dllimport )

	#endif

	#if !defined APIENTRY

		#define APIENTRY __stdcall

	#endif

	#if !defined CALLBACK

		#define CALLBACK __stdcall

	#endif

	#include <GL/gl.h>
	#include <GL/glu.h>

#elif defined __linux

	#include <GL/gl.h>
	#include <GL/glu.h>

#elif defined __APPLE__

	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>

#endif