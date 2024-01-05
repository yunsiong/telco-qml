#include <telco-core.h>

#include "process.h"
#include "variant.h"

Process::Process(TelcoProcess *handle, QObject *parent) :
    QObject(parent),
    m_pid(telco_process_get_pid(handle)),
    m_name(telco_process_get_name(handle)),
    m_parameters(Telco::parseParametersDict(telco_process_get_parameters(handle)))
{
    auto iconProvider = IconProvider::instance();
    for (QVariant serializedIcon : m_parameters["icons"].toList())
        m_icons.append(iconProvider->add(serializedIcon.toMap()));
}

Process::~Process()
{
    auto iconProvider = IconProvider::instance();
    for (Icon icon : m_icons)
        iconProvider->remove(icon);
}

QVector<QUrl> Process::icons() const
{
    QVector<QUrl> urls;
    for (Icon icon : m_icons)
        urls.append(icon.url());
    return urls;
}
