from imapUTF7 import imapUTF7Decode as decode

class base_imap_control(object):
    
    def __init__(self, imap):
        self.imap = imap
    
    def select_mailbox(self, label):
        self.imap.select(label)
    
    def treat_uid(self, command, *args):
        result, data = self.imap.uid(command, *args)
        if result != 'OK':
            raise Exception(result)
        return data
    
    def list_mailbox(self):
        result, data = self.imap.list()
        if result != 'OK':
            raise Exception(result)
        for x in data:
            print '%s ---> %s' % (x, decode(x))
