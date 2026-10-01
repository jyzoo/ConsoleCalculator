#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QObject>
#include <QString>

class Calculator : public QObject
{
    Q_OBJECT

public:
    explicit Calculator(QObject *parent = nullptr);

    // Получить последний результат
    double result() const;

    // Проверить, была ли ошибка
    bool hasError() const;

    // Получить сообщение последней ошибки
    QString errorMessage() const;

public slots:
    // Арифметические операции
    void add(double a, double b);
    void subtract(double a, double b);
    void multiply(double a, double b);
    void divide(double a, double b);

    // Сброс калькулятора
    void reset();

    // Передача ошибки из main в Calculator
    void reportError(const QString &message);

signals:
    // Сигнал после успешного вычисления
    void resultReady(double result);

    // Сигнал при возникновении ошибки
    void errorOccurred(const QString &message);

    // Сигнал для передачи операции в History
    void operationPerformed(const QString &entry);

private:
    // Установить результат вычисления
    void setResult(double value);

    // Установить ошибку
    void setError(const QString &message);

    double m_result;
    bool m_hasError;
    QString m_errorMessage;
};

#endif // CALCULATOR_H