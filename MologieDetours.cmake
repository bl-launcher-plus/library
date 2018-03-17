set(MologieDetours ${CMAKE_CURRENT_LIST_DIR}/MologieDetours)

OPTION(BUILD_EXAMPLES "Build examples" OFF)

include_directories(
	${MologieDetours}/
)

ADD_LIBRARY(MologieDetours STATIC ${MologieDetours}/hde.cpp)