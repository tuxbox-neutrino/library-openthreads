CMAKE_MINIMUM_REQUIRED(VERSION 2.4.6 FATAL_ERROR)

PROJECT(OpenThreads)

################################################################################
# Installation stuff

SET(CMAKE_DEBUG_POSTFIX  "d")
#SET(INSTALL_BINDIR OpenThreads/bin)
#SET(INSTALL_INCDIR OpenThreads/include)
#SET(INSTALL_LIBDIR OpenThreads/lib)
#SET(INSTALL_DOCDIR OpenThreads/docs)

################################################################################


# common files 
SET(OpenThreads_PUBLIC_HEADERS
	${OpenThreads_SOURCE_DIR}/include/OpenThreads/Barrier
	${OpenThreads_SOURCE_DIR}/include/OpenThreads/Condition
	${OpenThreads_SOURCE_DIR}/include/OpenThreads/Exports
	${OpenThreads_SOURCE_DIR}/include/OpenThreads/Mutex
	${OpenThreads_SOURCE_DIR}/include/OpenThreads/ScopedLock
	${OpenThreads_SOURCE_DIR}/include/OpenThreads/Thread
	${OpenThreads_SOURCE_DIR}/include/OpenThreads/Block
	${OpenThreads_SOURCE_DIR}/include/OpenThreads/ReentrantMutex
	${OpenThreads_SOURCE_DIR}/include/OpenThreads/ReadWriteMutex
)

# Add a source group for the include headers so they are seen in IDEs.
# Hmmm, this isn't working for me in Xcode.
# SOURCE_GROUP(
# 	"Header Files"
# 	FILES ${OpenThreads_PUBLIC_HEADERS}
# )

# Make the headers visible to everything
INCLUDE_DIRECTORIES(
	${OpenThreads_SOURCE_DIR}/include
)

# This is mainly for Windows declspec, but other platforms know
# what to do with it.
ADD_DEFINITIONS(-DOPENTHREADS_EXPORTS)

# User Options
OPTION(DYNAMIC_OPENTHREADS "Set to ON to build OpenThreads for dynamic linking.  Use OFF for static." ON)
IF   (DYNAMIC_OPENTHREADS)
	SET(OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC "SHARED")
ELSE (DYNAMIC_OPENTHREADS)
	SET(OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC "STATIC")
ENDIF(DYNAMIC_OPENTHREADS)

# It doesn't look like sproc is used by OpenThreads
# Do we have sproc?
#IF(CMAKE_SYSTEM MATCHES IRIX)
#   CHECK_INCLUDE_FILES("sys/types.h;sys/prctl.h"  CMAKE_HAVE_SPROC_H)
#ENDIF(CMAKE_SYSTEM MATCHES IRIX)

# Maybe we should be using the FindThreads.cmake module?
IF(WIN32)
	# So I think Cygwin wants to use pthreads
	IF(CYGWIN)
		SUBDIRS(pthread_src examples_src)
	ELSE(CYGWIN)
		# Everybody else including Msys should probably go here
		SUBDIRS(win32_src) 
		# examples_src)
	ENDIF(CYGWIN)
ELSE(WIN32)
	IF(UNIX)
		SUBDIRS(pthread_src examples_src)
	ELSE(UNIX)
		MESSAGE("Sorry, OpenThreads may not support your platform")
	ENDIF(UNIX)
ENDIF(WIN32)


# FIXME: Make optional install test programs
# FIXME: /bin directory not correct for Windows
# INSTALL_TARGETS(/bin OpenThreads)
 
#SET(OUTPUT_BINDIR ${PROJECT_BINARY_DIR}/bin/${CMAKE_SYSTEM_NAME})
SET(OUTPUT_BINDIR ${PROJECT_BINARY_DIR}/bin)
MAKE_DIRECTORY(${OUTPUT_BINDIR})
SET(EXECUTABLE_OUTPUT_PATH ${OUTPUT_BINDIR})

#SET(OUTPUT_LIBDIR ${PROJECT_BINARY_DIR}/lib/${CMAKE_SYSTEM_NAME})
SET(OUTPUT_LIBDIR ${PROJECT_BINARY_DIR}/lib)
MAKE_DIRECTORY(${OUTPUT_LIBDIR})
SET(LIBRARY_OUTPUT_PATH ${OUTPUT_LIBDIR})


# For Doxygen
INCLUDE(${CMAKE_ROOT}/Modules/Documentation.cmake OPTIONAL)

# To build the documention, you will have to enable it
# and then do the equivalent of "make DoxygenDoc".
IF(BUILD_DOCUMENTATION)
	IF(DOT)
		SET(HAVE_DOT YES)
	ELSE(DOT)
		SET(HAVE_DOT NO)
	ENDIF(DOT)
	# This processes our Doxyfile.in and substitutes paths to generate
	# a final Doxyfile
	CONFIGURE_FILE(${PROJECT_SOURCE_DIR}/docs/doxyfile.cmake
		${PROJECT_BINARY_DIR}/docs/doxyfile
		)
	# This creates a new target to build documentation.
	# It runs ${DOXYGEN} which is the full path and executable to
	# Doxygen on your system, set by the FindDoxygen.cmake module
	# (called by FindDocumentation.cmake).
	# It runs the final generated Doxyfile against it.
	# The DOT_PATH is substituted into the Doxyfile.
	ADD_CUSTOM_TARGET(DoxygenDoc ${DOXYGEN}
		${PROJECT_BINARY_DIR}/docs/doxyfile
	)
ENDIF(BUILD_DOCUMENTATION)

