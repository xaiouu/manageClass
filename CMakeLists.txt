cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(managerClass)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

# 添加 FLTK 的路径（替换为你的实际安装路径）
set(FLTK_INCLUDE_DIR "D:/fltk-install/include")
set(FLTK_LIBRARY_DIR "D:/fltk-install/lib")

# 手动设置 FLTK 库
set(FLTK_LIBRARIES
    "${FLTK_LIB_DIR}/libfltk.dll.a"
    "${FLTK_LIB_DIR}/libfltk_forms.dll.a"
    "${FLTK_LIB_DIR}/libfltk_images.dll.a"
    "${FLTK_LIB_DIR}/libfltk_gl.dll.a"
)

# MinGW 需要的额外库
set(EXTRA_LIBS 
    -lgdi32
    -lole32
    -luuid
    -lcomctl32
    -lws2_32        # 添加 Windows Socket 库
    -lgdiplus       # 添加 GDI+ 库
    -lcomdlg32
    -loleaut32
    -lshell32
)

add_subdirectory(src)



