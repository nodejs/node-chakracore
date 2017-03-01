# Copyright 2016 the V8 project authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
  'variables': {
    'protocol_path': '../../third_party/WebKit/Source/platform/inspector_protocol',
  },
  'includes': [
    'inspector.gypi',
    '../../third_party/WebKit/Source/platform/inspector_protocol/inspector_protocol.gypi',
  ],
  'targets': [
    { 'target_name': 'protocol_compatibility',
      'type': 'none',
      'actions': [
        {
          'action_name': 'protocol_compatibility',
          'inputs': [
            'js_protocol.json',
          ],
          'outputs': [
            '<@(SHARED_INTERMEDIATE_DIR)/src/js_protocol.stamp',
          ],
          'action': [
            'python',
            '<(protocol_path)/CheckProtocolCompatibility.py',
            '--stamp', '<@(_outputs)',
            'js_protocol.json',
          ],
          'message': 'Generating inspector protocol sources from protocol json definition',
        },
      ]
    },
    { 'target_name': 'protocol_generated_sources',
      'type': 'none',
      'dependencies': [ 'protocol_compatibility' ],
      'actions': [
        {
          'action_name': 'protocol_generated_sources',
          'inputs': [
            'js_protocol.json',
            'inspector_protocol_config.json',
            '<@(inspector_protocol_files)',
          ],
          'outputs': [
            '<@(inspector_generated_sources)',
          ],
          'action': [
            'python',
            '<(protocol_path)/CodeGenerator.py',
            '--jinja_dir', '../../third_party',
            '--output_base', '<(SHARED_INTERMEDIATE_DIR)/src/inspector',
            '--config', 'inspector_protocol_config.json',
          ],
          'message': 'Generating inspector protocol sources from protocol json',
        },
      ]
    },
    {
      'target_name': 'standalone_inspector',
      'type': 'static_library',
      'include_dirs': [
        '<(SHARED_INTERMEDIATE_DIR)',
        '<(SHARED_INTERMEDIATE_DIR)/include',
        '../../include',
        '../..',
        '../../../'
      ],
      'sources': [
        '<@(inspector_all_sources)'
      ],
      'dependencies': [
        'protocol_generated_sources',
        '../../../chakracore.gyp:chakracore#host',
        '../../../chakrashim.gyp:chakrashim',
      ],
    },
  ],
}
