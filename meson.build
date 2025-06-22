project(
  'ByteWave',
  'c',
  'cpp',
  default_options : [
    'c_std=c11', #May change depending on compiler (clang not fully supported yet)
    'warning_level=1',
  ],
  meson_version : '>=1.1.0'
)


c_compiler = meson.get_compiler(
  'c',
  native : true
)

#ByteWave src files
bw_src_files = []

#ByteWave include directories
bw_inc_dir = [ 'include',
               'src/util/structs', ]

bw_deps = [ ]

bw_linker_args = [ ]

bw_c_compiler_args = [
  '-Wimplicit-fallthrough',
]

if build_machine.system() == 'windows'
  bw_c_compiler_args += '-D_CRT_SECURE_NO_WARNINGS'
  #Windows librarys
  winmm = c_compiler.find_library('winmm')
  ole32 = c_compiler.find_library('ole32')
  uuid = c_compiler.find_library('uuid')
  dsound = c_compiler.find_library('dsound')

  #winmm
  winmm = c_compiler.find_library('winmm')
  bw_deps += winmm

  #dgi
  gdi32 = c_compiler.find_library('gdi32')
  bw_deps += gdi32

  #OpenGL
  opengl = c_compiler.find_library('opengl32')
  bw_deps += opengl

  #Windows utils
  bw_inc_dir += ['include/win-utils']

  #HACK: Gets around some compiler errors
  bw_c_compiler_args += ['-D_MT', '-D_DLL']
  bw_linker_args += ['-nodefaultlibs', '-lmsvcrtd', '-lucrtd', '-lmsvcprt', '-lvcruntime']
endif
if build_machine.system() == 'linux'
  #GL
  gl = c_compiler.find_library('GL')
  bw_deps += gl

  #m
  m = c_compiler.find_library('m')
  bw_deps += m

  #pthread
  pthread = c_compiler.find_library('pthread')
  bw_deps += pthread

  #dl
  dl = c_compiler.find_library('dl')
  bw_deps += dl

  #rt
  rt = c_compiler.find_library('rt')
  bw_deps += rt

  #X11
  X11 = c_compiler.find_library('X11')
  bw_deps += X11
endif

#Host API Options
#General
bw_src_files += [
  'src/hostapi/bw-hostapi.c'
]
bw_inc_dir += [
  'src/hostapi'
]
if get_option('BW_USE_ASIO')
  #ASIO
  bw_asio_src = [
    'lib/ASIOSDK/common/asio.cpp',
    'lib/ASIOSDK/host/pc/asiolist.cpp',
    'lib/ASIOSDK/host/asiodrivers.cpp',
    'src/hostapi/asio/bw-asio.c',
    'src/hostapi/asio/bw-asio-il.cpp',
  ]
  bw_asio_inc = [
    'lib/ASIOSDK/common',
    'lib/ASIOSDK/host/pc',
    'lib/ASIOSDK/host',
    'src/hostapi/asio',
  ]
  message('Using ASIO API')
  bw_c_compiler_args += ['-DBW_ASIO']
  bw_src_files += bw_asio_src
  bw_inc_dir += bw_asio_inc
  if not bw_deps.contains(ole32)
    bw_deps += ole32
  endif
  if not bw_deps.contains(uuid)
    bw_deps += uuid
  endif
endif

#WASAPI
bw_wasapi_src = ['src/hostapi/wasapi/bw-wasapi.c']
bw_wasapi_inc = ['src/hostapi/wasapi']
if get_option('BW_USE_WASAPI')
  message('Using WASAPI')
  bw_c_compiler_args += ['-DBW_WASAPI']
  bw_src_files += bw_wasapi_src
  bw_inc_dir += bw_wasapi_inc
  if not bw_deps.contains(ole32)
    bw_deps += ole32
  endif
  if not bw_deps.contains(uuid)
    bw_deps += uuid
  endif
endif

#DirectSound
bw_ds_src = ['src/hostapi/dsound/bw-dsound.c']
bw_ds_inc = ['src/hostapi/dsound']
if get_option('BW_USE_DIRECTSOUND')
  message('Using DirectSound API')
  bw_c_compiler_args += ['-DBW_DSOUND']
  bw_src_files += bw_ds_src
  bw_inc_dir += bw_ds_inc
  if not bw_deps.contains(dsound)
    bw_deps += dsound
  endif
endif

#Utility
bw_src_files += ['src/util/bw-threads.c',
                 'src/util/bw-config.c',
                 'src/util/bw-types.c',
                 'src/util/bw-conversions.c',
                 'src/util/bw-schedule.c',]

#Data Structures
bw_src_files += ['src/util/structs/bw-dynamic-array.c',
                 'src/util/structs/bw-circ-buf.c',]

#UI
if get_option('BW_CUSTOM_WINDOW')
  bw_c_compiler_args += ['-DBW_CUSTOM_WINDOW']
endif

bw_ui_src_files = [
  'src/ui/bw-ui.c',
  'src/ui/bw-kb.c',
  'src/ui/bw-ui-components.c',
  'src/ui/bw-toggle-cluster.c',
  'src/ui/bw-mouse.c',
  'src/ui/bw-mixer-ui.c',
  'src/ui/bw-window.c',
  'src/ui/bw-popup-win32.c',
]


#Raylib
cwd = meson.current_source_dir()
lib_dir = join_paths(cwd, 'lib/ui')
raylib = c_compiler.find_library('raylib', dirs: lib_dir)
bw_deps += raylib
bw_src_files += bw_ui_src_files

#Debug Logging
bw_src_files += ['src/util/bw-log.c']
if get_option('BW_LOG')
  bw_c_compiler_args += '-DBW_LOG'
else
  if get_option('BW_LOG_ERRORS')
    bw_c_compiler_args += ['-DBW_LOG_ERRORS']
  endif
  if get_option('BW_LOG_FUNCTIONS')
    bw_c_compiler_args += ['-DBW_LOG_FUNCTIONS']
  endif
  if get_option('BW_LOG_GEN')
    bw_c_compiler_args += ['-DBW_LOG_GEN']
  endif
endif

executable(
  #executable name
  'ByteWave',

  #entry point
  'main.c',

  #Source Files
  bw_src_files,

  dependencies : [ bw_deps ],

  #subsystem used, set to 'windows' for a gui app, 'console' otherwise
  win_subsystem : 'console',

  #includes
  include_directories : bw_inc_dir,

  #c compiler args
  c_args : bw_c_compiler_args,

  link_args: bw_linker_args,
)
