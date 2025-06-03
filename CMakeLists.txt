# Include from git submodule
idf_component_register(SRCS "src/HTTPServer.cpp" "src/JSONResponse.cpp" "src/QueryURLParser.cpp"
                    INCLUDE_DIRS "include"
                    REQUIRES driver)
