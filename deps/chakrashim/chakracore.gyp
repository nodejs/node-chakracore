{
  'variables': {
    'target_arch%': 'ia32',
    'library%': 'static_library',   # build chakracore as static library or dll
    'component%': 'static_library', # link crt statically or dynamically
    'chakra_dir%': 'core',
    'msvs_windows_target_platform_version_prop': '',
    'icu_args%': '',
    'icu_include_path%': '',
    'icu_lib_path%': '',
    'linker_start_group%': '',
    'linker_end_group%': '',
    'chakra_libs_absolute%': '',

    # xplat (non-win32) only
    'chakra_config': 'Release',     # Debug, Release

    'conditions': [
      ['target_arch=="ia32"', { 'Platform': 'x86' }],
      ['target_arch=="x64"', { 'Platform': 'x64' }],
      ['target_arch=="arm"', {
        'Platform': 'arm',
        'msvs_windows_target_platform_version_prop':
          '/p:WindowsTargetPlatformVersion=$(WindowsTargetPlatformVersion)',
      }],
      ['OS=="mac"', {
        'icu_lib_path': '/usr/local/opt/icu4c/lib', # todo: make icu path customizable
        'icu_include_path': '/usr/local/opt/icu4c/include'
      }],

      # xplat (non-win32) only
      ['chakra_config=="Debug"', {
        'chakra_build_flags': [ '-d' ],
      }, {
        'chakra_build_flags': [],
      }],
    ],
  },

  'targets': [
    {
      'target_name': 'chakracore',
      'toolsets': ['host'],
      'type': 'none',

      'variables': {
        'chakracore_header': [
          '<(chakra_dir)/lib/Common/ChakraCoreVersion.h',
          '<(chakra_dir)/lib/Jsrt/ChakraCore.h',
          '<(chakra_dir)/lib/Jsrt/ChakraCommon.h',
          '<(chakra_dir)/lib/Jsrt/ChakraCommonWindows.h',
          '<(chakra_dir)/lib/Jsrt/ChakraDebug.h',
        ],

        'chakracore_win_bin_dir':
          '<(chakra_dir)/build/vcbuild/bin/<(Platform)_$(ConfigurationName)',
        'xplat_dir': '<(chakra_dir)/out/<(chakra_config)',
        'chakra_libs_absolute': '<(PRODUCT_DIR)/../../deps/chakrashim/<(xplat_dir)',

        'conditions': [
          ['OS=="win"', {
            'chakracore_input': '<(chakra_dir)/build/Chakra.Core.sln',
            'chakracore_binaries': [
              '<(chakracore_win_bin_dir)/chakracore.dll',
              '<(chakracore_win_bin_dir)/chakracore.pdb',
              '<(chakracore_win_bin_dir)/chakracore.lib',
            ]
          }],
          ['OS in "linux android"', {
            'chakracore_input': '<(chakra_dir)/build.sh',
            'chakracore_binaries': [
              '<(chakra_libs_absolute)/lib/libChakraCoreStatic.a',
            ],
            'linker_start_group': '-Wl,--start-group',
            'linker_end_group': [
              '-Wl,--end-group',
              '-lgcc_s',
              '-licuuc',
            ]
          }],
          ['OS=="mac"', {
            'chakracore_input': '<(chakra_dir)/build.sh',
            'chakracore_binaries': [
              '<(chakra_libs_absolute)/lib/libChakraCoreStatic.a',
            ],
            'icu_args': '--icu=<(icu_include_path)',
            'linker_start_group': '-Wl,-force_load',
            'linker_end_group': [
              '<(icu_lib_path)/libicuuc.a',
              '<(icu_lib_path)/libicui18n.a',
              '<(icu_lib_path)/libicudata.a',
            ],
          }]
        ],
      },

      'actions': [
        {
          'action_name': 'build_chakracore',
          'inputs': [
            '<(chakracore_input)',
          ],
          'outputs': [
            '<@(chakracore_binaries)',
          ],
          'conditions': [
            ['OS=="win"', {
              'action': [
                'msbuild',
                '/p:Platform=<(Platform)',
                '/p:Configuration=$(ConfigurationName)',
                '/p:RuntimeLib=<(component)',
                '/p:AdditionalPreprocessorDefinitions=COMPILE_DISABLE_Simdjs=1',
                '<(msvs_windows_target_platform_version_prop)',
                '/m',
                '<@(_inputs)',
              ],
            }, {
              'action': [
                'bash',
                '<(chakra_dir)/build.sh',
                '--without=Simdjs',
                '--static',
                '<@(chakra_build_flags)',
                '<@(icu_args)'
              ],
            }],
          ],
        },
      ],

      'copies': [
        {
          'destination': 'include',
          'files': [ '<@(chakracore_header)' ],
        },
        {
          'destination': '<(PRODUCT_DIR)',
          'files': [ '<@(chakracore_binaries)' ],
        },
      ],

      'direct_dependent_settings': {
        'library_dirs': [ '<(PRODUCT_DIR)' ],
        'conditions': [
          ['OS=="win"', {
          }, {
            'conditions': [
              ['OS=="mac"', {
                'libraries': [
                  '-framework CoreFoundation',
                  '-framework Security',
                ]
              }]
            ],
            'libraries': [
              '-Wl,-undefined,error',
              '<@(linker_start_group)',
              '<(chakra_libs_absolute)/lib/libChakraCoreStatic.a ' # keep this single space.
              '<@(linker_end_group)',                                         # gpy fails to patch with list
            ],
          }],
        ],
      },

    }, # end chakracore
  ],
}
