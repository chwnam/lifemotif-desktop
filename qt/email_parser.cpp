#include "email_parser.h"
#include <QDebug>
#include <iostream>

EmailParser::EmailParser()
{
}

void EmailParser::FeedParser(const std::string& message)
{
  mimetic::MimeEntity entity(message.begin(), message.end());

  const mimetic::Header& header = entity.header();
  const mimetic::Body& body = entity.body();

  std::cout << "Headers:";

  const mimetic::Mailbox& sender = header.sender();
  const mimetic::MailboxList& from = header.from();
  const mimetic::AddressList& to = header.to();
  const std::string& subject = header.subject();
  const mimetic::AddressList& replyto = header.replyto();
  const mimetic::MessageId& messageId = header.messageid();

  // mailbox
  std::cout << "sender mailbox: " << sender.mailbox();
  std::cout<< "sender domain: " << sender.domain();
  std::cout<< "sender label: " << sender.label();
  std::cout<< "sender sourceroute: " << sender.sourceroute();

  // mailboxlist
  for(mimetic::MailboxList::const_iterator it = from.begin();
      it != from.end(); ++it) {
    std::cout << *it;
  }

  // addresslist
  for(mimetic::AddressList::const_iterator it = to.begin();
      it != to.end(); ++it) {
    const mimetic::Address& adr = *it;

    if (adr.isGroup()) {
      std::cout<< "addresslist is a group";
      std::cout<< adr.group();
    } else {
      std::cout<< adr.mailbox();
    }
  }

  std::cout<< "subject: " << subject;
  std::cout<< "messageId: " << messageId;
}
