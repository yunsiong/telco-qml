#include <telco-core.h>

#include "plugin.h"

#include "application.h"
#include "device.h"
#include "telco.h"
#include "iconprovider.h"
#include "process.h"
#include "script.h"

#include <qqml.h>

static QObject *createTelcoSingleton(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    return Telco::instance();
}

void TelcoQmlPlugin::registerTypes(const char *uri)
{
    qRegisterMetaType<QList<Application *>>("QList<Application *>");
    qRegisterMetaType<QList<Process *>>("QList<Process *>");
    qRegisterMetaType<QSet<unsigned int>>("QSet<unsigned int>");
    qRegisterMetaType<Device::Type>("Device::Type");
    qRegisterMetaType<SessionEntry::DetachReason>("SessionEntry::DetachReason");
    qRegisterMetaType<Script::Status>("Script::Status");
    qRegisterMetaType<Script::Runtime>("Script::Runtime");
    qRegisterMetaType<ScriptInstance::Status>("ScriptInstance::Status");

    qmlRegisterSingletonType<Telco>(uri, 1, 0, "Telco", createTelcoSingleton);
}

void TelcoQmlPlugin::initializeEngine(QQmlEngine *engine, const char *uri)
{
    Q_UNUSED(uri);

    // Ensure Telco is initialized.
    Telco::instance();

    engine->addImageProvider("telco", IconProvider::instance());
}
