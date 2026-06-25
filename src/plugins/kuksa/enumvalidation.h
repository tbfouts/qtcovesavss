#ifndef ENUMVALIDATION_H
#define ENUMVALIDATION_H

#include <QVariant>

// Safely cast an integer QVariant to an enum type, clamping to [0, maxValue].
// Returns the default (0) if the value is out of range or not convertible.
template<typename EnumT>
EnumT validatedEnum(const QVariant &value, int maxValue)
{
    bool ok = false;
    const int v = value.toInt(&ok);
    if (!ok || v < 0 || v > maxValue)
        return static_cast<EnumT>(0);
    return static_cast<EnumT>(v);
}

#endif // ENUMVALIDATION_H
