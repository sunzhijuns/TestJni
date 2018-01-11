# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.4.1)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()

#add_compile_options(-std=c++11)

#如果当前系统版本过低设置拒绝编译
if(${ANDROID_PLATFORM_LEVEL} LESS 11)
	message(FATAL_ERROR "OpenGL 2 is not supported before API level 11(current using ${ANDROID_PLATFORM_LEVEL})")
	return()
elseif (${ANDROID_PLATFORM_LEVEL} LESS 18)
	add_definitions("-DDYNAMIC_ES3")
	set(OPENGL_LIB GLESv2)
else ()
	set(OPENGL_LIB GLESv3)
endif(${ANDROID_PLATFORM_LEVEL} LESS 11)



# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
             native-lib

             # Sets the library as a shared library.
             SHARED

             # Provides a relative path to your source file(s).
             src/main/cpp/native-lib.cpp 
			 src/main/cpp/bitmap/Bitmap.cpp)

add_library(native-activity 
			SHARED 
			src/main/cpp/native_activity/native-activity.cpp)					
			
add_library(triangle-lib 
			SHARED 
			src/main/cpp/triangle/esUtil.cpp
			src/main/cpp/triangle/triangle-lib.cpp)	

add_library(rect-lib 
			SHARED 
			src/main/cpp/rect/esUtil.cpp
			src/main/cpp/rect/rect-lib.cpp)	

add_library(cube-lib 
			SHARED 
			src/main/cpp/cube/esUtil.cpp
			src/main/cpp/cube/cube-lib.cpp)	
add_library(exa-lib 
			SHARED 
			src/main/cpp/exa/esUtil.cpp
			src/main/cpp/exa/VertexBufferObjectGen3/VertexBufferObjectGen3.cpp
			src/main/cpp/exa/VertexBufferObject/VertexBufferObject.cpp
			src/main/cpp/exa/VertexArrayObject/VertexArrayObject.cpp
			src/main/cpp/exa/SimpleVertexShader/SimpleVertexShader.cpp
			src/main/cpp/exa/SimpleTexture2D/SimpleTexture2D.cpp
			src/main/cpp/exa/MipMap2D/MipMap2D.cpp
			src/main/cpp/exa/DrawElements/DrawElements.cpp
			
			src/main/cpp/exa/exa-lib.cpp)	
# Searches for a specified prebuilt library and stores the path as a
# variable. Because CMake includes system libraries in the search path by
# default, you only need to specify the name of the public NDK library
# you want to add. CMake verifies that the library exists before
# completing its build.

find_library( # Sets the name of the path variable.
              log-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              log )
find_library( # Sets the name of the path variable.
              android-lib

              # Specifies the name of the NDK library that
              # you want CMake to locate.
              android )

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries( # Specifies the target library.
                       native-lib
					   
					   

                       # Links the target library to the log library
                       # included in the NDK.
                       ${log-lib}
					   jnigraphics
					   ${android-lib})
					   
					   
target_link_libraries(native-activity
			android
			EGL
			GLESv1_CM
			log)
			
			
target_link_libraries(triangle-lib 
			${OPENGL_LIB}
			android
			EGL
			log
			m)
			
target_link_libraries(rect-lib 
			${OPENGL_LIB}
			android
			EGL
			log
			m)
			
target_link_libraries(cube-lib 
			${OPENGL_LIB}
			android
			EGL
			log
			m)
			
target_link_libraries(exa-lib 
			${OPENGL_LIB}
			android
			EGL
			log
			m)