#include "firebaseqtmessaging.h"

#include "firebaseqtabstractmodule.h"
#include "firebaseqtapp.h"
#include "firebaseqtapp_p.h"

#include <firebase/messaging.h>

#include <QLoggingCategory>

Q_LOGGING_CATEGORY(FIREBASE_MESSAGING, "firebase.messaging")

class FirebaseQtMessagingPrivate final : public ::firebase::messaging::Listener
{
public:
    FirebaseQtMessagingPrivate(FirebaseQtMessaging *q)
        : q_ptr(q)
    {
    }

    /// Called on the client when a message arrives.
    ///
    /// @param[in] message The data describing this message.
    void OnMessage(const ::firebase::messaging::Message &message) override;

    /// Called when the app instance has been registered with FCM.
    ///
    /// @param[in] installationId The Firebase Installation ID (FID).
    void OnRegistrationReceived(const char *installationId) override;

    /// Called when the app instance has been unregistered from FCM.
    ///
    /// @param[in] installationId The FID that was unregistered.
    void OnUnregistrationReceived(const char *installationId) override;

    /// \deprecated Prefer OnRegistrationReceived.
    void OnTokenReceived(const char *token) override;

    FirebaseQtMessaging *q_ptr;
};

FirebaseQtMessaging::FirebaseQtMessaging(FirebaseQtApp *parent)
    : FirebaseQtAbstractModule(parent)
    , d_ptr{new FirebaseQtMessagingPrivate{this}}
{
}

FirebaseQtMessaging::~FirebaseQtMessaging()
{
    delete d_ptr;
}

void FirebaseQtMessaging::initialize(FirebaseQtApp *app)
{
    auto result = ::firebase::messaging::Initialize(*app->d_ptr->app, d_ptr);
    Q_ASSERT(result == ::firebase::InitResult::kInitResultSuccess);
}

void FirebaseQtMessaging::registerInstance()
{
    auto future = ::firebase::messaging::Register();
    future.OnCompletion([this](const ::firebase::Future<void> &result) {
        if (result.error()) {
            qWarning(FIREBASE_MESSAGING)
                << "Register error" << result.error() << result.error_message();
            QMetaObject::invokeMethod(this,
                                      &FirebaseQtMessaging::registerError,
                                      result.error(),
                                      QString::fromUtf8(result.error_message()));
        }
        // FID is delivered via OnRegistrationReceived on success.
    });
}

void FirebaseQtMessaging::unregisterInstance()
{
    auto future = ::firebase::messaging::Unregister();
    future.OnCompletion([this](const ::firebase::Future<void> &result) {
        if (result.error()) {
            qWarning(FIREBASE_MESSAGING)
                << "Unregister error" << result.error() << result.error_message();
            QMetaObject::invokeMethod(this,
                                      &FirebaseQtMessaging::unregisterError,
                                      result.error(),
                                      QString::fromUtf8(result.error_message()));
        }
        // FID is delivered via OnUnregistrationReceived on success.
    });
}

void FirebaseQtMessagingPrivate::OnMessage(const firebase::messaging::Message &message)
{
    qDebug(FIREBASE_MESSAGING) << "OnMessage" << QString::fromStdString(message.from)
                               << QString::fromStdString(message.notification->title);

    QMetaObject::invokeMethod(q_ptr, &FirebaseQtMessaging::messageReceived, message);
}

void FirebaseQtMessagingPrivate::OnRegistrationReceived(const char *installationId)
{
    qCDebug(FIREBASE_MESSAGING) << "OnRegistrationReceived" << installationId;

    QMetaObject::invokeMethod(
        q_ptr, &FirebaseQtMessaging::registrationReceived, QByteArray{installationId});
}

void FirebaseQtMessagingPrivate::OnUnregistrationReceived(const char *installationId)
{
    qCDebug(FIREBASE_MESSAGING) << "OnUnregistrationReceived" << installationId;

    QMetaObject::invokeMethod(
        q_ptr, &FirebaseQtMessaging::unregistrationReceived, QByteArray{installationId});
}

void FirebaseQtMessagingPrivate::OnTokenReceived(const char *token)
{
    qCDebug(FIREBASE_MESSAGING) << "OnTokenReceived (deprecated)" << token;

    QT_WARNING_PUSH
    QT_WARNING_DISABLE_DEPRECATED
    QMetaObject::invokeMethod(q_ptr, &FirebaseQtMessaging::tokenReceived, QByteArray{token});
    QT_WARNING_POP
}
