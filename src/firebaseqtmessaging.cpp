#include "firebaseqtmessaging.h"

#include "firebaseqtabstractmodule.h"
#include "firebaseqtapp.h"
#include "firebaseqtapp_p.h"

#include "firebase/messaging.h"

#include <QThread>
#include <QLoggingCategory>

Q_LOGGING_CATEGORY(FIREBASE_MESSAGING, "firebase.messaging")

class FirebaseQtMessagingPrivate : public ::firebase::messaging::Listener
{
public:
    FirebaseQtMessagingPrivate(FirebaseQtMessaging *q) : q_ptr(q) { }

    /// Called on the client when a message arrives.
    ///
    /// @param[in] message The data describing this message.
    virtual void OnMessage(const ::firebase::messaging::Message& message);

    /// Called on the client when a registration token arrives. This function
    /// will eventually be called in response to a call to
    /// firebase::messaging::Initialize(...).
    ///
    /// @param[in] token The registration token.
    virtual void OnTokenReceived(const char* token);

    FirebaseQtMessaging *q_ptr;
};

FirebaseQtMessaging::FirebaseQtMessaging(FirebaseQtApp *parent) : FirebaseQtAbstractModule(parent)
  , d_ptr(new FirebaseQtMessagingPrivate(this))
{

}

FirebaseQtMessaging::~FirebaseQtMessaging()
{
    delete d_ptr;
}

void FirebaseQtMessaging::initialize(FirebaseQtApp *app)
{
    ::firebase::messaging::Initialize(*app->d_ptr->app, d_ptr);
}

void FirebaseQtMessagingPrivate::OnMessage(const firebase::messaging::Message &message)
{
    qCDebug(FIREBASE_MESSAGING) << "OnMessage" << QString::fromStdString(message.from) << QString::fromStdString(message.message_id) << message.data.size();
    QMap<QString, QString> data;
    auto it = message.data.begin();
    while (it != message.data.end()) {
        qDebug() << "OnMessage data:" << it->first.c_str() << it->second.c_str();
        data.insert(QString::fromStdString(it->first), QString::fromStdString(it->second));
        ++it;
    }
    Q_EMIT q_ptr->messageReceived(data);
}

void FirebaseQtMessagingPrivate::OnTokenReceived(const char *token)
{
    qCDebug(FIREBASE_MESSAGING) << "OnTokenReceived" << QThread::currentThreadId() << token;

    Q_EMIT q_ptr->tokenReceived(token);
}
