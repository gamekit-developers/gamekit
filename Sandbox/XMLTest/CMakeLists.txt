set(SRC Main.cpp )
include_directories(../Utils/Src ../Utils/Src/XML)
link_libraries(Utils UtilsXML)
add_executable(XMLTest ${SRC})
