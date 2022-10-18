dependencies = [
    dependency('gtest')
]

includes = [
    '../include'
]

stderror_sources = [
    '../src/stderror.c',
    'stderror-tests.cpp'
]

stderror_tests = executable('stderror-tests', stderror_sources,
                             include_directories: includes,
                             dependencies: dependencies)

test('stderror-tests', stderror_tests)
