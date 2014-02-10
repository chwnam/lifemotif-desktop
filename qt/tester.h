#ifndef TESTER_H
#define TESTER_H
#include <QDir>
#include "./python_wrapper/googleoauth2wrapper.h"
#include "./python_wrapper/googleimapwrapper.h"
#include "./python_wrapper/localstructurewrapper.h"
#include "settingscontrol.h"
#include "localstructure.h"

std::string GetPythonPath();
void workaround();
void testOauth2WrapperAuthentication();
void CloseImapObject(bp::object& imapObject);
void testImap();
void testSettings();
void testStructure();

#endif // TESTER_H
