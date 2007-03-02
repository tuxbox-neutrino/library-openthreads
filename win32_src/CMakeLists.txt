# This file should only be included when WIN32

SET(LIB_NAME OpenThreads)
SET(LIB_PUBLIC_HEADERS ${OpenThreads_PUBLIC_HEADERS})

ADD_LIBRARY(${LIB_NAME}
    ${OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC}
    ${LIB_PUBLIC_HEADERS}
	HandleHolder.h
	Win32BarrierPrivateData.h
	WIN32Condition.cpp
	Win32Condition.h
	Win32ConditionPrivateData.h
	Win32Mutex.cpp
	Win32MutexPrivateData.h
	Win32Thread.cpp
	Win32ThreadBarrier.cpp
	Win32ThreadPrivateData.h
)

# Since we're building different platforms binaries in 
# their respective directories, we need to set the 
# link directory so it can find this location.
LINK_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}
)

INCLUDE(ModuleInstall OPTIONAL)
