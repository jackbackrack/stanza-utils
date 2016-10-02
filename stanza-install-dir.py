# from richard lin
import shlex, subprocess
from collections import namedtuple
import os
import warnings

env = dict()

if 'STANZA_CONFIG' in os.environ:
  stanza_config_dir = os.environ['STANZA_CONFIG']
else:
  stanza_config_dir = os.path.expanduser('~')
stanza_config_file = open(os.path.join(stanza_config_dir, '.stanza'), 'r')

for line in stanza_config_file:
  line_split = line.split('=')
  line_split = [elt.strip() for elt in line_split]
  line_split = [elt.strip('"') for elt in line_split]
  if line_split[0] == 'install-dir':
    assert 'STANZADIR' not in env, "multiple install-dir found in .stanza config"
    env['STANZADIR'] = line_split[1]
assert 'STANZADIR' in env, "no install-dir found in .stanza config"

print env['STANZADIR']
