#ifndef FIREBASEQTMESSAGING_H
#define FIREBASEQTMESSAGING_H

#include <QObject>

#include "firebaseqtabstractmodule.h"

class FirebaseQtMessagingPrivate;
class FirebaseQtMessaging : public FirebaseQtAbstractModule
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(FirebaseQtMessaging)
public:
    explicit FirebaseQtMessaging(FirebaseQtApp *parent);
    virtual ~FirebaseQtMessaging() override;

Q_SIGNALS:
    void tokenReceived(const QByteArray &token);
    void messageReceived(const QMap<QString, QString> &data);

protected:
    virtual void initialize(FirebaseQtApp *app) override;

private:
    FirebaseQtMessagingPrivate *d_ptr;
};

#endif // FIREBASEQTMESSAGING_H
