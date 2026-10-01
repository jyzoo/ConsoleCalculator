#include <QCoreApplication>
#include <QTextStream>
#include <QStringList>
#include <QFile>
#include <QIODevice>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

#include "calculator.h"
#include "history.h"

// Вывод справки по доступным командам
void printHelp(QTextStream &out)
{
    out << "\nДоступные команды:\n";
    out << "  add <a> <b>       - сложение\n";
    out << "  sub <a> <b>       - вычитание\n";
    out << "  mul <a> <b>       - умножение\n";
    out << "  div <a> <b>       - деление\n";
    out << "  reset             - сброс результата\n";
    out << "  history           - показать историю операций\n";
    out << "  clear_history     - очистить историю\n";
    out << "  help              - показать эту справку\n";
    out << "  quit              - выход из программы\n\n";
}

// Главная функция программы
int main(int argc, char *argv[])
{
#ifdef Q_OS_WIN
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif

    // QCoreApplication используется для консольного приложения
    QCoreApplication app(argc, argv);

    // Поток ввода из консоли
    QTextStream in(stdin);

    // Поток вывода в консоль
    QTextStream out(stdout);

    // Создаём Calculator
    Calculator calc;

    // Создаём History
    History history;

    QObject::connect(
        &calc,
        &Calculator::operationPerformed,
        &history,
        &History::addEntry
        );


    QObject::connect(
        &history,
        &History::entryAdded,
        [&out](const QString &entry)
        {
            out << "[История] Добавлена запись: "
                << entry << "\n";

            out.flush();
        }
        );


    QObject::connect(
        &calc,
        &Calculator::resultReady,
        [&out](double result)
        {
            out << "Результат: " << result << "\n";
            out.flush();
        }
        );


    QObject::connect(
        &calc,
        &Calculator::errorOccurred,
        [&out](const QString &message)
        {
            out << "Ошибка: " << message << "\n";
            out.flush();
        }
        );



    QFile historyFile("history.txt");

    if (!historyFile.open(
            QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        out << "Предупреждение: не удалось открыть history.txt\n";
    }

    QTextStream historyStream(&historyFile);

    QObject::connect(
        &calc,
        &Calculator::resultReady,
        [&historyStream](double result)
        {
            historyStream
                << "Result: "
                << result
                << "\n";

            historyStream.flush();
        }
        );

    // Приветствие
    out << "========================================\n";
    out << "     Консольный калькулятор на Qt\n";
    out << "========================================\n";

    printHelp(out);

    out << "> ";
    out.flush();

    // Основной цикл чтения команд
    QString line;

    while (in.readLineInto(&line))
    {
        // Убираем пробелы в начале и конце
        line = line.trimmed();

        // Если пользователь ввёл пустую строку
        if (line.isEmpty())
        {
            out << "> ";
            out.flush();
            continue;
        }

        // Разбиваем строку на отдельные части
        QStringList parts =
            line.split(' ', Qt::SkipEmptyParts);

        // Первая часть — команда
        QString command =
            parts.value(0).toLower();

        if (command == "quit" ||
            command == "exit")
        {
            out << "До свидания!\n";
            break;
        }


        if (command == "help")
        {
            printHelp(out);

            out << "> ";
            out.flush();

            continue;
        }



        if (command == "reset")
        {
            calc.reset();

            out << "> ";
            out.flush();

            continue;
        }


        if (command == "history")
        {
            if (history.isEmpty())
            {
                out << "История операций пуста.\n";
            }
            else
            {
                out << "\n";
                out << "--- История операций ---\n";

                QStringList entries =
                    history.entries();

                for (int i = 0;
                     i < entries.size();
                     ++i)
                {
                    out << (i + 1)
                    << ". "
                    << entries.at(i)
                    << "\n";
                }

                out << "------------------------\n";
            }

            out << "> ";
            out.flush();

            continue;
        }



        if (command == "clear_history")
        {
            history.clear();

            out << "История очищена.\n";

            out << "> ";
            out.flush();

            continue;
        }


        if (parts.size() != 3)
        {
            out << "Ошибка: неверный формат.\n";
            out << "Используйте: <команда> <a> <b>\n";

            out << "> ";
            out.flush();

            continue;
        }

        bool ok1 = false;
        bool ok2 = false;

        double a =
            parts.at(1).toDouble(&ok1);

        double b =
            parts.at(2).toDouble(&ok2);

        // Проверяем успешность преобразования
        if (!ok1 || !ok2)
        {
            calc.reportError(
                "Не удалось преобразовать операнды в числа."
                );

            out << "> ";
            out.flush();

            continue;
        }


        if (command == "add")
        {
            calc.add(a, b);
        }
        else if (command == "sub")
        {
            calc.subtract(a, b);
        }
        else if (command == "mul")
        {
            calc.multiply(a, b);
        }
        else if (command == "div")
        {
            calc.divide(a, b);
        }
        else
        {
            calc.reportError(
                "Неизвестная команда: " + command
                );
        }

        out << "> ";
        out.flush();
    }


    return 0;
}