set(USERLIB_NAME user)

set(USERSOURCES
    # main
    TTemplateProcessor.cc
    TTemplateData.cc
    # mwdc
    mwdc/TMWDCTrackingProcessor_crib.cc
)

set(USERHEADERS
    # main
    TTemplateProcessor.h
    TTemplateData.h
    # mwdc
    mwdc/TMWDCTrackingProcessor_crib.h
)

add_library(${USERLIB_NAME} SHARED ${USERSOURCES})

target_include_directories(${USERLIB_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(${USERLIB_NAME}
    PRIVATE
        ${ROOT_LIBRARIES}
        ${YAML_CPP_LIBRARIES}
        artemis::catcore
        artemis::catloop
        artemis::artcont
        artemis::catcmd
        artemis::catmc
        artemis::artdecoder
        artemis::artridf
        artemis::artrdf
        artemis::arthist
        artemis::artparam
        artemis::artshare
        artemis::CAT
        SRIMlib
)

ROOT_GENERATE_DICTIONARY(G__${USERLIB_NAME}
    ${USERHEADERS}
    LINKDEF "linkdef_user.h"
    MODULE ${USERLIB_NAME}
)

set_target_properties(
    ${USERLIB_NAME}
    PROPERTIES
        PUBLIC_HEADER "${USERHEADERS}"
)

install(TARGETS ${USERLIB_NAME}
    EXPORT userTargets
    INCLUDES DESTINATION include
    ARCHIVE DESTINATION lib
    PUBLIC_HEADER DESTINATION include/src
    LIBRARY DESTINATION lib
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lib${USERLIB_NAME}_rdict.pcm
    DESTINATION lib
)
