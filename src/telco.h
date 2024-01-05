#ifndef TELCOQML_TELCO_H
#define TELCOQML_TELCO_H

#include "telcofwd.h"

#include <QMutex>
#include <QQmlEngine>
#include <QWaitCondition>

class Device;
class MainContext;
class Scripts;

class Telco : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY_MOVE(Telco)
    Q_PROPERTY(Device *localSystem READ localSystem CONSTANT)
    QML_ELEMENT
    QML_SINGLETON

public:
    enum class Scope {
        Minimal,
        Metadata,
        Full
    };
    Q_ENUM(Scope)

    explicit Telco(QObject *parent = nullptr);
private:
    void initialize();
    void dispose();
public:
    ~Telco();

    static Telco *instance();

    Device *localSystem() const { return m_localSystem; }

    QList<Device *> deviceItems() const { return m_deviceItems; }

Q_SIGNALS:
    void localSystemChanged(Device *newLocalSystem);
    void deviceAdded(Device *device);
    void deviceRemoved(Device *device);

private:
    static void onGetLocalDeviceReadyWrapper(GObject *obj, GAsyncResult *res, gpointer data);
    void onGetLocalDeviceReady(GAsyncResult *res);
    static void onEnumerateDevicesReadyWrapper(GObject *obj, GAsyncResult *res, gpointer data);
    void onEnumerateDevicesReady(GAsyncResult *res);
    static void onDeviceAddedWrapper(Telco *self, TelcoDevice *deviceHandle);
    static void onDeviceRemovedWrapper(Telco *self, TelcoDevice *deviceHandle);
    void onDeviceAdded(TelcoDevice *deviceHandle);
    void onDeviceRemoved(TelcoDevice *deviceHandle);

private Q_SLOTS:
    void add(Device *device);
    void removeById(QString id);

private:
    QMutex m_mutex;
    TelcoDeviceManager *m_handle;
    QList<Device *> m_deviceItems;
    Device *m_localSystem;
    QWaitCondition m_localSystemAvailable;
    QScopedPointer<MainContext> m_mainContext;

    static Telco *s_instance;
};

#endif
