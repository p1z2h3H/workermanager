#!/usr/bin/env bash
# 一键编译脚本：在项目根目录执行 bash build.sh
set -e

CXX="${CXX:-g++}"
CXXFLAGS="${CXXFLAGS:--std=c++17 -Wall -Wextra -O2}"
OUT="${OUT:-empSys}"

SOURCES=(
    main.cpp
    workerManager.cpp
    employee.cpp
    manager.cpp
    boss.cpp
)

echo ">>> 使用编译器: $($CXX --version | head -1)"
echo ">>> 编译选项: $CXXFLAGS"
echo ">>> 输出文件: $OUT"

"$CXX" $CXXFLAGS -o "$OUT" "${SOURCES[@]}"

echo ">>> 编译成功，运行: ./$OUT"
