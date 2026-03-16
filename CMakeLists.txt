cmake_minimum_required(VERSION 3.20)

set(CMAKE_TOOLCHAIN_FILE "C:/vcpkg/scripts/buildsystems/vcpkg.cmake" CACHE STRING "vcpkg toolchain file")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(OOPProject LANGUAGES CXX)

# C++20 표준 설정
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 인코딩 및 Clang 전용 옵션 (Clang 사용 시 /utf-8 에러 방지 포함)
if(MSVC)
    add_compile_options(/utf-8)
elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    # --- 여기에 커버리지 설정을 추가합니다 ---
    message(STATUS "Clang detected: Enabling Coverage instrumentation")
    add_compile_options(-fprofile-instr-generate -fcoverage-mapping)
    add_link_options(-fprofile-instr-generate -fcoverage-mapping)
endif()

# vcpkg와 연동하여 Google Test 가져오기
find_package(GTest CONFIG REQUIRED)


# # 테스트 실행 파일 및 라이브러리 연결
# enable_testing()



# --- Static Analysis 설정 ---

# 1. Clang-Tidy (C++ 현대적 문법 및 품질 검사)
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
    # [4] 정적 분석 도구 설정 부분
    set(CMAKE_CXX_CLANG_TIDY 
        ${CLANG_TIDY_EXE}
        "-checks=-*,bugprone-*,clang-analyzer-*,cppcoreguidelines-*,performance-*,portability-*,modernize-*,-modernize-use-trailing-return-type" # 맨 끝에 추가
        "--warnings-as-errors=*"
    )
endif()

# 2. Cppcheck (잠재적 런타임 버그 및 메모리 누수 검사)
# --- Cppcheck ---
find_program(CPPCHECK_EXE NAMES "cppcheck")
if(CPPCHECK_EXE)
    set(CMAKE_CXX_CPPCHECK 
        ${CPPCHECK_EXE} 
        "--enable=all" 
        "--inconclusive" 
        "--force" 
        "--inline-suppr"
        "--error-exitcode=1"
        "--suppress=missingIncludeSystem"
    )
endif()

# 실행 파일 생성
add_executable(app src/main.cpp)

add_executable(run_tests tests/test_main.cpp)
target_link_libraries(run_tests PRIVATE GTest::gtest_main)

enable_testing()
add_test(NAME MyTest COMMAND run_tests)