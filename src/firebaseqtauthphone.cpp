#include "firebaseqtauthphone.h"

#include "firebase/auth.h"

#include "firebaseqtauth.h"
#include "firebaseqtauth_p.h"
#include "firebaseqtauthcredential.h"
#include "firebaseqtauthcredential_p.h"

#include <QThread>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(FIREBASE_AUTHPHONE, "firebase.authphone")

using namespace firebase;

class FirebaseQtAuthPhoneListener : public auth::PhoneAuthProvider::Listener {
public:
    FirebaseQtAuthPhoneListener(FirebaseQtAuthPhone *q) : q_ptr(q) {}
    ~FirebaseQtAuthPhoneListener() override {}

    void OnVerificationCompleted(auth::PhoneAuthCredential credential) override;

    void OnVerificationFailed(const std::string &error) override;

    void OnCodeSent(const std::string &verification_id,
                    const auth::PhoneAuthProvider::ForceResendingToken &force_resending_token) override;

    FirebaseQtAuthPhone *q_ptr;
};

class FirebaseQtAuthPhonePrivate {
public:
    FirebaseQtAuthPhonePrivate(FirebaseQtAuth *a) : auth(a) {}

    FirebaseQtAuth *auth;
};

FirebaseQtAuthPhone::FirebaseQtAuthPhone(FirebaseQtAuth *auth) : QObject(auth)
  , d_ptr(new FirebaseQtAuthPhonePrivate(auth))
{

}

FirebaseQtAuthPhone::~FirebaseQtAuthPhone()
{
    delete d_ptr;
}

void FirebaseQtAuthPhone::verifyPhoneNumber(const QString &phoneNumber)
{
    Q_D(FirebaseQtAuthPhone);
    auto listener = new FirebaseQtAuthPhoneListener(this);
    auth::PhoneAuthProvider &phone_provider = auth::PhoneAuthProvider::GetInstance(d->auth->d_ptr->auth);
    auth::PhoneAuthOptions options;
    options.phone_number = phoneNumber.toStdString();
    options.timeout_milliseconds = 5000;
    phone_provider.VerifyPhoneNumber(options, listener);
}

FirebaseQtAuthCredential FirebaseQtAuthPhone::getCredential(const QString &verificationId, const QString &code) const
{
    Q_D(const FirebaseQtAuthPhone);
    auth::PhoneAuthProvider &phone_provider = auth::PhoneAuthProvider::GetInstance(d->auth->d_ptr->auth);
    auth::Credential cred = phone_provider.GetCredential(verificationId.toLatin1().constData(), code.toLatin1().constData());
    return FirebaseQtAuthCredential(new FirebaseQtAuthCredentialPrivate(cred));
}

void FirebaseQtAuthPhoneListener::OnVerificationCompleted(firebase::auth::PhoneAuthCredential credential) {
    // Auto-sms-retrieval or instant validation has succeeded (Android only).
    // No need for the user to input the verification code manually.
    // `credential` can be used instead of calling GetCredential().
    FirebaseQtAuthCredential cred(new FirebaseQtAuthCredentialPrivate(credential));
    qCDebug(FIREBASE_AUTHPHONE) << "OnVerificationCompleted" << QThread::currentThreadId() << cred.isValid() << cred.provider();

    QMetaObject::invokeMethod(q_ptr, &FirebaseQtAuthPhone::verificationCompleted, cred);
}

void FirebaseQtAuthPhoneListener::OnVerificationFailed(const std::string &error) {
    // Verification code not sent.
    qCDebug(FIREBASE_AUTHPHONE) << "OnVerificationFailed" << QThread::currentThreadId() << QString::fromStdString(error);

    QMetaObject::invokeMethod(q_ptr, &FirebaseQtAuthPhone::verificationFailed, QString::fromStdString(error));
}

void FirebaseQtAuthPhoneListener::OnCodeSent(const std::string &verification_id, const firebase::auth::PhoneAuthProvider::ForceResendingToken &force_resending_token) {
    Q_UNUSED(force_resending_token)
    // Verification code successfully sent via SMS.
    // Show the Screen to enter the Code.
    // Developer may want to save that verification_id along with other app states in case
    // the app is terminated before the user gets the SMS verification code.
    qCDebug(FIREBASE_AUTHPHONE) << "OnCodeSent" << QThread::currentThreadId() << QString::fromStdString(verification_id);

    QMetaObject::invokeMethod(q_ptr, &FirebaseQtAuthPhone::codeSent, QString::fromStdString(verification_id));
}
