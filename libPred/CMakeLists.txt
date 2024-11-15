cmake_minimum_required(VERSION 3.5)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
IF (WIN32)
    # set stuff for windows
ELSE()
    # set stuff for other systems
ENDIF()

file(GLOB motionpred_SRC
     "src/*.cpp"
)

add_library(motionPredictionVC SHARED 
${motionpred_SRC}
)
# install(FILES weightsOrientation.txt DESTINATION weights)
file(COPY weightsOrientation.txt DESTINATION ${CMAKE_BINARY_DIR})
file(COPY weightsTranslation.txt DESTINATION ${CMAKE_BINARY_DIR})
file(COPY weightsOrientation.txt DESTINATION ${CMAKE_BINARY_DIR}/Debug)
file(COPY weightsTranslation.txt DESTINATION ${CMAKE_BINARY_DIR}/Debug)
target_include_directories(motionPredictionVC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(motionPredictionVC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/external)