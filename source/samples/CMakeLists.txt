ADD_EXECUTABLE( sfml-sample  sfml-sample.cpp)
TARGET_LINK_LIBRARIES( sfml-sample ${SFML_LIBRARIES} ${OPENGL_LIBRARY})

ADD_EXECUTABLE( assimp-sfml-sample assimp-sfml-sample.cpp camera.cpp)
TARGET_LINK_LIBRARIES( assimp-sfml-sample ${OPENGL_LIBRARY} ${ASSIMP_LIBRARY_DBG} ${SFML_LIBRARIES})

add_executable( bullet-basic-sample bullet-basic-sample.cpp)
TARGET_LINK_LIBRARIES( bullet-basic-sample ${BULLET_LIBRARIES})

add_executable( openni-sample openni-sample.cpp)
target_link_libraries( openni-sample ${OPENNI_LIBRARY})

add_executable( nite-openni-sample nite-openni-sample.cpp)
target_link_libraries( nite-openni-sample ${OPENNI_LIBRARY} ${NITE_LIBRARY})

add_executable( nite-trackpad-sample nite-trackpad-sample.cpp kbhit.cpp signal_catch.cpp)
target_link_libraries( nite-trackpad-sample ${OPENNI_LIBRARY} ${NITE_LIBRARY} ${OPENGL_LIBRARY} ${GLEW_LIBRARIES} -lglut)

add_executable( glm-sample glm-sample.cpp)
target_link_libraries( glm-sample ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES})

INSTALL( TARGETS sfml-sample DESTINATION bin/samples)
INSTALL( TARGETS assimp-sfml-sample DESTINATION bin/samples)
INSTALL( TARGETS bullet-basic-sample DESTINATION bin/samples)
INSTALL( TARGETS openni-sample DESTINATION bin/samples)
INSTALL( TARGETS nite-openni-sample DESTINATION bin/samples)
INSTALL( TARGETS nite-trackpad-sample DESTINATION bin/samples)
INSTALL( TARGETS glm-sample DESTINATION bin/samples)