set (proj RPI)

set (cmake_args
  -DBUILD_SHARED_LIBS:BOOL=ON
  -DRPI_BUILD_EXAMPLES:BOOL=OFF
  -DITK_DIR:PATH=${ITK_BUILD_DIR}
  -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
  )

set (location "")
if (NOT DEFINED ${proj}_SRC_DIR)
  set(location GIT_REPOSITORY git@github.com:Inria-Asclepios/RPI.git)
endif()

ExternalProject_Add(${proj}
  ${location}
  DEPENDS ITK
  PREFIX ${CMAKE_BINARY_DIR}/${proj}
  SOURCE_DIR ${CMAKE_SOURCE_DIR}/Projects/${proj}
  CMAKE_ARGS ${cmake_args}
  BUILD_IN_SOURCE 0
  BINARY_DIR ${CMAKE_BINARY_DIR}/${proj}
  UPDATE_COMMAND ""
  INSTALL_COMMAND ""
  )

ExternalProject_Get_Property(${proj} binary_dir)
set(${proj}_BUILD_DIR ${binary_dir})
set(${proj}_SRC_DIR ${CMAKE_SOURCE_DIR}/Projects/${proj})