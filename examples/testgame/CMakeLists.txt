add_executable(TestGame
    main.cpp
)

target_link_libraries(TestGame PRIVATE BlueSapphire)

set_target_properties(TestGame PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests
)