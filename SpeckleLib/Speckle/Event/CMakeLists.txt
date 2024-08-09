set(SourceFiles
	${CMAKE_CURRENT_SOURCE_DIR}/Publisher.cpp
)
set(HeaderFiles
	${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
	${CMAKE_CURRENT_SOURCE_DIR}/Event.h
	${CMAKE_CURRENT_SOURCE_DIR}/Event.md
	${CMAKE_CURRENT_SOURCE_DIR}/Message.h
	${CMAKE_CURRENT_SOURCE_DIR}/Participant.h
	${CMAKE_CURRENT_SOURCE_DIR}/PostBox.h
	${CMAKE_CURRENT_SOURCE_DIR}/Publisher.h
	${CMAKE_CURRENT_SOURCE_DIR}/Subscriber.h
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
