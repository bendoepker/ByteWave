project(
  'ByteWave',
  'c',
  'cpp',
  default_options : [
    'cpp_std=manual',
    'warning_level=1',
  ]
)

cpp_compiler = meson.get_compiler(
  'cpp',
  native : true
)

c_compiler = meson.get_compiler(
  'c',
  native : true
)

incdir = include_directories(
  'include/portaudio',
  'include/portaudio/asio',
  'include/ASIOSDK2',
  'include/ASIOSDK2/common',
  'include/ASIOSDK2/host',
  'include/ASIOSDK2/host/pc',
)

asio = [
  'include/ASIOSDK2/common/asio.cpp', 
  'include/ASIOSDK2/host/asiodrivers.cpp', 
  'include/ASIOSDK2/host/pc/asiolist.cpp'
]

portaudio = [
  #removing the line below stopped multiple references of
  #'InterlockedXXX' definitions, if it needs to be added back
  #line 119 of lib/portaudio/pa_asio.cpp needs to be uncommented
  #for it to take effect
  #'lib/portaudio/iasiothiscallresolver.cpp',

  'lib/portaudio/pa_allocation.c',
  'lib/portaudio/pa_converters.c',
  'lib/portaudio/pa_cpuload.c',
  'lib/portaudio/pa_debugprint.c',
  'lib/portaudio/pa_dither.c',
  'lib/portaudio/pa_front.c',
  'lib/portaudio/pa_process.c',
  'lib/portaudio/pa_ringbuffer.c',
  'lib/portaudio/pa_stream.c',
  'lib/portaudio/pa_trace.c',
  'lib/portaudio/pa_win_coinitialize.c',
  'lib/portaudio/pa_win_hostapis.c',
  'lib/portaudio/pa_win_util.c',
  'lib/portaudio/pa_win_version.c',
  'lib/portaudio/pa_win_waveformat.c',
  #'lib/portaudio/pa_x86_plain_converters.c',
  'lib/portaudio/pa_asio.cpp',
]

winmm = cpp_compiler.find_library('winmm')
#ole32 = cpp_compiler.find_library('ole32')

executable(
  #executable name
  'ByteWave',

  #entry point
  'main.cpp',

  #asio dependencies
  asio,

  #portaudio dependencies
  portaudio,

  #windows dependencies
  dependencies : winmm,

  #subsystem used, set to 'windows' for a gui app, 'console' otherwise
  win_subsystem : 'console',

  #includes
  include_directories : incdir,

  #c++ compiler args
  cpp_args : [
    '-fpermissive', #Makes some of the error warning produced by ASIOSDK / PortAudio go away (might break something down the road)
    '-ffast-math', #Increase Floating Point Speed (And break IEEE compliance but thats alright)
  ]
)
