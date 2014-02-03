import httplib2
import imaplib

# google oauth2client liberaries
from oauth2client.client import flow_from_clientsecrets
from oauth2client.file import Storage

from base_oauth2_control import base_oauth2_control

class google_oauth2_control(base_oauth2_control):
    
    def __init__(self):
        self.scope = 'https://mail.google.com/'
        self.redirect_uri = 'urn:ietf:wg:oauth:2.0:oob'
        
    def authorize(self, storage_name, secret_path):
        storage = Storage(storage_name)
        flow = flow_from_clientsecrets(secret_path,
                    scope=self.scope, redirect_uri=self.redirect_uri)
        auth_uri = flow.step1_get_authorize_url()
        code = self.user_permission_cli(auth_uri)
        credentials = flow.step2_exchange(code)
        storage.put(credentials)

    def get_credentials(self, storage_name):
        storage = Storage(storage_name)
        credentials = storage.get()
        http = httplib2.Http()
        if credentials.access_token_expired:
            credentials.refresh(http)
        credentials.authorize(http)
        return credentials    

    def imap_authenticate(self, storage_name, email_address, debug_level=0):
        credentials = self.get_credentials(storage_name)
        auth_string = 'user=%s\1auth=Bearer %s\1\1' % \
                        (email_address, credentials.access_token)
        imap = imaplib.IMAP4_SSL(GMAIL_IMAP)
        imap.debug = debug_level
        imap.authenticate('XOAUTH2', lambda x: auth_string)
        return imap