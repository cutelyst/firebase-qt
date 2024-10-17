#include "firebaseqtapp.h"
#include "firebaseqtapp_p.h"

#ifdef Q_OS_ANDROID
#include <QtCore/private/qandroidextras_p.h>
#include <QJniEnvironment>
#endif

#include "firebase/messaging.h"

#include "firebaseqtabstractmodule.h"

#include <QTimer>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(FIREBASE_APP, "firebase.app")

FirebaseQtApp::FirebaseQtApp(QObject *parent) : QObject(parent)
  , d_ptr(new FirebaseQtAppPrivate)
{

}

FirebaseQtApp::~FirebaseQtApp()
{
    delete d_ptr;
}

void FirebaseQtApp::setAppId(const QString &id)
{
    Q_D(FirebaseQtApp);
    d->appId = id;
}

void FirebaseQtApp::setApiKey(const QString &key)
{
    Q_D(FirebaseQtApp);
    d->apiKey = key;
}

void FirebaseQtApp::setDatabaseUrl(const QString &url)
{
    Q_D(FirebaseQtApp);
    d->databaseUrl = url;
}

void FirebaseQtApp::setMessagingSenderId(const QString &id)
{
    Q_D(FirebaseQtApp);
    d->messagingSenderId = id;
}

void FirebaseQtApp::setStorageBucket(const QString &storageBucket)
{
    Q_D(FirebaseQtApp);
    d->storageBucket = storageBucket;
}

void FirebaseQtApp::setProjectId(const QString &id)
{
    Q_D(FirebaseQtApp);
    d->projectId = id;
}

void FirebaseQtApp::initialize()
{
    Q_D(FirebaseQtApp);
    auto options = ::firebase::AppOptions();
    if (!d->appId.isEmpty()) {
        options.set_app_id(d->appId.toLatin1().constData());
    }
    if (!d->apiKey.isEmpty()) {
        options.set_api_key(d->apiKey.toLatin1().constData());
    }
    if (!d->databaseUrl.isEmpty()) {
        options.set_database_url(d->databaseUrl.toLatin1().constData());
    }
    if (!d->messagingSenderId.isEmpty()) {
        options.set_messaging_sender_id(d->messagingSenderId.toLatin1().constData());
    }
    if (!d->storageBucket.isEmpty()) {
        options.set_storage_bucket(d->storageBucket.toLatin1().constData());
    }
    if (!d->projectId.isEmpty()) {
        options.set_project_id(d->projectId.toLatin1().constData());
    }

#ifdef Q_OS_ANDROID
    d->app = ::firebase::App::Create(options, QJniEnvironment::getJniEnv(), QNativeInterface::QAndroidApplication::context().object());
    Q_ASSERT_X(d->app, "FirebaseQtApp", "App::Create");
#else
    d->app = ::firebase::App::Create();
    Q_ASSERT_X(d->app, "FirebaseQtApp", "App::Create");
#endif

    QTimer::singleShot(0, this, [=] {
        for (FirebaseQtAbstractModule *module : d->modules) {
            module->initialize(this);
        }
    });
}

void FirebaseQtApp::registerModule(FirebaseQtAbstractModule *module)
{
    Q_D(FirebaseQtApp);
    d->modules.push_back(module);
}
