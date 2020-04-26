#include "firebaseqtauthcredential.h"
#include "firebaseqtauthcredential_p.h"

#include <QString>

FirebaseQtAuthCredential::FirebaseQtAuthCredential(FirebaseQtAuthCredentialPrivate *priv)
    : d(priv)
{

}

FirebaseQtAuthCredential::FirebaseQtAuthCredential(const FirebaseQtAuthCredential &other)
    : d(other.d)
{

}

FirebaseQtAuthCredential::~FirebaseQtAuthCredential()
{
}

QString FirebaseQtAuthCredential::provider() const
{
    return QString::fromStdString(d->credential.provider());
}

bool FirebaseQtAuthCredential::isValid() const
{
    return d->credential.is_valid();
}
