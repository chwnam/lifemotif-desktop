import httplib2
import imaplib

# google oauth2client liberaries
from oauth2client.client import flow_from_clientsecrets
from oauth2client.file import Storage

from base_oauth2_control import base_oauth2_control

# google ouath2client documentation
# http://google-api-python-client.googlecode.com/hg/docs/epy/oauth2client-module.html

class google_oauth2_control(base_oauth2_control):
    
    def __init__(self):
        flow = None
        self.imap = 'imap.gmail.com'
        self.scope = 'https://mail.google.com/'
        self.redirect_uri = 'urn:ietf:wg:oauth:2.0:oob'
        
    def get_authorization_url(self, secret_path):
        self.flow = flow_from_clientsecrets(secret_path,
                    scope=self.scope, redirect_uri=self.redirect_uri)
        url = self.flow.step1_get_authorize_url()
        return url
    
    def make_credentials(self, code):
        credentials = self.flow.step2_exchange(code)
        return credentials
        
    def set_credentials(self, storage_name, credentials):
        storage = Storage(storage_name)
        storage.put(credentials)
        
    def get_credentials(self, storage_name):
        storage = Storage(storage_name)
        credentials = storage.get()
        http = httplib2.Http()
        if credentials:
            if credentials.access_token_expired:
                credentials.refresh(http)
            credentials.authorize(http)
        return credentials    

    def imap_authenticate(self, storage_name, email_address, debug_level=0):
        credentials = self.get_credentials(storage_name)
        imap = None
        if credentials:
            auth_string = 'user=%s\1auth=Bearer %s\1\1' % \
                            (email_address, credentials.access_token)
            imap = imaplib.IMAP4_SSL(self.imap)
            imap.debug = debug_level
            imap.authenticate('XOAUTH2', lambda x: auth_string)
        return imap

    def revoke(self, storage_name):
        storage = Storage(storage_name)
        credentials = storage.get()
        http = httplib2.Http()
        credentials.revoke(http)


