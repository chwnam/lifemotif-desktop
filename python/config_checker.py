#!/usr/bin/env python
# -*- coding: utf-8 -*-

import json
import os

def check_config(config_file):
  '''
  return code, description
  '''
  try:
      f = open(config_file, 'r')
      config = json.load(f)
      f.close()

      keys = [
        ('secret_path', 'required'),
        ('storage_name', 'required'),
        ('label', 'required'),
        ('email_address', 'required'),
        ('local_structure', 'required'),
        ('debug_level', 'required'),]

      for items in keys:
          if items[0] not in config and items[1] == 'required':
           raise Exception(-1, items[0])

  except ValueError as e:
      return (-4, e)
  except IOError as e:
      return e.errno, e.strerror
  except Exception as e:
      return e

  return (0, '')

if __name__ == '__main__':
    import sys
    print check_config(sys.argv[1])
    del sys
