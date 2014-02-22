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

    def get_mailbox_list(self):
        result, data = self.imap.list();
        if result != 'OK':
            raise Exception(result)
        return data

    def list_mailbox(self):
        for x in self.get_mailbox_list():
            print '%s ---> %s' % (x, decode(x))
