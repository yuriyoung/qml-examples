#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "sql.h"
#include "migration.h"
#include "tablemodel.h"

int main(int argc, char *argv[])
{
#if (QT_VERSION < QT_VERSION_CHECK(6, 0, 0))
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    // create database and tables
    {
        QStringList files = { ":/migrations/001_books.sql" };
        Migration migration(Sql::database("data.db"));
        if(!migration.run(files))
        {
            qWarning() << "Create migration table(s) failed in database"
                                   << migration.connection().databaseName();
        }
    }

    qmlRegisterType<TableModel>("Macai.App", 1, 0, "SqlTableModel");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
