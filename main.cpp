#include <QtWidgets>

#include <cstdio>

#include "mainwindow.h"

static char messageTypeToChar(QtMsgType type)
{
    switch(type)
    {
        case QtDebugMsg:    return 'D';
        case QtWarningMsg:  return 'W';
        case QtCriticalMsg: return 'C';
        case QtFatalMsg:    return 'F';
#if QT_VERSION >= QT_VERSION_CHECK(5,5,0)
        case QtInfoMsg:     return 'I';
#endif

        default:
            return 'U';
    }
}

static void platesOutput(QtMsgType type, const QMessageLogContext &context, const QString &message)
{
    Q_UNUSED(context)

    QByteArray msg = message.toLatin1();

    const QString currentTime = QDateTime::currentDateTime().toString("hh:mm:ss.zzz");

    const char messageChar = messageTypeToChar(type);

    fprintf(stderr, "%s %c %s\n", qPrintable(currentTime), messageChar, static_cast<const char *>(msg));

#ifndef PLATES_NO_LOG
    static bool noLog = qgetenv("PLATES_NO_LOG") == "1";

    if(noLog)
        return;

    static QFile log(
                    QStandardPaths::writableLocation(QStandardPaths::TempLocation)
                    + QDir::separator()
                    + QCoreApplication::applicationName().toLower()
                    + ".log");

    static bool failed = false;

    if(!log.isOpen() && !failed)
    {
        failed = !log.open(QIODevice::ReadWrite | QIODevice::Append) || !log.resize(0);

        if(failed)
            fprintf(stderr, "Log file is unavailable\n");
    }

    if(!failed)
    {
        // truncate
        if(log.size() > 1*1024*1024) // 1 Mb
        {
            fprintf(stderr, "Truncating log\n");

            log.seek(0);

            char buf[1024];
            int lines = 0;
            char c = 'x';

            while(log.readLine(buf, sizeof(buf)) > 0 && lines++ < 30)
            {}

            log.resize(log.pos());

            // check if '\n' is last
            if(log.seek(log.pos()-1))
            {
                log.getChar(&c);

                if(c != '\n')
                    log.write("\n");
            }

            log.write("...\n<overwrite>\n...\n");
        }

        log.write(currentTime.toLatin1());
        log.write(" ");
        log.write(&messageChar, sizeof(decltype(messageChar)));
        log.write(" ");
        log.write(static_cast<const char *>(msg));
        log.write("\n");
    }
#endif // PLATES_NO_LOG

    if(type == QtFatalMsg)
        abort();
}

int main(int argc, char *argv[])
{
    setbuf(stderr, 0);

    QCoreApplication::setApplicationName(TARGET_HUMAN_STRING);
    QCoreApplication::setOrganizationName("OpenALPR");
    QCoreApplication::setApplicationVersion(VERSION_STRING);

    QApplication app(argc, argv);

    qInstallMessageHandler(platesOutput);

    MainWindow w;
    w.show();

    return app.exec();
}
