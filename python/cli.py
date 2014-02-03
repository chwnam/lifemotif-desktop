#!/usr/bin/python
# -*- coding: utf-8 -*-

import argparse
import json
import sys

from email_parser import email_parser, email_message
from google_imap_control import google_imap_control
from google_oauth2_control import google_oauth2_control
from local_database_manager import local_database_manager as locdb

def parse_args():
    parser = argparse.ArgumentParser()

    parser.add_argument('--authorize', action='store_true',
                        help='Get authorization from Google')

    parser.add_argument('--build-database', action='store_true',
                        help='Build database of diary date')

    parser.add_argument('--config', default='./config.json',
                        help='Specify config file (default: config.json)')

    parser.add_argument('--list-database', action='store_true',
                        help='Print all items in the database')

    parser.add_argument('--list-mailbox', action='store_true',
                        help='Print all mailboxes')

    parser.add_argument('--view', nargs=2, type=str,
                        møetavar=('yyyymmdd', 'n'),
                        help='View diary written at yymmdd n')

    args = parser.parse_args()
    if args.authorize is False and args.build_database is False and \
       args.list_database is False and args.list_mailbox is False and \
       args.view is None:
        return None
    return args

def authorize(config):
    oauth = google_oauth2_control()
    
    url = oauth.get_authorization_url(config['secret_path'])
    code = oauth.grant_user_permission(url)
    credentials = oauth.make_credentials(code)
    
    oauth.set_credentials(config['storage_name'], credentials)

def build_database(config):
    oauth = google_oauth2_control()   
    imap_obj = oauth.imap_authenticate(
                           config['storage_name'],
                           config['email_address'],
                           debug_level=4)
    
    imap = google_imap_control(imap_obj)
    db = locdb()
    
    structure = imap.fetch_thread_structure(config['label'])
    db.build(config['local_database'], structure)

def list_database(config):
    db = locdb()
    structure = db.load(config['local_database'])

    l = sorted(list(structure))
    for x in l:
        print 'time:', x
        print '\tthread id:', structure[x][0]
        for y in structure[x][1]:
            print '\tmessage id:', y


def view_diary(config, diary_date, diary_index):
    db = locdb()
    structure = db.load(config['local_database'])

    print 'Date:', diary_date
    print 'Index:', diary_index

    thread_id, message_ids = structure[diary_date]
    message_id = message_ids[int(diary_index)]
    print 'Thread ID:', thread_id
    print 'Message ID:', message_id

    oauth = google_oauth2_control()
    imap_obj = oauth.imap_authenticate(
                           config['storage_name'],
                           config['email_address'],
                           debug_level=4)
    
    imap = google_imap_control(imap_obj)
    raw_message = imap.fetch_mail(config['label'], message_id)

    with open('%s-%s.txt' % (diary_date, diary_index), 'w') as f:
        f.write(raw_message)

    parser = email_parser()
    parser.parse(raw_message)
    parsed_message = parser.message
    messages = parsed_message.get_messages('text', 'plain')

    if messages:
        print messages[0]


def list_mailbox(config):
    oauth = google_oauth2_control()
    imap_obj = oauth.imap_authenticate(
                           config['storage_name'],
                           config['email_address'],
                           debug_level=4)
    
    imap = google_imap_control(imap_obj)
    imap.list_mailbox()


def main():
    args = parse_args()
    if args is None:
        print 'Error: please specify argument(s)! \'-h\' to help'
        return 1

    # read config file
    with open(args.config, 'r') as f:
        config = json.load(f)

    # authorization
    if args.authorize is True:
        authorize(config)

    # list mailbox
    if args.list_mailbox is True:
        list_mailbox(config)

    # build database
    if args.build_database is True:
        build_database(config)

    # list database
    if args.list_database is True:
        list_database(config)

    # view diary
    if args.view is not None:
        diary_date = args.view[0]
        diary_index = args.view[1]
        view_diary(config, diary_date, diary_index)

    return 0


if __name__ == '__main__':
    sys.exit(main())
