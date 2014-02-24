#! -*- coding: utf-8 -*-
import email.utils
import re

from base_imap_control import base_imap_control
from imapUTF7 import imapUTF7Encode as encode
from imapUTF7 import imapUTF7Decode as decode


class google_imap_control(base_imap_control):

    def __init__(self, imap):
        super(google_imap_control, self).__init__(imap)
        
    def fetch_thread_structure(self, label):
        # label must be unicode!
        self.select_mailbox(encode(label))
        data = self.treat_uid('SEARCH', 'ALL')
        ids = data[0].split()
        rng = '%s:%s' % (ids[0], ids[-1])
        data = self.treat_uid('FETCH', rng,
                        '(X-GM-THRID X-GM-MSGID BODY[HEADER.FIELDS (DATE)])')
        exp = re.compile(r'\d+ \(X-GM-THRID (\d+) X-GM-MSGID (\d+) UID \d+')
        threads = {}
        i = 0
        j = len(data)
        while(i < j):
            t, m = exp.match(data[i][0]).groups()
            thr = long(t)
            msg = long(m)
            if thr not in threads:
                threads[thr] = set()
            threads[thr].add((msg, self.parse_datetime(data[i][1])))
            i += 2

        # structure: converted from threads dictionary
        # key: date
        # value: tuple (thread_id, [message_ids])
        structure = {}
        for thr_id in threads:
            msg_ids = sorted([x[0] for x in threads[thr_id]])
            mindate = min([x[1] for x in threads[thr_id]])
            structure[mindate] = (thr_id, msg_ids)
        return structure

    def fetch_mail(self, label, message_id):
        self.select_mailbox(encode(label))
        uid = self.treat_uid('SEARCH', 'X-GM-MSGID', message_id)
        result = self.treat_uid('FETCH', uid[0], '(RFC822)')
        return result[0][1]
    
    def parse_datetime(self, datetimetext):
        colonpos = datetimetext.find(':')
        parsed = email.utils.parsedate(datetimetext[colonpos + 1:].strip())
        return '%04d%02d%02d' % (parsed[0], parsed[1], parsed[2])
        
    def get_google_mailbox_list(self):
        ''' get mailbox and decode ''' 
        exp = re.compile(r'\((.+)\) "(.+)" "(.+)"')
        encoded = self.get_mailbox_list();
        decoded = []

        for line in encoded:
            m = exp.match(line)
            if m:
                groups = m.groups()
                flags = groups[0].split()
                root  = groups[1]
                name  = decode(groups[2])
                decoded.append((flags, root, name))
        
        return decoded
    
    def get_googe_mailbox_simple_list(self):
        return [x[2].encode('utf-8') for x in self.get_google_mailbox_list()]
        
    def list_google_mailbox(self):
        for x in self.get_googe_mailbox_simple_list():
            print x
        
