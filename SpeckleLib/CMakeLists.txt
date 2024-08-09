cmake_minimum_required(VERSION 3.21)

if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif ()

project(
    SpeckleLib
    VERSION 0.9.0
    LANGUAGES CXX
)

# ---- Set C++ standard ----
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(SPECKLELIB_TESTS "Build the SpeckleLib tests" ON)

# ---- Target parameters ----
set(SpeckleLib_lib_name "speckle")

# ---- Installation parameters ----
set(generated_dir "${CMAKE_CURRENT_BINARY_DIR}/generated")

set(include_install_dir "include")
set(config_install_dir "lib/cmake/${PROJECT_NAME}")

set(project_config "${generated_dir}/${PROJECT_NAME}Config.cmake")
set(version_config "${generated_dir}/${PROJECT_NAME}ConfigVersion.cmake")
set(targets_export_name "${PROJECT_NAME}Targets")
set(namespace "SpeckleLib::")

set(pkgconfig_install_dir "lib/pkgconfig")

set(project_pkgconfig "${generated_dir}/pkgconfig/specklelib.pc")

# ---- Package configuration ----
include(GNUInstallDirs)
set(INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
    CACHE PATH "Location of header files")
set(SYSCONFIG_INSTALL_DIR "${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}"
    CACHE PATH "Location of configuration files")

include(CMakePackageConfigHelpers)

configure_package_config_file(
    "${PROJECT_NAME}Config.cmake.in"
    "${project_config}"
    INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}"
    PATH_VARS INCLUDE_INSTALL_DIR SYSCONFIG_INSTALL_DIR
)

write_basic_package_version_file(
    "${version_config}"
    VERSION ${SpeckleLib_VERSION}
    COMPATIBILITY SameMinorVersion
)

# ---- pkg-config
configure_file(
    specklelib.pc.in
    "${project_pkgconfig}" @ONLY
)

# generate_export_header()

add_subdirectory(Speckle)

# ---- cmake config installation ----
install(
    FILES "${project_config}" "${version_config}"
    DESTINATION "${config_install_dir}"
)

install(
    EXPORT "${targets_export_name}"
    NAMESPACE "${namespace}"
    DESTINATION "lib/cmake/${PROJECT_NAME}"
)

# ---- pkg-config installation ----
install(
    FILES "${project_pkgconfig}"
    DESTINATION "${pkgconfig_install_dir}"
)

if(SPECKLELIB_TESTS)
    add_subdirectory(SpeckleLibDoctest)
endif()
