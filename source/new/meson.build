# newSMBC Core sources

newSMBCSrc =  [
]

newSMBCIncludeDirs = [
 '.',
]

newSMBCCompileArgs = [
]

# newSMBC Core Configuration

 newSMBCDependency = declare_dependency(
  compile_args        : [ newSMBCCompileArgs ],
  include_directories : include_directories(newSMBCIncludeDirs),
  sources             : [ newSMBCSrc ],
  dependencies        : [  
                          dependency('sdl2',  required : true),
                        ]
 )
