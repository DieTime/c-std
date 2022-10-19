dependencies = [
    dependency('gtest')
]

includes = [
    '../include'
]

std_error_sources = [
    '../src/std-error.c',
    'std-error-tests.cpp'
]

std_error_tests = executable('std-error-tests', std_error_sources,
                             include_directories: includes,
                             dependencies: dependencies)

test('std-error-tests', std_error_tests)
