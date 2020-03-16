#!/usr/bin/python3

import os

repo_root = os.path.dirname(os.path.realpath(__file__)) + '/../'
readme_filename = repo_root + 'README.md'
matchable_usage_filename = repo_root + 'test/programs/matchable_usage.cpp'

with open(readme_filename) as f:
    orig_content = f.readlines()

getting_started = '## Matchable API'
new_content = []

# skip to getting started heading
readme_index = -1
found = False
for line in orig_content:
    readme_index += 1
    new_content.append(line)
    if line.startswith(getting_started):
        found = True
        break
if not found:
    print('failed to find line \'' + getting_started + '\'')
    exit(-1)
    
    
# skip to ```
found = False
while readme_index < len(orig_content) - 1:
    readme_index += 1
    new_content.append(orig_content[readme_index])
    if orig_content[readme_index].startswith('```'):
        found = True
        break
if not found:
    print('failed to find start line \'```\'')
    exit(-1)


# skip to ``` without appending any new content
found = False
while readme_index < len(orig_content) - 1:
    readme_index += 1
    if orig_content[readme_index].startswith('```'):
        found = True
        break
if not found:
    print('failed to find end line \'```\'')
    exit(-1)

# open test program
with open(matchable_usage_filename) as f:
    usage_content = f.readlines()

# skip over license
usage_index = -1
found = False
while usage_index < len(usage_content) - 1:
    usage_index += 1
    if usage_content[usage_index].startswith('#include'):
        found = True
        break
if not found:
    print('failed to find begining of usage')
    exit(-1)

# copy the rest of the test program (after license) to new_content
while usage_index < len(usage_content):
    new_content.append(usage_content[usage_index])
    usage_index += 1
new_content += '\n'

# remember the readme_index has already skipped over the old content, so
# just pick up where we left off to copy the rest of readme to new_content
while readme_index < len(orig_content):
    new_content.append(orig_content[readme_index])
    readme_index += 1

# write new content back to readme file
f = open(readme_filename, 'w')
for line in new_content:
    f.write(line)
