cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "vcpkg toolchain file")

project(OOPProject LANGUAGES CXX)
# 인코딩 옵션 추가 위치
if(MSVC)
    add_compile_options(/utf-8)
endif()
# C++20 표준 설정
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# vcpkg와 연동하여 Google Test 가져오기
find_package(GTest CONFIG REQUIRED)


# 테스트 실행 파일 및 라이브러리 연결
enable_testing()
add_executable(run_tests tests/test_main.cpp)
target_link_libraries(run_tests PRIVATE GTest::gtest_main)

enable_testing()
add_test(NAME MyTest COMMAND run_tests)

# --- Static Analysis 설정 ---

# 1. Clang-Tidy (C++ 현대적 문법 및 품질 검사)
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY 
    ${CLANG_TIDY_EXE}
    "-checks=bugprone-*,clang-analyzer-*,cppcoreguidelines-*" # 메모리 관련 핵심 규칙 포함
    "--warnings-as-errors=*" # 모든 경고를 에러로!
)
    message(STATUS "Clang-Tidy found and enabled")
endif()

# 2. Cppcheck (잠재적 런타임 버그 및 메모리 누수 검사)
find_program(CPPCHECK_EXE NAMES "cppcheck")
if(CPPCHECK_EXE)
    set(CMAKE_CXX_CPPCHECK ${CPPCHECK_EXE} "--enable=all" "--inconclusive" "--force" "--inline-suppr" "--error-exitcode=1" "--suppress=missingIncludeSystem")
    message(STATUS "Cppcheck found and enabled")
endif()


# 실행 파일 생성
add_executable(app src/main.cpp)