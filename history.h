#ifndef HISTORY_H
#define HISTORY_H

#include <QObject>
#include <QString>
#include <QStringList>

class History : public QObject
{
    Q_OBJECT

public:
    explicit History(QObject *parent = nullptr);

    // Получить все записи истории
    QStringList entries() const;

    // Проверить, пуста ли история
    bool isEmpty() const;

    // Очистить историю
    void clear();

public slots:
    // Добавить новую запись в историю
    void addEntry(const QString &entry);

signals:
    // Сигнал отправляется после добавления записи
    void entryAdded(const QString &entry);

private:
    // Список выполненных операций
    QStringList m_entries;
};

#endif // HISTORY_H