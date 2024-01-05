#ifndef TELCOQML_VARIANT_H
#define TELCOQML_VARIANT_H

#include <telco-core.h>
#include <QVariantMap>

namespace Telco
{
    QVariantMap parseParametersDict(GHashTable *dict);
    QVariant parseVariant(GVariant *v);
};

#endif
