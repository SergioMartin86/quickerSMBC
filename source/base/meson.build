# baseSMBC Core sources

baseSMBCSrc =  [
 'core/source/Util/Video.cpp',
 'core/source/Emulation/PPU.cpp',
 'core/source/Emulation/APU.cpp',
 'core/source/Emulation/MemoryAccess.cpp',
 'core/source/Emulation/Controller.cpp',
 'core/source/Configuration.cpp',
 'core/source/SMB/SMBEngine.cpp',
 'core/source/SMB/SMB.cpp',
 'core/source/SMB/SMBData.cpp',
 'core/source/Main.cpp',
]

baseSMBCIncludeDirs = [
 '.',
 'core',
 'core/source',
]

baseSMBCCompileArgs = [
]

# baseSMBC Core Configuration

 baseSMBCDependency = declare_dependency(
  compile_args        : [  baseSMBCCompileArgs ],
  include_directories : include_directories(baseSMBCIncludeDirs),
  sources             : [ baseSMBCSrc ],
  dependencies        : [  
                          dependency('sdl2',  required : true),
                        ]
 )