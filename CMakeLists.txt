cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
#project(artemis-crib)
project(artemis-crib LANGUAGES CXX C Fortran)
enable_language(Fortran)

# ROOT 6 installation
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

# Default installation dir
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/build/install CACHE PATH "..." FORCE)
endif()

# path definitions
set(ARTPATH $ENV{TARTSYS})
find_package(artemis HINTS ${ARTPATH} REQUIRED)
#find_package(SRIMlib HINTS ${SRIMLIB_DIR} REQUIRED)

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO Net Physics Minuit Minuit2 Geom Gui Genetic)
find_package(yaml-cpp REQUIRED HINTS ${ARTEMIS_YAML_CPP_LIB_DIR}/../)

#-fPIC
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

link_directories(${ARTEMIS_GET_LIB_DIR})

add_subdirectory(src-crib)
add_subdirectory(src)



# ===== okawa test =====
#add_executable(main ./elastic2/main.cpp)
#add_subdirectory(./elastic2/src)
#add_subdirectory(./elastic2/kineT)
#set_target_properties(main PROPERTIES LINKER_LANGUAGE Fortran)
#
#target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src-crib)
#target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
#
#target_link_libraries(main ${ROOT_LIBRARIES} ${YAML_CPP_LIBRARIES}
#    artemis::catcmd artemis::catloop artemis::catcore artemis::artcont artemis::catmc
#    artemis::artdecoder artemis::artridf artemis::artrdf artemis::arthist
#    artemis::artparam artemis::artshare artemis::CAT crib user
#    phyExp kine 
#)
#
#INSTALL(TARGETS main
#    RUNTIME DESTINATION bin
#)
## ===== end =====
#
#add_executable(run_artemis run_artemis.cpp)
#target_include_directories(run_artemis PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src-crib)
#target_include_directories(run_artemis PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
#
#target_link_libraries(run_artemis ${ROOT_LIBRARIES} ${YAML_CPP_LIBRARIES}
#    artemis::catcmd artemis::catloop artemis::catcore artemis::artcont artemis::catmc
#    artemis::artdecoder artemis::artridf artemis::artrdf artemis::arthist
#    artemis::artparam artemis::artshare artemis::CAT crib user
#)

#INSTALL(TARGETS run_artemis
#    RUNTIME DESTINATION bin
#)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/.thisartemis-crib.sh.in"
    "${CMAKE_SOURCE_DIR}/thisartemis-crib.sh"
    @ONLY
)

configure_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/.artemislogon.C.in"
    "${CMAKE_SOURCE_DIR}/artemislogon.C"
    @ONLY
)

# make uninstall
add_custom_target("uninstall" COMMENT "Uninstall installed files")
add_custom_command(
    TARGET "uninstall"
    POST_BUILD
    COMMENT "Uninstall files with install_manifest.txt"
    COMMAND xargs rm -vf < install_manifest.txt || echo Nothing in
            install_manifest.txt to be uninstalled!
)
