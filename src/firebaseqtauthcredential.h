#ifndef FIREBASEQTAUTHCREDENTIAL_H
#define FIREBASEQTAUTHCREDENTIAL_H

#include <QSharedDataPointer>

class FirebaseQtAuthCredentialPrivate;
class FirebaseQtAuthCredential
{
public:
    FirebaseQtAuthCredential(FirebaseQtAuthCredentialPrivate *priv);
    FirebaseQtAuthCredential(const FirebaseQtAuthCredential &other);
    ~FirebaseQtAuthCredential();

    QString provider() const;
    bool isValid() const;

protected:
    friend class FirebaseQtAuth;
    QSharedDataPointer<FirebaseQtAuthCredentialPrivate> d;
};

#endif // FIREBASEQTAUTHCREDENTIAL_H
