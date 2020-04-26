#include "firebaseqtabstractmodule.h"
#include "firebaseqtapp.h"

FirebaseQtAbstractModule::FirebaseQtAbstractModule(FirebaseQtApp *parent) : QObject(parent)
{
    parent->registerModule(this);
}
