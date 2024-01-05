#include <telco-core.h>

#include "telco.h"

#include "device.h"
#include "devicelistmodel.h"
#include "maincontext.h"

Telco *Telco::s_instance = nullptr;

Telco::Telco(QObject *parent) :
    QObject(parent),
    m_localSystem(nullptr),
    m_mainContext(nullptr)
{
    telco_init();

    m_mainContext.reset(new MainContext(telco_get_main_context()));
    m_mainContext->schedule([this] () { initialize(); });

    QMutexLocker locker(&m_mutex);
    while (m_localSystem == nullptr)
        m_localSystemAvailable.wait(&m_mutex);
}

void Telco::initialize()
{
    m_handle = telco_device_manager_new();

    telco_device_manager_get_device_by_type(m_handle, TELCO_DEVICE_TYPE_LOCAL, 0, nullptr,
        onGetLocalDeviceReadyWrapper, this);

    g_signal_connect_swapped(m_handle, "added", G_CALLBACK(onDeviceAddedWrapper), this);
    g_signal_connect_swapped(m_handle, "removed", G_CALLBACK(onDeviceRemovedWrapper), this);
    telco_device_manager_enumerate_devices(m_handle, nullptr, onEnumerateDevicesReadyWrapper, this);
}

void Telco::dispose()
{
    g_signal_handlers_disconnect_by_func(m_handle, GSIZE_TO_POINTER(onDeviceRemovedWrapper), this);
    g_signal_handlers_disconnect_by_func(m_handle, GSIZE_TO_POINTER(onDeviceAddedWrapper), this);
    g_object_unref(m_handle);
    m_handle = nullptr;
}

Telco::~Telco()
{
    m_localSystem = nullptr;
    qDeleteAll(m_deviceItems);
    m_deviceItems.clear();

    telco_device_manager_close_sync(m_handle, nullptr, nullptr);
    m_mainContext->perform([this] () { dispose(); });
    m_mainContext.reset();

    s_instance = nullptr;

    telco_deinit();
}

Telco *Telco::instance()
{
    if (s_instance == nullptr)
        s_instance = new Telco();
    return s_instance;
}

void Telco::onGetLocalDeviceReadyWrapper(GObject *obj, GAsyncResult *res, gpointer data)
{
    Q_UNUSED(obj);

    static_cast<Telco *>(data)->onGetLocalDeviceReady(res);
}

void Telco::onGetLocalDeviceReady(GAsyncResult *res)
{
    GError *error = nullptr;
    TelcoDevice *deviceHandle = telco_device_manager_get_device_by_type_finish(m_handle, res, &error);
    g_assert(error == nullptr);

    auto device = new Device(deviceHandle);
    device->moveToThread(this->thread());

    {
        QMutexLocker locker(&m_mutex);
        m_localSystem = device;
        m_localSystemAvailable.wakeOne();
    }

    QMetaObject::invokeMethod(this, "add", Qt::QueuedConnection, Q_ARG(Device *, device));

    g_object_unref(deviceHandle);
}

void Telco::onEnumerateDevicesReadyWrapper(GObject *obj, GAsyncResult *res, gpointer data)
{
    Q_UNUSED(obj);

    static_cast<Telco *>(data)->onEnumerateDevicesReady(res);
}

void Telco::onEnumerateDevicesReady(GAsyncResult *res)
{
    GError *error = nullptr;
    TelcoDeviceList *devices = telco_device_manager_enumerate_devices_finish(m_handle, res, &error);
    g_assert(error == nullptr);

    gint count = telco_device_list_size(devices);
    for (gint i = 0; i != count; i++) {
        TelcoDevice *device = telco_device_list_get(devices, i);
        onDeviceAdded(device);
        g_object_unref(device);
    }

    g_object_unref(devices);
}

void Telco::onDeviceAddedWrapper(Telco *self, TelcoDevice *deviceHandle)
{
    self->onDeviceAdded(deviceHandle);
}

void Telco::onDeviceRemovedWrapper(Telco *self, TelcoDevice *deviceHandle)
{
    self->onDeviceRemoved(deviceHandle);
}

void Telco::onDeviceAdded(TelcoDevice *deviceHandle)
{
    if (deviceHandle == m_localSystem->handle())
        return;

    auto device = new Device(deviceHandle);
    device->moveToThread(this->thread());

    QMetaObject::invokeMethod(this, "add", Qt::QueuedConnection, Q_ARG(Device *, device));
}

void Telco::onDeviceRemoved(TelcoDevice *deviceHandle)
{
    QMetaObject::invokeMethod(this, "removeById", Qt::QueuedConnection, Q_ARG(QString, telco_device_get_id(deviceHandle)));
}

void Telco::add(Device *device)
{
    device->setParent(this);
    m_deviceItems.append(device);
    Q_EMIT deviceAdded(device);
}

void Telco::removeById(QString id)
{
    for (int i = 0; i != m_deviceItems.size(); i++) {
        auto device = m_deviceItems.at(i);
        if (device->id() == id) {
            m_deviceItems.removeAt(i);
            Q_EMIT deviceRemoved(device);
            delete device;
            break;
        }
    }
}
