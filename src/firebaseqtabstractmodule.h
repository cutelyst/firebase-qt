#ifndef FIREBASEQTABSTRACTMODULE_H
#define FIREBASEQTABSTRACTMODULE_H

#include <QObject>

class FirebaseQtApp;
class FirebaseQtAbstractModule : public QObject
{
    Q_OBJECT
public:
    FirebaseQtAbstractModule(FirebaseQtApp *parent = nullptr);

protected:
    friend class FirebaseQtApp;

    virtual void initialize(FirebaseQtApp *app) = 0;
};

#endif // FIREBASEQTABSTRACTMODULE_H
