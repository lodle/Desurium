include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}/code
  ${CMAKE_CURRENT_SOURCE_DIR}/RES
  ${UTIL_BOOTLOADER_INCLUDE_DIRS}
  ${COMMON_INCLUDE_DIRS}
  ${UTIL_INCLUDE_DIRS}
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/RES/dumpgen.rc ${RC_DIR}/dumpgen.rc @ONLY)
file(GLOB Sources code/*.cpp ${RC_DIR}/dumpgen.rc)

add_definitions(-D_WINDOWS -DDONTUSE_PCH)

add_executable(dumpgen WIN32 ${Sources})

if (OFFICIAL_BUILD)
	add_definitions(-DDESURA_OFFICAL_BUILD)
endif()

target_link_libraries(dumpgen
  util_bootloader_s
  util_s
  mdump_s
  ${OFFICIAL_DEP}
)
SetStaticRuntime(dumpgen)

install(TARGETS dumpgen DESTINATION "${BIN_INSTALL_DIR}")

CopyTargetFiles(dumpgen)
SET_PROPERTY(TARGET dumpgen                PROPERTY FOLDER "Executable")