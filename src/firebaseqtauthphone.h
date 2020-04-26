#pragma once

#include <QObject>

class FirebaseQtAuth;
class FirebaseQtAuthCredential;
class FirebaseQtAuthPhonePrivate;
class FirebaseQtAuthPhone : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(FirebaseQtAuthPhone)
public:
    explicit FirebaseQtAuthPhone(FirebaseQtAuth *auth);
    virtual ~FirebaseQtAuthPhone();

    void verifyPhoneNumber(const QString &phoneNumber);
    FirebaseQtAuthCredential getCredential(const QString &verificationId, const QString &code) const;

Q_SIGNALS:
    void verificationCompleted(const FirebaseQtAuthCredential &credential);
    void verificationFailed(const QString &error);
    void codeSent(const QString &verificationId);

private:
    FirebaseQtAuthPhonePrivate *d_ptr;
};

