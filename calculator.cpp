#include "calculator.h"

#include <QDebug>
#include <QtGlobal>

// Конструктор Calculator
Calculator::Calculator(QObject *parent)
    : QObject(parent)
    , m_result(0.0)
    , m_hasError(false)
    , m_errorMessage()
{
    qDebug() << "Calculator created";
}

// Получение последнего результата
double Calculator::result() const
{
    return m_result;
}

// Проверка наличия ошибки
bool Calculator::hasError() const
{
    return m_hasError;
}

// Получение сообщения последней ошибки
QString Calculator::errorMessage() const
{
    return m_errorMessage;
}

// Установка успешного результата
void Calculator::setResult(double value)
{
    m_result = value;
    m_hasError = false;
    m_errorMessage.clear();

    // Сообщаем всем подключенным обработчикам
    // о новом результате
    emit resultReady(m_result);
}

// Установка ошибки
void Calculator::setError(const QString &message)
{
    m_hasError = true;
    m_errorMessage = message;

    // Сообщаем всем подключенным обработчикам
    // о возникшей ошибке
    emit errorOccurred(m_errorMessage);
}

// Сложение
void Calculator::add(double a, double b)
{
    qDebug() << "add(" << a << "," << b << ")";

    const double result = a + b;

    setResult(result);

    // Передаём выполненную операцию в History
    emit operationPerformed(
        QString("%1 + %2 = %3")
            .arg(a)
            .arg(b)
            .arg(result)
        );
}

// Вычитание
void Calculator::subtract(double a, double b)
{
    qDebug() << "subtract(" << a << "," << b << ")";

    const double result = a - b;

    setResult(result);

    // Передаём операцию в History
    emit operationPerformed(
        QString("%1 - %2 = %3")
            .arg(a)
            .arg(b)
            .arg(result)
        );
}

// Умножение
void Calculator::multiply(double a, double b)
{
    qDebug() << "multiply(" << a << "," << b << ")";

    const double result = a * b;

    setResult(result);

    // Передаём операцию в History
    emit operationPerformed(
        QString("%1 * %2 = %3")
            .arg(a)
            .arg(b)
            .arg(result)
        );
}

// Деление
void Calculator::divide(double a, double b)
{
    qDebug() << "divide(" << a << "," << b << ")";

    // Проверяем деление на ноль
    if (qFuzzyIsNull(b)) {
        setError("Деление на ноль невозможно!");
        return;
    }

    const double result = a / b;

    setResult(result);

    // Передаём операцию в History
    emit operationPerformed(
        QString("%1 / %2 = %3")
            .arg(a)
            .arg(b)
            .arg(result)
        );
}

// Сброс калькулятора
void Calculator::reset()
{
    qDebug() << "reset()";

    m_result = 0.0;
    m_hasError = false;
    m_errorMessage.clear();

    emit resultReady(m_result);

    // Записываем сброс в историю
    emit operationPerformed("reset -> 0");
}

// Передача ошибки из main.cpp
void Calculator::reportError(const QString &message)
{
    setError(message);
}