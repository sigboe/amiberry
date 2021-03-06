cmake_minimum_required(VERSION 3.0)
set(CMAKE_VERBOSE_MAKEFILE OFF)

set(VERSION_MAJOR 2)
set(VERSION_MINOR 24)

if(BUILD_PSP2)
if (NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    if (DEFINED ENV{VITASDK})
        set(CMAKE_TOOLCHAIN_FILE "$ENV{VITASDK}/share/vita.toolchain.cmake" CACHE PATH "toolchain file")
    else ()
        message(FATAL_ERROR "Please define VITASDK to point to your SDK path!")
    endif ()
endif ()
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BUILD_DEBUG true CACHE BOOL "Build with psp2shell support")
endif()
endif(BUILD_PSP2)

if(BUILD_NX)
    set(CMAKE_SYSTEM_NAME "Generic")
    set(DEVKITPRO $ENV{DEVKITPRO})
    set(CMAKE_SYSTEM_PROCESSOR "armv8-a")
    set(CMAKE_C_COMPILER "${DEVKITPRO}/devkitA64/bin/aarch64-none-elf-gcc")
    set(CMAKE_CXX_COMPILER "${DEVKITPRO}/devkitA64/bin/aarch64-none-elf-g++")
    set(CMAKE_ASM_COMPILER "${DEVKITPRO}/devkitA64/bin/aarch64-none-elf-as")
    set(CMAKE_AR "${DEVKITPRO}/devkitA64/bin/aarch64-none-elf-gcc-ar" CACHE STRING "")
    set(CMAKE_RANLIB "${DEVKITPRO}/devkitA64/bin/aarch64-none-elf-gcc-ranlib" CACHE STRING "")
    set(CMAKE_FIND_ROOT_PATH ${DEVKITPRO} ${DEVKITPRO}/devkitA64 ${DEVKITPRO}/libnx ${DEVKITPRO}/portlibs/switch)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
    set(BUILD_SHARED_LIBS OFF CACHE INTERNAL "Shared libs not available")
    link_directories(${DEVKITPRO}/libnx/lib ${DEVKITPRO}/portlibs/switch/lib)
endif(BUILD_NX)

execute_process(COMMAND "date" "+%Y.%m.%d"
        OUTPUT_VARIABLE BUILD_DATE OUTPUT_STRIP_TRAILING_WHITESPACE)

project(amiberry)

if(BUILD_PSP2)
  enable_language(ASM)

  include("${VITASDK}/share/vita.cmake" REQUIRED)
endif(BUILD_PSP2)

set(TITLEID "AMIBERRY00")

