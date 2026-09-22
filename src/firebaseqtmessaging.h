#pragma once

#include "firebaseqtabstractmodule.h"

#include <QObject>

namespace firebase::messaging {
struct Message;
}

class FirebaseQtMessagingPrivate;
class FirebaseQtMessaging : public FirebaseQtAbstractModule {
  Q_OBJECT
  Q_DECLARE_PRIVATE(FirebaseQtMessaging)
public:
  explicit FirebaseQtMessaging(FirebaseQtApp *parent);
  ~FirebaseQtMessaging() override;

  /// Registers the app instance with FCM. On success, registrationReceived is
  /// emitted with the Firebase Installation ID (FID).
  /// Call this when auto-init is disabled (FirebaseMessagingAutoInitEnabled=NO
  /// / firebase_messaging_auto_init_enabled=false).
  void registerInstance();

  /// Unregisters the app instance from FCM. On success, unregistrationReceived
  /// is emitted with the FID that was unregistered.
  void unregisterInstance();

Q_SIGNALS:
  /// Emitted when the app instance is registered with FCM. The value is the
  /// Firebase Installation ID (FID); send messages with Admin SDK `fid`, not
  /// `token`.
  void registrationReceived(const QByteArray &installationId);

  /// Emitted after a successful unregisterInstance() call.
  void unregistrationReceived(const QByteArray &installationId);

  /// \deprecated Use registrationReceived() instead.
  QT_DEPRECATED_X("Use registrationReceived() — FCM registration tokens are "
                  "deprecated in favor of FID")
  void tokenReceived(const QByteArray &token);

  void messageReceived(const firebase::messaging::Message &message);

  void registerError(int errorCode, const QString &errorMessage);
  void unregisterError(int errorCode, const QString &errorMessage);

protected:
  void initialize(FirebaseQtApp *app) override;

private:
  FirebaseQtMessagingPrivate *d_ptr;
};
