#include "firebaseqtauth.h"
#include "firebaseqtauth_p.h"

#include "firebaseqtapp.h"
#include "firebaseqtapp_p.h"
#include "firebaseqtauthphone.h"
#include "firebaseqtauthcredential.h"
#include "firebaseqtauthcredential_p.h"

#include <QThread>
#include <QLoggingCategory>

using namespace firebase;

class FirebaseQtAuthStateListener : public auth::AuthStateListener {
public:
    void OnAuthStateChanged(auth::Auth* auth) override {
        auth::User user = auth->current_user();
        if (user.is_valid()) {
            // User is signed in
            qDebug("OnAuthStateChanged: signed_in %s\n", user.uid().c_str());
            const std::string displayName = user.display_name();
            const std::string emailAddress = user.email();
            const std::string photoUrl = user.photo_url();
        } else {
            // User is signed out
            qDebug("OnAuthStateChanged: signed_out\n");
        }
        // ...
    }
};

FirebaseQtAuth::FirebaseQtAuth(FirebaseQtApp *app) : FirebaseQtAbstractModule(app)
  , d_ptr(new FirebaseQtAuthPrivate)
{
}

FirebaseQtAuth::~FirebaseQtAuth()
{
    delete d_ptr->stateListener;
    delete d_ptr;
}

void FirebaseQtAuth::initialize(FirebaseQtApp *app)
{
    Q_D(FirebaseQtAuth);
    InitResult result;
    d->auth = auth::Auth::GetAuth(app->d_ptr->app, &result);
    Q_ASSERT_X(result == InitResult::kInitResultSuccess, "FirebaseQtAuth::initialize()", "result");
    d->stateListener = new FirebaseQtAuthStateListener;
    d->auth->AddAuthStateListener(d->stateListener);
}

void FirebaseQtAuth::signIn(const FirebaseQtAuthCredential &credential)
{
    Q_D(FirebaseQtAuth);
    qDebug() << "signIn" << QThread::currentThreadId();
    Future<auth::User> future = d->auth->SignInWithCredential(credential.d->credential);
    future.OnCompletion([=] (const Future<auth::User>& result) {
        if (result.error()) {
            qWarning() << "signIn future error" << result.error() << result.error_message() << result.status() << result.result();

            QMetaObject::invokeMethod(this, &FirebaseQtAuth::signInError, result.error(), QString::fromStdString(result.error_message()));
        } else {
            auth::User user = *result.result();
            Future<std::string> f2 = user.GetToken(false);
            f2.OnCompletion([=] (const Future<std::string>& r2) {
                if (result.error()) {
                    qWarning() << "get token future error" << result.error() << r2.error_message() << r2.status() << r2.result();

                    QMetaObject::invokeMethod(this, &FirebaseQtAuth::signInError, result.error(), QString::fromStdString(r2.error_message()));
                } else {
                    std::string token = *r2.result();
                    qDebug() << "get token future" << user.uid().c_str() << token.c_str();

                    QMetaObject::invokeMethod(this, &FirebaseQtAuth::signInToken, QString::fromStdString(token));
                }
            });
        }
    });
}
