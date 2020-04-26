#ifndef FIREBASEQTAUTHCREDENTIAL_P_H
#define FIREBASEQTAUTHCREDENTIAL_P_H

#include <QSharedData>

#include "firebase/auth.h"

class FirebaseQtAuthCredentialPrivate : public QSharedData {
public:
    FirebaseQtAuthCredentialPrivate(::firebase::auth::Credential cred) : credential(cred) {}
    ::firebase::auth::Credential credential;
};

#endif // FIREBASEQTAUTHCREDENTIAL_P_H
