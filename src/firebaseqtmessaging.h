#pragma once

#include <QObject>

#include "firebaseqtabstractmodule.h"

namespace firebase::messaging {
struct Message;
}

class FirebaseQtMessagingPrivate;
class FirebaseQtMessaging : public FirebaseQtAbstractModule
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(FirebaseQtMessaging)
public:
    explicit FirebaseQtMessaging(FirebaseQtApp *parent);
    ~FirebaseQtMessaging() override;

Q_SIGNALS:
    void tokenReceived(const QByteArray &token);
    void messageReceived(const firebase::messaging::Message &message);

protected:
    void initialize(FirebaseQtApp *app) override;

private:
    FirebaseQtMessagingPrivate *d_ptr;
};
