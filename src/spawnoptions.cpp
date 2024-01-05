#include <telco-core.h>

#include "spawnoptions.h"

static QVector<QString> parseStrv(gchar **strv, gint length);
static gchar **unparseStrv(QVector<QString> vector);

SpawnOptions::SpawnOptions(QObject *parent) :
    QObject(parent),
    m_handle(telco_spawn_options_new())
{
}

SpawnOptions::~SpawnOptions()
{
    g_object_unref(m_handle);
}

bool SpawnOptions::hasArgv() const
{
    return telco_spawn_options_get_argv(m_handle, nullptr) != nullptr;
}

QVector<QString> SpawnOptions::argv() const
{
    gint n;
    gchar **strv = telco_spawn_options_get_argv(m_handle, &n);
    return parseStrv(strv, n);
}

void SpawnOptions::setArgv(QVector<QString> argv)
{
    bool hadArgv = hasArgv();

    gchar **strv = unparseStrv(argv);
    telco_spawn_options_set_argv(m_handle, strv, argv.size());
    g_strfreev(strv);

    Q_EMIT argvChanged(argv);
    if (!hadArgv)
        Q_EMIT hasArgvChanged(true);
}

void SpawnOptions::unsetArgv()
{
    if (!hasArgv())
        return;
    telco_spawn_options_set_argv(m_handle, nullptr, 0);
    Q_EMIT argvChanged(QVector<QString>());
    Q_EMIT hasArgvChanged(false);
}

bool SpawnOptions::hasEnv() const
{
    return telco_spawn_options_get_env(m_handle, nullptr) != nullptr;
}

QVector<QString> SpawnOptions::env() const
{
    gint n;
    gchar **strv = telco_spawn_options_get_env(m_handle, &n);
    return parseStrv(strv, n);
}

void SpawnOptions::setEnv(QVector<QString> env)
{
    bool hadEnv = hasEnv();

    gchar **strv = unparseStrv(env);
    telco_spawn_options_set_env(m_handle, strv, env.size());
    g_strfreev(strv);

    Q_EMIT envChanged(env);
    if (!hadEnv)
        Q_EMIT hasEnvChanged(true);
}

void SpawnOptions::unsetEnv()
{
    if (!hasEnv())
        return;
    telco_spawn_options_set_env(m_handle, nullptr, 0);
    Q_EMIT envChanged(QVector<QString>());
    Q_EMIT hasEnvChanged(false);
}

bool SpawnOptions::hasCwd() const
{
    return telco_spawn_options_get_cwd(m_handle) != nullptr;
}

QString SpawnOptions::cwd() const
{
    const gchar *str = telco_spawn_options_get_cwd(m_handle);
    if (str == nullptr)
        return "";
    return QString::fromUtf8(str);
}

void SpawnOptions::setCwd(QString cwd)
{
    bool hadCwd = hasCwd();

    std::string cwdStr = cwd.toStdString();
    telco_spawn_options_set_cwd(m_handle, cwdStr.c_str());

    Q_EMIT cwdChanged(cwd);
    if (!hadCwd)
        Q_EMIT hasCwdChanged(true);
}

void SpawnOptions::unsetCwd()
{
    if (!hasCwd())
        return;
    telco_spawn_options_set_cwd(m_handle, nullptr);
    Q_EMIT cwdChanged("");
    Q_EMIT hasCwdChanged(false);
}

static QVector<QString> parseStrv(gchar **strv, gint length)
{
    QVector<QString> result(length);
    for (gint i = 0; i != length; i++)
        result[i] = QString::fromUtf8(strv[i]);
    return result;
}

static gchar **unparseStrv(QVector<QString> vector)
{
    int n = vector.size();
    gchar **strv = g_new(gchar *, n + 1);

    for (int i = 0; i != n; i++) {
        std::string str = vector[i].toStdString();
        strv[i] = g_strdup(str.c_str());
    }
    strv[n] = nullptr;

    return strv;
}
