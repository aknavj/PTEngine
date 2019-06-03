#ifndef __ENGINE_CORE_H__
#define __ENGINE_CORE_H__

#include "Base.h"

#include "Singleton.h"
#include "Counter.h"
#include "Reference.h"
#include "PropertySet.h"
#include "Functor.h"
#include "Profiler.h"
#include "Factory.h"
#include "Object.h"
#include "Allocator.h"

#include "MathLib.h"
#include "Noise.h"
#include "Matrix.h"
#include "Geometry.h"

#include "PixelBuffer.h"
#include "Texture.h"
#include "Font.h"

#include "Kernel.h"
#include "KernelTask.h"
#include "TimerTask.h"
#include "Trigger.h"
#include "Interpolator.h"
#include "InputTask.h"
#include "VideoTask.h"
#include "CameraTask.h"
#include "ConsoleTask.h"
#include "CaptureTask.h"

#include "GLBufferObject.h"
#include "GLShaderObject.h"
#include "GLFrameBufferObject.h"

extern const std::string g_strBuildStamp;

#define ENGINE_WIDTH	1280
#define ENGINE_HEIGHT	1024
#define ENGINE_LOG		"Generator"
#define ENGINE_CONFIG	"Generator.conf"
#define ENGINE_TITLE	"Planetary Engine"

#define DECLARE_CORE_GLOBALS\
	const std::string g_strBuildStamp = std::string(__DATE__) + std::string(" ") + std::string(__TIME__);\
	CLog *CLog::m_pSingleton = NULL;\
	CObjectType CObjectType::m_root


#endif /* __ENGINE_H__ */
