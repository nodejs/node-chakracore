// Copyright Microsoft. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and / or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

var child_process = require('child_process');
var fs = require('fs');
var path = require('path');

const header = "# ChakraShim ChangeLog";
const commitUrl = "https://github.com/nodejs/node-chakracore/commit/";
const releasePrefix = "node-chakracore";
const EOL_REGEX = /\r\n|\n|\r/;
var srcroot;
var lastRelease;
var changeLog = [];

function changelogFile() {
  return path.join(srcroot, 'CHANGELOG.md');
}

// Load existing CHANGELOG.md content
function loadChangelogFile() {
  fs.readFile(changelogFile(), function(err, data) {
    // ignore err, if so assume no existing CHANGELOG file
    if (data) {
      console.log('Found existing CHANGELOG');
      data = data.toString();

      var r = new RegExp("##.*, (" + releasePrefix + "[^,\n]*)\n").exec(data);
      if (r) {
        lastRelease = r[1].replace(EOL_REGEX, '');
        console.log('Existing CHANGELOG has last release', lastRelease);
      }

      changeLog = data.split(EOL_REGEX);
    } else {
      changeLog.push(header);
    }

    while (changeLog.length < 2) {
      changeLog.push('');
    }

    loadGitReleaseTags();
  });
}

// Load existing git release tags
function loadGitReleaseTags() {
  var releaseTags = [];
  git_pipe([
    'tag', '-l',
    '--format=%(refname:short) %(objectname:short) ' +
      '%(taggerdate:short) %(authordate:short)', // at least 1 date available
    releasePrefix + '*'],
    line => {
      var r = /^(\S+) (\S+)\s+(\S+)/.exec(line);
      if (r) {
        releaseTags.push({name: r[1], commit: r[2], date: r[3]});
      }
    },
    () => matchReleases(releaseTags, lastRelease)
  );
}

function filterReleaseTags(releaseTag) {
  var branches = git_sync(['branch', '--contains', releaseTag.name]);
  // If tag is present in current branch, then it is a valid tag to consider
  return branches.split(/\n/).some((x) => {return x.startsWith('* ');});
}

function compareReleasesByDate(a, b) {
  if (a.date < b.date) {
    return -1;
  } else if (a.date > b.date) {
    return 1;
  } else {
    return 0;
  }
}

// Match last release, figure out next release and update ranges
function matchReleases(releaseTags, lastRelease) {
  releaseTags.sort(compareReleasesByDate);
  releaseTags = releaseTags.filter(filterReleaseTags);
  var ver;
  command_pipe(process.argv[0], ['--version'],
  line => {
    ver = ver || line.replace(/^v/, '');
  },
  () => {
    // Figure out new release name
    var newReleaseName = releasePrefix + '-' + ver;
    var patch;
    if (releaseTags.length > 0) {
      var lastReleaseName = releaseTags[releaseTags.length - 1].name;
      if (lastReleaseName.indexOf(newReleaseName) == 0) {
        patch = +lastReleaseName.substr(newReleaseName.length) + 1;
      }
    }
    newReleaseName += patch || 1;

    if (lastRelease == newReleaseName) {
      return console.log('CHANGELOG already has', newReleaseName,
        'changes. \nPlease update manually, or remove', newReleaseName,
        'section and run this script again to regenerate updates.');
    }

    // Remove (... - lastRelease) from releaseTags
    if (lastRelease) {
      var lastIndex = releaseTags.findIndex(v => v.name == lastRelease);
      if (lastIndex < 0) {
        return console.error('ERROR:', lastRelease,
          'not found in release tags.',
          'Please fetch release tags (git fetch --tags ...)');
      }
      releaseTags.splice(0, lastIndex);
    } else {
      // Don't look at commits older than `47000c74f4~1`
      releaseTags.splice(0, 0, { commit: "47000c74f4~1" });
    }

    // Last range to be updated
    releaseTags.push({
      name: newReleaseName,
      commit: 'HEAD',
      date: new Date().toISOString().replace(/T.*/,'')});

    updateChangelogs(releaseTags);
  });
}

// Update change logs for every range in releaseTags
function updateChangelogs(releaseTags) {
  if (releaseTags.length >= 2) {
    const last = releaseTags.shift();
    const next = releaseTags[0];
    var commits = [];
    return git_pipe([
      'log', '--first-parent', '--pretty=%h %s',
      last.commit + '..' + next.commit, '--', srcroot + '/../..'],
    line => {
      var r = /^(\S+) (.+)$/.exec(line);
      if (r) {
        commits.push({commit: r[1], subject: r[2]});
      }
    },
    () => {
      if (commits.length > 0) {
        var output = '## ' + next.date + ", " + next.name +
          '\n\n### Commits\n\n';
        commits.forEach(c => {
          const sha = c.commit;
          output += `* [[\`${sha}\`](${commitUrl + sha})] - ${c.subject}\n`;
        });

        changeLog.splice(2, 0, output);
      }

      updateChangelogs(releaseTags);
    });
  } else {
    return saveChangelog();
  }
}

function pipe(proc, onLine) {
  var last;

  function addData(chunk) {
    var lines = ((last || '') + chunk).split(EOL_REGEX);
    last = lines[lines.length - 1];

    for (var i = 0; i < lines.length - 1; i++) {
      onLine(lines[i]);
    }
  }

  proc.stdout.on('data', function(data) {
    addData(data.toString());
  }).on('end', function() {
    if (last !== undefined) {
      onLine(last);
    }
  });

  return proc;
}

function command_sync(command, args) {
   /* make buffer size 500KB for git sync commands */
  var x = child_process.spawnSync(command, args, {maxBuffer : 500 * 1024});
  return x.status == 0 ? x.stdout.toString(): x.error.toString() + "\n" + x.stderr.toString();
}

function git_sync(args) {
  return command_sync('git', args);
}

function command_pipe(command, args, onLine, done) {
  return pipe(child_process.spawn(command, args), onLine).on(
    'exit', function(code) {
      if (code != 0) {
        return console.log(command, args, 'exits abnormally:', code);
      }
      done();
    });
}

function git_pipe(args, onLine, done) {
  return command_pipe('git', args, onLine, done);
}

// Save updated changelog
function saveChangelog() {
  fs.writeFile(changelogFile(),
    changeLog.join('\n'),
    function(err) {
      if (err) throw err;
      console.log('Updated ' + path.relative('', changelogFile()));
    });
}

// Start
srcroot = process.argv.length > 2 ?
  process.argv[2] : path.join(path.dirname(process.argv[1]), '..');

loadChangelogFile();
