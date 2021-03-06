
# The atom_types.h file is written to the build directory
INCLUDE_DIRECTORIES( ${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY (atomcore SHARED
	DefineLink.cc
	FreeLink.cc
	LambdaLink.cc
	PutLink.cc
	VariableList.cc
)

# Without this, parallel make will race and crap up the generated files.
ADD_DEPENDENCIES(atomcore opencog_atom_types)

TARGET_LINK_LIBRARIES(atomcore
	${COGUTIL_LIBRARY}
)

INSTALL (TARGETS atomcore
	DESTINATION "lib${LIB_DIR_SUFFIX}/opencog"
)

INSTALL (FILES
	DefineLink.h
	FreeLink.h
	LambdaLink.h
	PutLink.h
	VariableList.h
	DESTINATION "include/opencog/atoms/core"
)
