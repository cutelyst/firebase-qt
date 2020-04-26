#ifndef FIREBASEQTAUTH_P_H
#define FIREBASEQTAUTH_P_H

#include "firebase/auth.h"

class FirebaseQtAuthStateListener;
class FirebaseQtAuthPrivate
{
public:
    FirebaseQtAuthStateListener *stateListener = nullptr;
    firebase::auth::Auth *auth = nullptr;
};

#endif // FIREBASEQTAUTH_P_H
