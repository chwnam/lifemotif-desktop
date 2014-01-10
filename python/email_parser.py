import email.parser
import email.header
import email.utils
import time


class email_message(object):

    def __init__(self):
        self.content_maintype = None
        self.content_subtype = None
        self.content_charset = None
        self.content = None

        self.parent = None
        self.subparts = []

    def __str__(self):
        msg = ''
        msg += 'parent: %r\n' % self.parent
        msg += 'content_maintype: %s\n' % self.content_maintype
        msg += 'content_subtype: %s\n' % self.content_subtype
        msg += 'content_charset: %s\n' % self.content_charset
        if self.content_maintype == 'image':
            msg += 'content length: %s\n' % len(self.content)
        else:
            msg += 'content: %s\n' % self.content
        msg += 'subparts: %s\n' % self.subparts
        return msg

    def get_messages(self, content_maintype, content_subtype):
        messages = []

        if self.content_maintype == content_maintype and \
           self.content_subtype == content_subtype:
            messages.append(self)

        for x in self.subparts:
            messages += x.get_messages(content_maintype, content_subtype)

        return messages


class email_parser(object):

    def __init__(self):
        self.parsed = None
        self.from_addr = None
        self.to_addr = []
        self.date = None
        self.subject = None
        self.message = None

    def parse(self, raw_message):
        parser = email.parser.FeedParser()
        parser.feed(raw_message)
        self.parsed = parser.close()

        self.from_addr = self.parsed['From']
        self.to_addr = self.parsed['To']

        datetuple = email.utils.parsedate(self.parsed['Date'])
        self.date = time.localtime(time.mktime(datetuple))

        subj = email.header.decode_header(self.parsed['Subject'])
        self.subject = subj[0][0]  # [0][1]: encoding (usually utf-8)

        self.message = self.parse_payload(self.parsed)

    def parse_payload(self, payload, parent=None):
        msg = email_message()
        msg.parent = parent

        msg.content_maintype = payload.get_content_maintype()
        msg.content_subtype = payload.get_content_subtype()

        if payload.is_multipart():
            subparts = payload.get_payload()
            for subpart in subparts:
                msg.subparts.append(self.parse_payload(subpart, msg))
            return msg
        else:
            msg.content_charset = payload.get_content_charset()
            msg.content = payload.get_payload(decode=True)
            # when it is image, 'filename' property is added
            if msg.content_maintype == 'image':
                msg.filename = payload.get_filename()
            return msg


def print_subtypes(msg, tabs):
    if tabs:
        print '    ' * tabs,

    print msg.content_subtype
    if msg.subparts:
        for x in msg.subparts:
            print_subtypes(x, tabs + 1)

if __name__ == '__main__':
    import sys

    f = open(sys.argv[1], 'r')
    raw_message = f.read()
    f.close()

    parser = email_parser()
    parser.parse(raw_message)
    msg = parser.message

    # print-out all structure
    print_subtypes(msg, 0)

    image = msg.get_messages('image', 'jpeg')
    text = msg.get_messages('text', 'plain')

    if text:
        print
        print text[0]

    if image:
        print
        print image[0]

