import httplib2
import imaplib

# google oauth2client liberaries
from oauth2client.client import flow_from_clientsecrets
from oauth2client.file import Storage

from imap_manager import imap_manager

GMAIL_SCOPE = 'https://mail.google.com/'
GMAIL_IMAP = 'imap.gmail.com'
REDIRECT_URI = 'urn:ietf:wg:oauth:2.0:oob'


class oauth_manager(object):

    def authorize(self, storage_name, secret_path):
        storage = Storage(storage_name)
        flow = flow_from_clientsecrets(secret_path,
                    scope=GMAIL_SCOPE, redirect_uri=REDIRECT_URI)
        auth_uri = flow.step1_get_authorize_url()
        code = self.user_permission_cli(auth_uri)
        credentials = flow.step2_exchange(code)
        storage.put(credentials)

    def user_permission_cli(self, auth_uri):
        print '  %s' % auth_uri
        code = raw_input('Please visit above url, and copy and paste code: ')
        return code

    def imap_authenticate(self, storage_name, email_address, debug_level=0):
        storage = Storage(storage_name)
        credentials = storage.get()
        http = httplib2.Http()
        if credentials.access_token_expired:
            credentials.refresh(http)
        credentials.authorize(http)
        auth_string = 'user=%s\1auth=Bearer %s\1\1' % \
                        (email_address, credentials.access_token)
        imap = imaplib.IMAP4_SSL(GMAIL_IMAP)
        imap.debug = debug_level
        imap.authenticate('XOAUTH2', lambda x: auth_string)
        return imap_manager(imap)

