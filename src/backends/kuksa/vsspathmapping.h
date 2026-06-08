#ifndef VSSPATHMAPPING_H
#define VSSPATHMAPPING_H

#include <QHash>
#include <QString>
#include <QStringList>

struct VssEntry {
    QString iid;
    QString property;
    QString zone;
};

class VssPathMapping
{
public:
    // (iid, property, zone) -> VSS path
    static QString vssPath(const QString &iid, const QString &property,
                           const QString &zone = QString());

    // VSS path -> list of (iid, property, zone) entries
    static QList<VssEntry> entries(const QString &vssPath);

    // All VSS paths for a given IID
    static QStringList vssPaths(const QString &iid);

    // All known VSS paths
    static QStringList allVssPaths();

    // Available zones for a zoned interface
    static QStringList zones(const QString &iid);

    // Zone-to-VSS-instance name mapping for zoned interfaces
    static QString zoneToVssInstance(const QString &iid, const QString &zone);

private:
    struct Tables {
        // key: "iid\0property\0zone" -> VSS path
        QHash<QString, QString> forward;
        // VSS path -> list of entries
        QHash<QString, QList<VssEntry>> reverse;
        // iid -> list of VSS paths
        QHash<QString, QStringList> iidPaths;
        // iid -> zone list
        QHash<QString, QStringList> iidZones;
        // "iid\0zone" -> VSS instance name
        QHash<QString, QString> zoneInstance;

        Tables();

        void reg(const QString &iid, const QString &prop, const QString &vss);
        void regZoned(const QString &iid, const QString &prop,
                      const QString &vssTemplate, const QStringList &zones);
        void regZones(const QString &iid, const QStringList &zones,
                      const QHash<QString, QString> &instanceMap);
    };

    static const Tables &tables();
    static QString makeKey(const QString &iid, const QString &property,
                           const QString &zone = QString());
};

#endif // VSSPATHMAPPING_H
