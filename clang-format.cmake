# Gets all the files and makes a target for clang-format 

file(GLOB_RECURSE ALL_SOURCE_FILES *.cpp *.h *.hpp *.ipp)

add_custom_target(
    clangformat 
    COMMAND clang-format 
    -style=file
    -i
    ${ALL_SOURCE_FILES}
)
