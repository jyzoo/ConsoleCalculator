#include "history.h"

#include <QDebug>

// Конструктор History
History::History(QObject *parent)
    : QObject(parent)
{
    qDebug() << "History created";
}

// Добавление новой записи в историю
void History::addEntry(const QString &entry)
{
    // Сохраняем операцию в список
    m_entries.append(entry);

    qDebug() << "History entry added:" << entry;

    // Сообщаем всем подключенным объектам,
    // что новая запись была добавлена
    emit entryAdded(entry);
}

// Получение всех записей истории
QStringList History::entries() const
{
    return m_entries;
}

// Проверка, пуста ли история
bool History::isEmpty() const
{
    return m_entries.isEmpty();
}

// Очистка истории
void History::clear()
{
    m_entries.clear();

    qDebug() << "History cleared";
}