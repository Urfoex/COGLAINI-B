IF( NOT ( ${CMAKE_BINARY_DIR} STREQUAL ${CMAKE_SOURCE_DIR}))
	FILE( GLOB thisDir RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.dae )
	foreach( myFile  ${thisDir})
		CONFIGURE_FILE( ${myFile} ${CMAKE_CURRENT_BINARY_DIR}/ COPYONLY)
	endforeach()
ENDIF()

FILE( GLOB thisDir RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" *.dae )
foreach( myFile  ${thisDir})
	INSTALL( FILES ${myFile} DESTINATION resources/Engine1-Data)
endforeach()
