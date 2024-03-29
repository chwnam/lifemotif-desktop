import cPickle
import os


class local_structure_control(object):

    def __init__(self):
        pass

    def build(self, file_name, structure):
        with open(file_name, 'wb') as f:
            cPickle.dump(structure, f, cPickle.HIGHEST_PROTOCOL)

    def load(self, file_name):
        structure = {}
        if os.path.exists(file_name):
          with open(file_name, 'rb') as f:
              structure = cPickle.load(f)
        return structure
