#ifndef SETTINGS_H
#define SETTINGS_H

#include <QtCore>

#define SETTINGS_CONTAINS Settings::instance()->contains

#define SETTINGS_GET_BOOL Settings::instance()->value<bool>
#define SETTINGS_SET_BOOL Settings::instance()->setValue<bool>

#define SETTINGS_GET_INT Settings::instance()->value<int>
#define SETTINGS_SET_INT Settings::instance()->setValue<int>

#define SETTINGS_GET_STRING Settings::instance()->value<QString>
#define SETTINGS_SET_STRING Settings::instance()->setValue<QString>

#define SETTINGS_REMOVE Settings::instance()->remove

#define SETTING_LAST_DIRECTORY "last-directory"
#define SETTING_SKIP_TAGGED    "skip-tagged"
#define SETTING_MOVE_UNTAGGED  "move-untagged"
#define SETTING_TOOLTIPS       "tooltips"

class SettingsPrivate;

/*
 *  System settings
 */
class Settings
{
public:
    static Settings* instance();

    ~Settings();

    enum SyncType { NoSync, Sync };

    template <typename T>
    T value(const QString &key);

    template <typename T>
    T value(const QString &key, const T &def);

    template <typename T>
    void setValue(const QString &key, const T &value, SyncType sync = Sync);

    QVariant defaultValue(const QString &key) const;

    /*
     *  Remove the specified key from the section "settings"
     */
    void remove(const QString &key, SyncType sync = Sync);

    void sync();

    /*
     *  Available translations, hardcoded
     */
    QMap<QString, QString> translations();

private:
    Settings();

    /*
     *  Install some default values
     */
    void addDefaultValues();
    void addDefaultValues(const QHash<QString, QVariant> &defaultValues);

    void fillTranslations();

    QHash<QString, QVariant> &defaultValues();

    QSettings *settings() const;

private:
    SettingsPrivate *d;
};

/**********************************/

template <typename T>
T Settings::value(const QString &key)
{
    T def = T();
    QHash<QString, QVariant>::iterator it = defaultValues().find(key);

    if(it != defaultValues().end())
        def = it.value().value<T>();

    return value<T>(key, def);
}

template <typename T>
T Settings::value(const QString &key, const T &def)
{
    QSettings *s = settings();

    s->beginGroup("settings");
    QVariant value = s->value(key, QVariant::fromValue(def));
    s->endGroup();

    return value.value<T>();
}

template <typename T>
void Settings::setValue(const QString &key, const T &value, Settings::SyncType sync)
{
    QSettings *s = settings();

    s->beginGroup("settings");
    s->setValue(key, QVariant::fromValue(value));
    s->endGroup();

    if(sync == Sync)
        s->sync();
}

#endif // SETTINGS_H
