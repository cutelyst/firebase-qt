#include "firebaseqtauth.h"
#include "firebaseqtauth_p.h"

#include "firebaseqtapp.h"
#include "firebaseqtapp_p.h"
#include "firebaseqtauthphone.h"
#include "firebaseqtauthcredential.h"
#include "firebaseqtauthcredential_p.h"

#include <QThread>
#include <QLoggingCategory>

class FirebaseQtAuthStateListener : public firebase::auth::AuthStateListener {
 public:
  void OnAuthStateChanged(firebase::auth::Auth* auth) override {
    firebase::auth::User* user = auth->current_user();
    if (user != nullptr) {
      // User is signed in
      printf("OnAuthStateChanged: signed_in %s\n", user->uid().c_str());
      const std::string displayName = user->display_name();
      const std::string emailAddress = user->email();
      const std::string photoUrl = user->photo_url();
    } else {
      // User is signed out
      printf("OnAuthStateChanged: signed_out\n");
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
    firebase::InitResult result;
    d->auth = firebase::auth::Auth::GetAuth(app->d_ptr->app, &result);
    Q_ASSERT_X(result == firebase::InitResult::kInitResultSuccess, "FirebaseQtAuth::initialize()", "result");
    d->stateListener = new FirebaseQtAuthStateListener;
    d->auth->AddAuthStateListener(d->stateListener);
}

void FirebaseQtAuth::signIn(const FirebaseQtAuthCredential &credential)
{
    Q_D(FirebaseQtAuth);
    qDebug() << "signIn" << QThread::currentThreadId();
    firebase::Future<firebase::auth::User*> future = d->auth->SignInWithCredential(credential.d->credential);
    future.OnCompletion([=] (const firebase::Future<firebase::auth::User*> &result) {
        if (result.error()) {
            qWarning() << "signIn future error" << result.error() << result.error_message() << result.status() << result.result();
            Q_EMIT signInError(result.error(), QString::fromStdString(result.error_message()));
        } else {
            firebase::auth::User *user = *result.result();
            firebase::Future<std::string> f2 = user->GetToken(false);
            f2.OnCompletion([=] (const firebase::Future<std::string> &r2) {
                if (result.error()) {
                    qWarning() << "get token future error" << result.error() << r2.error_message() << r2.status() << r2.result();
                    Q_EMIT signInError(result.error(), QString::fromStdString(r2.error_message()));
                } else {
                    std::string token = *r2.result();
                    qDebug() << "get token future" << user->uid().c_str() << token.c_str();
                    Q_EMIT signInToken(QString::fromStdString(token));
                }
            });
        }
    });
}
