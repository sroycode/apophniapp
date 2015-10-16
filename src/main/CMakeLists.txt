include_directories(${CMAKE_CURRENT_BINARY_DIR})

set(APOPHNIAPP_LIB_DEPS
	${APOPHNIAPP_LIB_DEPS}
	apn_http
)

set(APOPHNIAPP_SOURCES
	ApnServer.cc
)


add_executable(apnserver ${APOPHNIAPP_SOURCES})
target_link_libraries(apnserver ${APOPHNIAPP_LIB_DEPS})

install(TARGETS ${APOPHNIAPP_TARGET_LIBS} apnserver
	RUNTIME DESTINATION bin
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
)

