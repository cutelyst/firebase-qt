#ifndef FIREBASEQTAPP_H
#define FIREBASEQTAPP_H

#include <QObject>

class FirebaseQtAbstractModule;
class FirebaseQtAppPrivate;
class FirebaseQtApp : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(FirebaseQtApp)
public:
    explicit FirebaseQtApp(QObject *parent = nullptr);
    ~FirebaseQtApp();

    void setAppId(const QString &id);
    void setApiKey(const QString &key);
    void setDatabaseUrl(const QString &url);
    void setMessagingSenderId(const QString &id);
    void setStorageBucket(const QString &storageBucket);
    void setProjectId(const QString &id);

    void initialize();

protected:
    void registerModule(FirebaseQtAbstractModule *module);

    friend class FirebaseQtAuth;
    friend class FirebaseQtMessaging;
    friend class FirebaseQtAbstractModule;

    FirebaseQtAppPrivate *d_ptr;
};

#endif // FIREBASEQTAPP_H