set(SRC_FILES
        src/osdep/vkbd/vkbd.cpp
        src/akiko.cpp
        src/ar.cpp
        src/aros.rom.cpp
        src/audio.cpp
        src/autoconf.cpp
        src/blitfunc.cpp
        src/blittable.cpp
        src/blitter.cpp
        src/blkdev.cpp
        src/blkdev_cdimage.cpp
        src/bsdsocket.cpp
        src/calc.cpp
        src/cd32_fmv.cpp
        src/cd32_fmv_genlock.cpp
        src/cdrom.cpp
        src/cfgfile.cpp
        src/cia.cpp
        src/crc32.cpp
        src/custom.cpp
        src/def_icons.cpp
        src/devices.cpp
        src/disk.cpp
        src/diskutil.cpp
        src/dlopen.cpp
        src/drawing.cpp
        src/events.cpp
        src/expansion.cpp
        src/fdi2raw.cpp
        src/filesys.cpp
        src/flashrom.cpp
        src/fpp.cpp
        src/fsdb.cpp
        src/fsdb_unix.cpp
        src/fsusage.cpp
        src/gayle.cpp
        src/gfxboard.cpp
        src/gfxutil.cpp
        src/hardfile.cpp
        src/hrtmon.rom.cpp
        src/ide.cpp
        src/inputdevice.cpp
        src/keybuf.cpp
        src/main.cpp
        src/memory.cpp
        src/native2amiga.cpp
        src/rommgr.cpp
        src/rtc.cpp
        src/savestate.cpp
        src/scsi.cpp
        src/statusline.cpp
        src/traps.cpp
        src/uaelib.cpp
        src/uaeresource.cpp
        src/zfile.cpp
        src/zfile_archive.cpp
        src/archivers/7z/7zAlloc.cpp
        src/archivers/7z/7zBuf.cpp
        src/archivers/7z/7zCrc.cpp
        src/archivers/7z/7zCrcOpt.cpp
        src/archivers/7z/7zDec.cpp
        src/archivers/7z/7zIn.cpp
        src/archivers/7z/7zStream.cpp
        src/archivers/7z/Bcj2.cpp
        src/archivers/7z/Bra.cpp
        src/archivers/7z/Bra86.cpp
        src/archivers/7z/BraIA64.c
        src/archivers/7z/Delta.c
        src/archivers/7z/LzmaDec.cpp
        src/archivers/7z/Lzma2Dec.cpp
        src/archivers/7z/Sha256.c
        src/archivers/7z/Xz.cpp
        src/archivers/7z/XzCrc64.c
        src/archivers/7z/XzDec.c
        src/archivers/dms/crc_csum.cpp
        src/archivers/dms/getbits.cpp
        src/archivers/dms/maketbl.cpp
        src/archivers/dms/pfile.cpp
        src/archivers/dms/tables.cpp
        src/archivers/dms/u_deep.cpp
        src/archivers/dms/u_heavy.cpp
        src/archivers/dms/u_init.cpp
        src/archivers/dms/u_medium.cpp
        src/archivers/dms/u_quick.cpp
        src/archivers/dms/u_rle.cpp
        src/archivers/lha/crcio.cpp
        src/archivers/lha/dhuf.cpp
        src/archivers/lha/header.cpp
        src/archivers/lha/huf.cpp
        src/archivers/lha/larc.cpp
        src/archivers/lha/lhamaketbl.cpp
        src/archivers/lha/lharc.cpp
        src/archivers/lha/shuf.cpp
        src/archivers/lha/shuf.cpp
        src/archivers/lha/slide.cpp
        src/archivers/lha/uae_lha.cpp
        src/archivers/lha/util.cpp
        src/archivers/lzx/unlzx.cpp
        src/archivers/mp2/kjmp2.cpp
        src/archivers/wrp/warp.cpp
        src/archivers/zip/unzip.cpp
        src/caps/caps_win32.cpp
        src/machdep/support.cpp
        src/osdep/bsdsocket_host.cpp
        src/osdep/cda_play.cpp
        src/osdep/charset.cpp
        src/osdep/fsdb_host.cpp
        src/osdep/amiberry_hardfile.cpp
        src/osdep/keyboard.cpp
        src/osdep/mp3decoder.cpp
        src/osdep/picasso96.cpp
        src/osdep/writelog.cpp
        src/osdep/amiberry.cpp
        src/osdep/amiberry_filesys.cpp
        src/osdep/amiberry_input.cpp
        src/osdep/amiberry_gfx.cpp
        src/osdep/amiberry_gui.cpp
        src/osdep/amiberry_rp9.cpp
        src/osdep/amiberry_mem.cpp
        src/osdep/amiberry_whdbooter.cpp
        src/osdep/sigsegv_handler.cpp
        src/sounddep/sound.cpp
        src/osdep/gui/UaeRadioButton.cpp
        src/osdep/gui/UaeDropDown.cpp
        src/osdep/gui/UaeCheckBox.cpp
        src/osdep/gui/UaeListBox.cpp
        src/osdep/gui/InGameMessage.cpp
        src/osdep/gui/SelectorEntry.cpp
        src/osdep/gui/ShowHelp.cpp
        src/osdep/gui/ShowMessage.cpp
        src/osdep/gui/SelectFolder.cpp
        src/osdep/gui/SelectFile.cpp
        src/osdep/gui/CreateFilesysHardfile.cpp
        src/osdep/gui/EditFilesysVirtual.cpp
        src/osdep/gui/EditFilesysHardfile.cpp
        src/osdep/gui/PanelAbout.cpp
        src/osdep/gui/PanelPaths.cpp
        src/osdep/gui/PanelQuickstart.cpp
        src/osdep/gui/PanelConfig.cpp
        src/osdep/gui/PanelCPU.cpp
        src/osdep/gui/PanelChipset.cpp
        src/osdep/gui/PanelCustom.cpp
        src/osdep/gui/PanelROM.cpp
        src/osdep/gui/PanelRAM.cpp
        src/osdep/gui/PanelFloppy.cpp
        src/osdep/gui/PanelHD.cpp
        src/osdep/gui/PanelInput.cpp
        src/osdep/gui/PanelDisplay.cpp
        src/osdep/gui/PanelSound.cpp
        src/osdep/gui/PanelMisc.cpp
        src/osdep/gui/PanelSavestate.cpp
        src/osdep/gui/main_window.cpp
        src/osdep/gui/Navigation.cpp
        src/newcpu.cpp
        src/newcpu_common.cpp
        src/readcpu.cpp
        src/cpudefs.cpp
        src/cpustbl.cpp
        src/cpuemu_0.cpp
        src/cpuemu_4.cpp
        src/cpuemu_11.cpp
        src/cpuemu_40.cpp
        src/cpuemu_44.cpp
        src/jit/compemu.cpp
        src/jit/compstbl.cpp
        src/jit/compemu_fpp.cpp
        src/jit/compemu_support.cpp
)

