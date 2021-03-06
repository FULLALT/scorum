file(GLOB_RECURSE HEADERS "include/scorum/blockchain_history/*.hpp")

add_library( scorum_blockchain_history
             blockchain_history_plugin.cpp
             account_history_api.cpp
             blockchain_history_api.cpp
             schema/applied_operation.cpp
             devcommittee_history_api.cpp
           )

target_link_libraries( scorum_blockchain_history
                       scorum_chain
                       scorum_account_identity
                       scorum_protocol
                       scorum_app
                       scorum_common_api)
target_include_directories( scorum_blockchain_history
                            PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

add_custom_target( scorum_blockchain_history_manifest SOURCES plugin.json)

install( TARGETS
   scorum_blockchain_history

   RUNTIME DESTINATION bin
   LIBRARY DESTINATION lib
   ARCHIVE DESTINATION lib
)
