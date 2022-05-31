project('lastfmlib', 'c', 'cpp',
  version : '1.0.0',
  default_options : ['warning_level=3', 'cpp_std=c++17'],
)

curl_dep = dependency('libcurl')
thread_dep = dependency('threads')
gtest_dep = dependency('gtest', required: get_option('tests'))
gmock_dep = dependency('gmock', required: get_option('tests'))

cdata = configuration_data()
cdata.set('HAVE_CONFIG_H', true)
cdata.set('ENABLE_DEBUG', get_option('buildtype').startswith('debug'))
cdata.set('ENABLE_LOGGING', host_machine.system() != 'windows')
cfile = configure_file(
  output : 'config.h',
  configuration : cdata,
)

lastfm_inc = include_directories('.')

lastfmlib = library('lastfmlib',
  'lastfmlib/nowplayinginfo.cpp',
  'lastfmlib/urlclient.cpp',
  'lastfmlib/submissioninfocollection.cpp',
  'lastfmlib/lastfmscrobbler.cpp',
  'lastfmlib/submissioninfo.cpp',
  'lastfmlib/lastfmclient.cpp',
  'lastfmlib/md5/md5.c',
  'lastfmlib/utils/log.cpp',
  'lastfmlib/utils/stringoperations.cpp',
   dependencies : [ curl_dep, thread_dep ],
   include_directories: lastfm_inc,
   install : true,
)

pkg = import('pkgconfig')
pkg.generate(lastfmlib, name: 'liblastfm')

install_headers(
  'lastfmlib/lastfmscrobbler.h',
  'lastfmlib/lastfmtypes.h',
  'lastfmlib/lastfmclient.h',
  'lastfmlib/nowplayinginfo.h',
  'lastfmlib/submissioninfocollection.h',
  'lastfmlib/urlclient.h',
  'lastfmlib/submissioninfo.h',
  'lastfmlib/lastfmexceptions.h',
  subdir : 'lastfmlib',
)

install_headers(
  'lastfmlib/utils/log.h',
  subdir : 'lastfmlib/utils'
)

if gtest_dep.found() and gmock_dep.found()
testrunner = executable(
    'testlastfmclientmock',
    'lastfmlib/unittest/lastfmclientmock.cpp',
    'lastfmlib/unittest/lastfmclienttest.cpp',
    'lastfmlib/unittest/lastfmscrobblertest.cpp',
    'lastfmlib/unittest/nowplayinginfotest.cpp',
    'lastfmlib/unittest/stringoperationstest.cpp',
    'lastfmlib/unittest/submissioninfocollectiontest.cpp',
    'lastfmlib/unittest/submissioninfotest.cpp',
    'lastfmlib/unittest/testrunner.cpp',
    dependencies: [ gmock_dep, gtest_dep ],
    link_with: lastfmlib,
  )

  test('testrunner', testrunner)
endif

lastfm_dep = declare_dependency(
  include_directories : lastfm_inc,
  link_with : lastfmlib,
)
