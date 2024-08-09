set(SourceFiles
	${CMAKE_CURRENT_SOURCE_DIR}/App.cpp
)
set(HeaderFiles
	${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
	${CMAKE_CURRENT_SOURCE_DIR}/App.h
	${CMAKE_CURRENT_SOURCE_DIR}/Environment.md
)
set(ModuleFiles
	${SourceFiles}
	${HeaderFiles}
	PARENT_SCOPE
)
target_sources(SpeckleLib
	PRIVATE
		${SourceFiles}
	PUBLIC
	FILE_SET HEADERS
	BASE_DIRS ${PROJECT_SOURCE_DIR}
	FILES
		${HeaderFiles}
)