include_directories(
        ../common/include
        include
        src
        src/osdep
        src/osdep/psp2
        src/osdep/switch
        src/osdep/vkbd
        src/include
        src/threaddep
        src/archivers
)

if(BUILD_PSP2)
  set(PSP2_SRC_FILES
    src/osdep/psp2/psp2_shader.cpp
    src/osdep/psp2/psp2-dirent.c
    src/osdep/psp2/psp2_touch.c
    src/osdep/neon_helper.s
  )  
  set(SRC_FILES ${PSP2_SRC_FILES} ${SRC_FILES})

  include_directories(
    ${VITASDK}/arm-vita-eabi/include/SDL2
  )

  set(FLAGS
          -Wno-write-strings
          -DVERSION=\"${BUILD_DATE}\"
          -DAMIBERRY -D__PSP2__ -DUSE_SDL2
          -DGCCCONSTFUNC="__attribute__\(\(const\)\)" -DUSE_UNDERSCORE
          -DSHM_SUPPORT_LINKS=0 -DUNALIGNED_PROFITABLE -DOPTIMIZED_FLAGS -DOS_WITHOUT_MEMORY_MANAGEMENT
          -DROM_PATH_PREFIX=\"ux0:/data/amiberry/roms/\" -DDATA_PREFIX=\"app0:/data/\" -DSAVE_PREFIX=\"ux0:/data/amiberry/saves/\"
          -DUSE_AMIBERRY_VKBD
          -DUSE_ARMV7
          -DARMV6_ASSEMBLY
          -DARMV6T2
          -DUSE_ARMNEON
          -DARM_HAS_DIV
          -marm -mfpu=neon -mcpu=cortex-a9 -march=armv7-a -mfloat-abi=hard -ffast-math
          -fno-asynchronous-unwind-tables -funroll-loops
          -mword-relocations -fno-unwind-tables -fno-optimize-sibling-calls
          -mvectorize-with-neon-quad -funsafe-math-optimizations
          -mlittle-endian -munaligned-access
  )

  set(LDFLAGS
          SDL2 SDL2_image SDL2_ttf guisan FLAC mpg123 dl mpeg2convert mpeg2
          ${CMAKE_SOURCE_DIR}/src/osdep/psp2/vita2d_fbo/lib/libvita2d.a
          ${CMAKE_SOURCE_DIR}/src/osdep/psp2/vita-shader-collection/lib/libvitashaders.a
          ${CMAKE_SOURCE_DIR}/guisan-dev/lib/libguisan.a
          SceSysmodule_stub SceDisplay_stub SceGxm_stub
          SceCtrl_stub ScePgf_stub ScePower_stub SceCommonDialog_stub
          SceAudio_stub SceShellSvc_stub SceHid_stub SceTouch_stub png jpeg z m c
  )

  if (BUILD_DEBUG)
      list(APPEND FLAGS -DDEBUG -DDEBUG_AMIBERRY -DUAE_CONSOLE)
      list(APPEND LDFLAGS psp2shell SceSysmodule_stub SceNetCtl_stub
              SceNet_stub SceAppUtil_stub SceAppMgr_stub
      )
      set(UNSAFE_FLAG UNSAFE)
  else ()
      list(APPEND FLAGS -O3)
  endif (BUILD_DEBUG)

  add_executable(${PROJECT_NAME} ${SRC_FILES})
  target_compile_options(${PROJECT_NAME} PRIVATE ${FLAGS})
  target_link_libraries(${PROJECT_NAME} ${LDFLAGS})

  vita_create_self(${PROJECT_NAME}.self ${PROJECT_NAME} ${UNSAFE_FLAG})
  add_custom_target(${PROJECT_NAME}.vpk
          DEPENDS ${PROJECT_NAME}.self
          COMMAND rm -rf vpk && mkdir -p vpk/sce_sys
          COMMAND ${VITASDK}/bin/vita-mksfoex -s TITLE_ID=${TITLEID} "${PROJECT_NAME}" vpk/sce_sys/param.sfo
          COMMAND cp ${PROJECT_NAME}.self vpk/eboot.bin
          COMMAND cp -r ${CMAKE_SOURCE_DIR}/psp2data/* vpk
          COMMAND cd vpk && zip -r ../amiberry_${VERSION_MAJOR}.${VERSION_MINOR}.vpk . && cd ..
  )
endif(BUILD_PSP2)

if(BUILD_NX)
  set(NX_SRC_FILES
    src/osdep/switch/switch_touch.c
    src/osdep/switch/sdl2_to_sdl1.cpp
  )
  set(SRC_FILES ${NX_SRC_FILES} ${SRC_FILES})

  include_directories(
    ${DEVKITPRO}/portlibs/switch/include/SDL2
    ${DEVKITPRO}/libnx/include
    ${DEVKITPRO}/portlibs/switch/include
  )
  
  add_definitions(
    -DVERSION=\"${BUILD_DATE}\"
    -DGP2X
    -DPANDORA
    -D__SWITCH__
    -D__64BIT__
    -DUSE_AUTOCONFIG
    -DUSE_ZFILE
    -DUSE_FAME_CORE
    -DWITH_TESTMODE
    -DUSE_SDL
    -DUSE_SDL2
    -DGCCCONSTFUNC=\"__attribute__\(\(const\)\)\"
    -DUSE_UNDERSCORE
    -DSHM_SUPPORT_LINKS=0
    -DUNALIGNED_PROFITABLE
    -DOPTIMIZED_FLAGS
    -DOS_WITHOUT_MEMORY_MANAGEMENT
    -DROM_PATH_PREFIX=\"./roms/\"
    -DDATA_PREFIX=\"./data/\"
    -DSAVE_PREFIX=\"./saves/\"
    -DUSE_AMIBERRY_VKBD
  )

  set(FLAGS
    -Wno-write-strings
    -march=armv8-a
    -mtune=cortex-a57
    -mtp=soft
#-fPIE is neccessary to prevent crash on startup
    -fPIE
    -O1
    -ffast-math
    -fno-asynchronous-unwind-tables
    -funroll-loops
    -fno-unwind-tables
    -fno-optimize-sibling-calls
    -funsafe-math-optimizations
    -mlittle-endian
    -ffunction-sections
    -fno-rtti
    -fno-exceptions
#    -fomit-frame-pointer
#    -fno-strict-aliasing
#    -fexpensive-optimizations		
#    -fweb
#    -frename-registers
#    -fomit-frame-pointer		
#    -falign-functions=32
#    -finline
#    -finline-functions
#    -fno-builtin
  )
  
  set(LIBS
    SDL2 
    SDL2_image
    EGL
    GLESv2
    glapi
    drm_nouveau
    nx
    png
    jpeg
    z
    m
    c
    stdc++
  )

  foreach(flag ${FLAGS})
    set(FLAGS_GENERAL "${FLAGS_GENERAL} ${flag}")
  endforeach(flag ${FLAGS})

  #set(CMAKE_C_FLAGS "${FLAGS_GENERAL}" CACHE STRING "C Flags" FORCE)
  #set(CMAKE_CXX_FLAGS "${FLAGS_GENERAL}" CACHE STRING "C++ Flags" FORCE)

  set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${FLAGS_GENERAL} -specs=${DEVKITPRO}/libnx/switch.specs --verbose")
  
  add_executable(${PROJECT_NAME}.elf ${SRC_FILES})
  target_compile_options(${PROJECT_NAME}.elf PRIVATE ${FLAGS})
  target_link_libraries(${PROJECT_NAME}.elf PRIVATE ${LIBS})
  add_custom_target(${PROJECT_NAME}.nro
    DEPENDS ${PROJECT_NAME}.elf
    COMMAND ${DEVKITPRO}/tools/bin/nacptool --create "${PROJECT_NAME}" "midwan" "${VERSION_MAJOR}.${VERSION_MINOR}" ${PROJECT_NAME}.nacp
    COMMAND ${DEVKITPRO}/tools/bin/elf2nro ${PROJECT_NAME}.elf ${PROJECT_NAME}.nro --icon=${CMAKE_SOURCE_DIR}/switchdata/icon0.jpg --nacp=${PROJECT_NAME}.nacp
  )
  add_custom_target(${PROJECT_NAME}_switch.zip
    DEPENDS ${PROJECT_NAME}.nro
    COMMAND rm -rf ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/data
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/roms
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/kickstarts
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/saves
    COMMAND mkdir -p ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/conf
    COMMAND cp -f ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.nro ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/${PROJECT_NAME}.nro
    COMMAND cp -r ${CMAKE_SOURCE_DIR}/switchdata/data/* ${CMAKE_BINARY_DIR}/release/${PROJECT_NAME}/data/
    COMMAND cd ${CMAKE_BINARY_DIR}/release && zip -r ../${PROJECT_NAME}_${VERSION_MAJOR}.${VERSION_MINOR}_switch.zip ${PROJECT_NAME} && cd ${CMAKE_BINARY_DIR}
  )
endif(BUILD_NX)


