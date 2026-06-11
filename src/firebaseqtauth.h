#ifndef FIREBASEQTAUTH_H
#define FIREBASEQTAUTH_H

#include "firebaseqtabstractmodule.h"

#include <QObject>

class FirebaseQtApp;
class FirebaseQtAuthCredential;
class FirebaseQtAuthPrivate;
class FirebaseQtAuth : public FirebaseQtAbstractModule
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(FirebaseQtAuth)
public:
    explicit FirebaseQtAuth(FirebaseQtApp *app);
    virtual ~FirebaseQtAuth() override;

    virtual void initialize(FirebaseQtApp *app) override;

    void signIn(const FirebaseQtAuthCredential &credential);

Q_SIGNALS:
    void signInError(int code, const QString &errorMessage);
    void signInToken(const QString &token);

protected:
    friend class FirebaseQtAuthPhone;

    FirebaseQtAuthPrivate *d_ptr;
};

#endif // FIREBASEQTAUTH_H
