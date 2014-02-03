

class base_oauth2_control(object):
    
    scope = ''
    redirect_uri = ''
    
    #implement this
    def authorize(self):
        pass
    
    #override this method or use as-is
    def grant_user_permission(self, auth_uri):         
        print '  %s' % auth_uri
        code = raw_input('Please visit above url, and copy and paste code: ')
        return code
    
    #implement this
    def imap_authentication(self):
        pass