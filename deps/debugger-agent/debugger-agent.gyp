{
  "targets": [{
    "target_name": "debugger-agent",
    "type": "<(library)",
    "include_dirs": [
      "src",
      "include",
      "../uv/include",

      # Private node.js folder and stuff needed to include from it
      "../../src",
      "../cares/include",
    ],
    "direct_dependent_settings": {
      "include_dirs": [
        "include",
      ],
    },
    'conditions': [
      ['node_use_chakra=="false"', {
        "include_dirs": [
          "../v8/include",
        ],
      }, {
        "include_dirs": [
          "../chakrashim/include",
        ],
      }],
      [ 'gcc_version<=44', {
        # GCC versions <= 4.4 do not handle the aliasing in the queue
        # implementation, so disable aliasing on these platforms
        # to avoid subtle bugs
        'cflags': [ '-fno-strict-aliasing' ],
      }],
    ],
    "sources": [
      "src/agent.cc",
    ],
  }],
}
