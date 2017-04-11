#include <QtCore>

#include "settings.h"

class SettingsPrivate
{
public:
    QSettings *settings;
    QMap<QString, QString> translations;
    QHash<QString, QVariant> defaultValues;
};

/*******************************************************/

Settings::Settings()
{
    d = new SettingsPrivate;

    addDefaultValues();

    d->settings = new QSettings(QSettings::IniFormat,
                                QSettings::UserScope,
                                QCoreApplication::organizationName(),
                                QCoreApplication::applicationName());

    qDebug("Configuration file: %s", qPrintable(d->settings->fileName()));

    d->settings->setFallbacksEnabled(false);
}

void Settings::addDefaultValues()
{
    QHash<QString, QVariant> defaultValues;

    defaultValues.insert(SETTING_SKIP_TAGGED, false);
    defaultValues.insert(SETTING_MOVE_UNTAGGED, false);
    defaultValues.insert(SETTING_TOOLTIPS, true);

    addDefaultValues(defaultValues);
}

void Settings::addDefaultValues(const QHash<QString, QVariant> &defaultValues)
{
    QHash<QString, QVariant>::const_iterator itEnd = defaultValues.end();

    for(QHash<QString, QVariant>::const_iterator it = defaultValues.begin();it != itEnd;++it)
    {
        d->defaultValues.insert(it.key(), it.value());
    }
}

void Settings::fillTranslations()
{
    d->translations.insert("en", "English");

    // http://www.loc.gov/standards/iso639-2/php/code_list.php
    // currently nothing to add
}

Settings::~Settings()
{
    delete d->settings;
    delete d;
}

QVariant Settings::defaultValue(const QString &key) const
{
    return d->defaultValues.value(key);
}

void Settings::remove(const QString &key, Settings::SyncType sync)
{
    QSettings *s = settings();

    s->beginGroup("settings");
    s->remove(key);
    s->endGroup();

    if(sync == Sync)
        s->sync();
}

void Settings::sync()
{
    d->settings->sync();
}

QMap<QString, QString> Settings::translations()
{
    if(d->translations.isEmpty())
        fillTranslations();

    return d->translations;
}

Settings* Settings::instance()
{
    static Settings *inst = new Settings;

    return inst;
}

QHash<QString, QVariant>& Settings::defaultValues()
{
    return d->defaultValues;
}

QSettings *Settings::settings() const
{
    return d->settings;
}
