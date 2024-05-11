nomalloc = environment({'MALLOC_PERTURB_': '0'})

bash = find_program('bash')
testTimeout = 240

# Tests for copyrighted game roms (only for local testing or own CI runners)
protectedTestSet = [ 
  'world101a'
]

# Tests for open source free roms (for public cloud testing)
openSourceTestSet = [
]

# Creating test set based on whether copyrighted roms are to be used
testSet = openSourceTestSet
if get_option('onlyOpenSource') == false
 testSet += protectedTestSet
endif

# Adding tests to the suite
foreach testFile : testSet
  test(testFile,
       bash,
       workdir : meson.current_source_dir(),
       timeout: testTimeout,
       args : [ 'run_test.sh', baseSMBCTester.path(),  quickerSMBCTester.path(), testFile + '.test', testFile + '.sol' ],
       suite : [ 'smbc' ])
endforeach
