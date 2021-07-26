#ifndef FIREBASEQTAPP_P_H
#define FIREBASEQTAPP_P_H

#include <QString>
#include <vector>

#include <firebase/app.h>
#include "firebaseqtabstractmodule.h"

class FirebaseQtAppPrivate {
public:
    QString appId;
    QString apiKey;
    QString databaseUrl;
    QString messagingSenderId;
    QString storageBucket;
    QString projectId;
    std::vector<FirebaseQtAbstractModule *> modules;
    firebase::App *app = nullptr;
};

#endif // FIREBASEQTAPP_P_H
