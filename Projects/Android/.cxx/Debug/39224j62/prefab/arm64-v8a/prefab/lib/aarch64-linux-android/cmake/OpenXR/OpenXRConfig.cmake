if(NOT TARGET OpenXR::headers)
add_library(OpenXR::headers INTERFACE IMPORTED)
set_target_properties(OpenXR::headers PROPERTIES
    INTERFACE_INCLUDE_DIRECTORIES "C:/Users/Alex/.gradle/caches/transforms-3/43cd656f9691079a8f1d28783975aa49/transformed/openxr_loader_for_android-1.1.38/prefab/modules/headers/include"
    INTERFACE_LINK_LIBRARIES ""
)
endif()

if(NOT TARGET OpenXR::openxr_loader)
add_library(OpenXR::openxr_loader SHARED IMPORTED)
set_target_properties(OpenXR::openxr_loader PROPERTIES
    IMPORTED_LOCATION "C:/Users/Alex/.gradle/caches/transforms-3/43cd656f9691079a8f1d28783975aa49/transformed/openxr_loader_for_android-1.1.38/prefab/modules/openxr_loader/libs/android.arm64-v8a/libopenxr_loader.so"
    INTERFACE_LINK_LIBRARIES "OpenXR::headers"
)
endif()

