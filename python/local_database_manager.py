import cPickle


class local_database_manager(object):

    def __init__(self):
        pass

    def build(self, file_name, structure):
        with open(file_name, 'wb') as f:
            cPickle.dump(structure, f, cPickle.HIGHEST_PROTOCOL)

    def load(self, file_name):
        with open(file_name, 'rb') as f:
            sturcture = cPickle.load(f)
        return sturcture
