cmake_minimum_required(VERSION 3.20)

IF (WIN32)
	add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
	add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
ENDIF()

add_library(
    SpeckleLib
)

	#Add the sources for a module
function(addModule moduleName)
	add_subdirectory(${moduleName})
	source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/${moduleName} PREFIX ${moduleName} FILES ${ModuleFiles})
endfunction()

	#Module sources
addModule(Database)
addModule(Environment)
addModule(Event)
addModule(Serialise)
addModule(Utility)
	#Add sources at the root
set(SourceFiles
)
set(HeaderFiles
	${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
	${CMAKE_CURRENT_SOURCE_DIR}/Version.h
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
set(ModuleFiles
	${SourceFiles}
	${HeaderFiles}
)
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${ModuleFiles})

add_library(${namespace}SpeckleLib ALIAS SpeckleLib)

IF (WIN32)
	target_compile_definitions(SpeckleLib PRIVATE
        WINDOWS
        _LIB
    )
ENDIF()

target_compile_features(SpeckleLib PUBLIC
	cxx_std_20
)

set_target_properties(SpeckleLib PROPERTIES
    OUTPUT_NAME "${SpeckleLib_lib_name}"
)

target_include_directories(
    SpeckleLib PUBLIC
    $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:include>
)

# ---- library installation ----
install(
    TARGETS SpeckleLib
    EXPORT "${targets_export_name}"
    FILE_SET HEADERS
    INCLUDES DESTINATION "${include_install_dir}"
)
