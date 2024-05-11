# quickerSMBC Core sources

quickerSMBCSrc =  [
 'core/source/Util/Video.cpp',
 'core/source/Emulation/PPU.cpp',
 'core/source/Emulation/APU.cpp',
 'core/source/Emulation/Controller.cpp',
 'core/source/Configuration.cpp',
 'core/source/SMB/SMBData.cpp',
 'core/source/Main.cpp',
 'core/source/common.cpp'
]

quickerSMBCIncludeDirs = [
 '.',
 'core',
 'core/source',
]

quickerSMBCCompileArgs = [
]

# quickerSMBC Core Configuration

 quickerSMBCDependency = declare_dependency(
  compile_args        : [  quickerSMBCCompileArgs ],
  include_directories : include_directories(quickerSMBCIncludeDirs),
  sources             : [ quickerSMBCSrc ],
  dependencies        : [  
                          dependency('sdl2',  required : true),
                        ]
 )