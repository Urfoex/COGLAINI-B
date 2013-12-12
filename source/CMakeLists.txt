#--
#- Find needed packages
#--

OPTION( SFML_STATIC_LIBRARIES "Using static libraries for SFML" OFF)
SET( SFMLDIR "" CACHE PATH "Folder where SFML libraries could be found")

FIND_PACKAGE( SFML REQUIRED WINDOW SYSTEM GRAPHICS AUDIO)
IF( NOT SFML_FOUND)
	MESSAGE( FATAL_ERROR "SFML not found!" )
ENDIF( NOT SFML_FOUND)
INCLUDE_DIRECTORIES( ${SFML_INCLUDE_DIR} )


FIND_PACKAGE( OpenGL REQUIRED )
IF( NOT OPENGL_FOUND)
	MESSAGE( FATAL_ERROR "OpenGL not found!" )
ENDIF( NOT OPENGL_FOUND)
INCLUDE_DIRECTORIES( ${OPENGL_INCLUDE_DIR} )


FIND_PACKAGE( ASSIMP REQUIRED )
IF( NOT ASSIMP_FOUND)
	MESSAGE( FATAL_ERROR "ASSIMP not found!" )
ENDIF( NOT ASSIMP_FOUND)
INCLUDE_DIRECTORIES( ${ASSIMP_INCLUDE_DIR} )


FIND_PACKAGE( Bullet REQUIRED )
IF( NOT BULLET_FOUND)
	MESSAGE( FATAL_ERROR "Bullet not found!" )
ENDIF( NOT BULLET_FOUND)
INCLUDE_DIRECTORIES( ${BULLET_INCLUDE_DIR} )


FIND_PACKAGE( OpenNI REQUIRED )
IF( NOT OPENNI_FOUND)
	MESSAGE( FATAL_ERROR "OpenNI not found!" )
ENDIF( NOT OPENNI_FOUND)
INCLUDE_DIRECTORIES( ${OPENNI_INCLUDE_DIR} )


FIND_PACKAGE( Nite REQUIRED )
IF( NOT NITE_FOUND)
	MESSAGE( FATAL_ERROR "Nite not found!" )
ENDIF( NOT NITE_FOUND)
INCLUDE_DIRECTORIES( ${NITE_INCLUDE_DIR} )

find_package( GLEW REQUIRED)
IF( NOT GLEW_FOUND)
	MESSAGE( FATAL_ERROR "GLEW not found!" )
ENDIF( NOT GLEW_FOUND)
INCLUDE_DIRECTORIES( ${GLEW_INCLUDE_PATH} )

#--
#- Add some flags to the compiler
#--

if( ${CMAKE_BUILD_TYPE} MATCHES Debug)
	message("(!) Debug-Build")
	set( DEBUG 1)
	if(CMAKE_COMPILER_IS_GNUCXX )
		set( CMAKE_VERBOSE_MAKEFILE true)
		if( NOT WIN32)
			set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdouble-promotion -ldl ")
		endif( NOT WIN32)
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wextra -g3 -Weffc++ -pedantic -Wswitch-default -Wswitch-enum -Wshadow -std=c++0x ")
	endif(CMAKE_COMPILER_IS_GNUCXX)
elseif( ${CMAKE_BUILD_TYPE} MATCHES Release)
	message("(!) Release-Build")
	set( DEBUG 0)
	if(CMAKE_COMPILER_IS_GNUCXX )
		set( CMAKE_VERBOSE_MAKEFILE true)
		if( NOT WIN32)
			set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ldl ")
		endif( NOT WIN32)
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -g0 -std=c++0x ")
	endif(CMAKE_COMPILER_IS_GNUCXX)
else( ${CMAKE_BUILD_TYPE} MATCHES Debug)
	message("(!) Debug-Build")
	set( DEBUG 1)
	if(CMAKE_COMPILER_IS_GNUCXX )
		set( CMAKE_VERBOSE_MAKEFILE true)
		if( NOT WIN32)
			set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wdouble-promotion -ldl ")
		endif( NOT WIN32)
		set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -Wall -Wextra -g3 -Weffc++ -pedantic -Wswitch-default -Wswitch-enum -Wshadow -std=c++0x ")
	endif(CMAKE_COMPILER_IS_GNUCXX)
endif( ${CMAKE_BUILD_TYPE} MATCHES Debug)


#--
# Grab and use all source and header from current directory
#--
FILE( GLOB thisDirH RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.h )
FILE( GLOB thisDirCPP RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.cpp )
add_executable( coglainib  ${thisDirCPP} ${thisDirH})

target_link_libraries( coglainib ${SFML_LIBRARIES} ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${BULLET_LIBRARIES} ${OPENNI_LIBRARY} ${NITE_LIBRARY})

IF( ${CMAKE_BUILD_TYPE} MATCHES Debug)
	TARGET_LINK_LIBRARIES( coglainib ${ASSIMP_LIBRARY_DBG})
ELSE()
	TARGET_LINK_LIBRARIES( coglainib ${ASSIMP_LIBRARY_REL})
ENDIF()


#--
#- say which files should be "installed" and where
#--

install( TARGETS coglainib DESTINATION bin )


#--
#- maybe install some extra stuff if needed on Windows
#--

# if( WIN32 )
# 	install( FILES 
# 	${CMAKE_CURRENT_BINARY_DIR}/SDL.dll
# 	DESTINATION src/gui
# 	)
# endif( WIN32 )

add_subdirectory(samples)
add_subdirectory(headers_with_gcc_warnings)