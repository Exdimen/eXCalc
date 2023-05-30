# Automatically generated by scripts/boost/generate-ports.ps1

vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO boostorg/multi_array
    REF boost-1.80.0
    SHA512 5e66cd038dd9d86a04b1631ba11245e0fa6742ffa73b12a2b09093891f0c5ebb53844cc0c23acaece067dceea721a1f003360bc90c632e2eab90a5446359123a
    HEAD_REF master
)

include(${CURRENT_INSTALLED_DIR}/share/boost-vcpkg-helpers/boost-modular-headers.cmake)
boost_modular_headers(SOURCE_PATH ${SOURCE_PATH})