# This file should only be included when using Pthreads

SET(LIB_NAME OpenThreads)
SET(LIB_PUBLIC_HEADERS ${OpenThreads_PUBLIC_HEADERS})

ADD_LIBRARY(${LIB_NAME}
    ${OPENTHREADS_USER_DEFINED_DYNAMIC_OR_STATIC}
    ${LIB_PUBLIC_HEADERS}
	PThread.c++
	PThreadBarrier.c++
	PThreadBarrierPrivateData.h
	PThreadCondition.c++
	PThreadConditionPrivateData.h
	PThreadMutex.c++
	PThreadMutexPrivateData.h
	PThreadPrivateData.h
)

TARGET_LINK_LIBRARIES(${LIB_NAME}
	pthread
)

# Since we're building different platforms binaries in 
# their respective directories, we need to set the 
# link directory so it can find this location.
LINK_DIRECTORIES(
	${CMAKE_CURRENT_BINARY_DIR}
)

INSTALL(
	TARGETS OpenThreads
	ARCHIVE DESTINATION lib
	LIBRARY DESTINATION lib
	RUNTIME DESTINATION bin
)
INSTALL(
	FILES ${OpenThreads_PUBLIC_HEADERS}
	DESTINATION include/OpenThreads
)

#commented out# INCLUDE(ModuleInstall OPTIONAL)
