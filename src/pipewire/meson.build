pipewire_headers = [
  'array.h',
  'buffers.h',
  'impl-core.h',
  'impl-client.h',
  'client.h',
  'conf.h',
  'context.h',
  'control.h',
  'core.h',
  'device.h',
  'impl-device.h',
  'data-loop.h',
  'factory.h',
  'impl-factory.h',
  'filter.h',
  'global.h',
  'keys.h',
  'impl.h',
  'i18n.h',
  'impl-link.h',
  'link.h',
  'log.h',
  'loop.h',
  'main-loop.h',
  'map.h',
  'mem.h',
  'impl-metadata.h',
  'impl-module.h',
  'module.h',
  'impl-node.h',
  'node.h',
  'permission.h',
  'pipewire.h',
  'impl-port.h',
  'port.h',
  'properties.h',
  'protocol.h',
  'proxy.h',
  'resource.h',
  'stream.h',
  'thread.h',
  'thread-loop.h',
  'type.h',
  'utils.h',
  'work-queue.h',
]

pipewire_sources = [
  'buffers.c',
  'impl-core.c',
  'impl-client.c',
  'conf.c',
  'context.c',
  'control.c',
  'core.c',
  'data-loop.c',
  'impl-device.c',
  'filter.c',
  'global.c',
  'introspect.c',
  'impl-link.c',
  'log.c',
  'loop.c',
  'main-loop.c',
  'mem.c',
  'impl-module.c',
  'impl-node.c',
  'impl-factory.c',
  'impl-metadata.c',
  'pipewire.c',
  'impl-port.c',
  'properties.c',
  'protocol.c',
  'proxy.c',
  'resource.c',
  'settings.c',
  'stream.c',
  'thread.c',
  'thread-loop.c',
  'utils.c',
  'work-queue.c',
]

configure_file(input : 'version.h.in',
  output : 'version.h',
  install_dir : get_option('includedir') / pipewire_headers_dir,
  configuration : versiondata)


install_headers(pipewire_headers, subdir : pipewire_headers_dir)

libpipewire_c_args = [
  '-DOLD_MEDIA_SESSION_WORKAROUND=1'
]

if host_machine.system() != 'freebsd' and host_machine.system() != 'midnightbsd'
  libpipewire_c_args += [
    '-D_POSIX_C_SOURCE'
  ]
endif

libpipewire = shared_library(pipewire_name, pipewire_sources,
  version : libversion,
  soversion : soversion,
  c_args : libpipewire_c_args,
  include_directories : [pipewire_inc, configinc, includes_inc],
  install : true,
  dependencies : [spa_dep, dl_lib, mathlib, pthread_lib, libintl_dep, atomic_dep, ],
)

pipewire_dep = declare_dependency(link_with : libpipewire,
  include_directories : [pipewire_inc, configinc],
  dependencies : [pthread_lib, atomic_dep, spa_dep],
  variables : {
    'moduledir' : meson.current_build_dir() / '..' / 'modules',
    'confdatadir' : meson.current_build_dir() / '..' / 'daemon',
  }
)

pkgconfig.generate(libpipewire,
  filebase : 'lib@0@'.format(pipewire_name),
  requires : ['lib@0@'.format(spa_name)],
  name : 'libpipewire',
  subdirs : pipewire_name,
  description : 'PipeWire Interface',
  version : pipewire_version,
  extra_cflags : '-D_REENTRANT',
  variables : ['moduledir=${libdir}/@0@'.format(pipewire_name)],
  uninstalled_variables : [
    'moduledir=${prefix}/src/modules',
    'confdatadir=${prefix}/src/daemon',
  ],
)

meson.override_dependency('lib@0@'.format(pipewire_name), pipewire_dep)

subdir('extensions')
