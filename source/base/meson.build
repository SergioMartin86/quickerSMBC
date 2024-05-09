# baseSMBC Core sources

baseSMBCSrc =  [
]

baseSMBCIncludeDirs = [
 '.',
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