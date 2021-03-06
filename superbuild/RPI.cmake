set (proj RPI)

set (cmake_args
  ${common_cache_args}
  -DRPI_BUILD_EXAMPLES:BOOL=OFF
  -DITK_DIR:PATH=${ITK_BUILD_DIR}
  )

set (location "")
if (NOT DEFINED ${proj}_SRC_DIR)
  set(location GIT_REPOSITORY ${GITHUB_PREFIX}Inria-Asclepios/RPI.git GIT_TAG origin/master)
endif()

set(${proj}_DEPS "")
if (NOT USE_SYSTEM_ITK)
  set(${proj}_DEPS "${${proj}_DEPS};ITK")
endif()

ExternalProject_Add(${proj}
  ${location}
  DEPENDS ${${proj}_DEPS}
  PREFIX ${CMAKE_BINARY_DIR}/External-Projects/${proj}
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/External-Projects/${proj}
  CMAKE_GENERATOR ${cmake_gen}
  CMAKE_ARGS ${cmake_args}
  BUILD_ALWAYS 1
  BINARY_DIR ${CMAKE_BINARY_DIR}/${proj}
  INSTALL_COMMAND ""
  )

ExternalProject_Get_Property(${proj} binary_dir)
set(${proj}_BUILD_DIR ${binary_dir})
set(${proj}_SRC_DIR ${CMAKE_SOURCE_DIR}/External-Projects/${proj})

set(Anima_DEPS "${Anima_DEPS};${proj}")
