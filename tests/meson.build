nomalloc = environment({'MALLOC_PERTURB_': '0'})

bash = find_program('bash')
testTimeout = 240

testSet = [ 
  'world101a'
]

# Adding tests to the suite
foreach testFile : testSet
  test(testFile,
       bash,
       workdir : meson.current_source_dir(),
       timeout: testTimeout,
       args : [ 'run_test.sh', baseSMBCTester.path(),  quickerSMBCTester.path(), testFile + '.test', testFile + '.sol' ],
       suite : [ 'smbc' ])
endforeach
