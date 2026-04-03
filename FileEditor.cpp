// TextEditor.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <windows.h>
#include <fstream>
#include <vector>
#include <clocale>
#include <string>
#include <cstdio>

using namespace std;


class FileExtensionError : public exception
{
    private:
        string message;
    public:
        FileExtensionError(const char* msg) : message(msg){}
        const char* what() const noexcept {return message.c_str();}
};

class FileNotFoundError : public exception
{
private:
    string message;
public:
    FileNotFoundError(const char* msg) : message(msg) {}
    const char* what() const noexcept { return message.c_str(); }
};


struct FileHeader {
    int autorLength;
    int textLength;
};

string getFileExtension(const string& filename) {
    size_t dotPos = filename.find_last_of('.');
    if (dotPos != string::npos && dotPos != filename.length() - 1) {
        return filename.substr(dotPos + 1);
    }
    return ""; // расширение не найдено
}

void fileWrite(string filename, vector<string> file, bool overwrite = false) {
    if (getFileExtension(filename) != "txt") {
        throw FileExtensionError("Неподдерживаемое расширение файла.");
    }
    ofstream fout(filename, overwrite ? ios::out | ios::trunc : ios::out);
    if (fout.is_open()) {
        // Записываем новое содержимое в файл
        for (const auto& line : file) {
            fout << line << endl;
        }
        fout.close();
    }
    else {
        throw "FileOpenError: Файл не найден";
    }
}

vector<string> fileRead(string filename) {
    if (getFileExtension(filename) != "txt") {
        throw FileExtensionError("Неподдерживаемое расширение файла.");
    }
    vector<string> file;
    string line;
    ifstream fin(filename);

    if (fin.is_open()) {
        while (getline(fin, line)) {
            file.push_back(line);
        }
        fin.close();
    }
    else {
        throw FileNotFoundError("Такого файла не существует");
    }
    return file;
}

void filePrint(string filename, vector<string> file) {
    cout << "=== Содержимое файла " << filename << " " << "=======================================" << "\n";
    for (int i = 0; i < file.size(); ++i) {
        cout << i + 1 << " " << file[i] << "\n";
    }
    cout << "==================================================================================";
}

int main()
{
    SYSTEM_INFO siSysInfo;
    GetSystemInfo(&siSysInfo);
    cout << "dwPageSize: " << siSysInfo.dwPageSize << "\n";
    cout << "lpMinimumApplicationAddress: " << siSysInfo.lpMinimumApplicationAddress << "\n";
    cout << "lpMaximumApplicationAddress: " << siSysInfo.lpMaximumApplicationAddress << "\n";
    cout << "dwActiveProcessorMask: " << siSysInfo.dwActiveProcessorMask << "\n";
    cout << "dwNumberOfProcessors: " << siSysInfo.dwNumberOfProcessors << "\n";
    cout << "dwProcessorType: " << siSysInfo.dwProcessorType << "\n";
    cout << "dwAllocationGranularity: " << siSysInfo.dwAllocationGranularity << "\n";
    cout << "wProcessorLevel: " << siSysInfo.wProcessorLevel << "\n";
    cout << "wProcessorRevision: " << siSysInfo.wProcessorRevision << "\n";

    string programInfo =
        "\n\nПеред вами текстовый редактор 1.0\n"
        "Выберите действие:\n"
        "0). Закрыть программу\n"
        "1). Прочитать файл\n"
        "2). Заменить строку в файле по ее номеру\n"
        "3). Добавить строку в конец\n"
        "4). Сменить рабочий файл\n"
        "5). Очистить файл\n"
        "6). Заменить все вхождения\n"
        "7). Записать файл в бинарный файл\n"
        "8). Прочитать файл бинарный\n"
        "9). Создать текстовый файл\n";

    setlocale(0, "");
    int command = -1;
    string filename;
    string line;
    ifstream fin;
    ofstream fout;
    int replacedLineNumber;
    string replacedLine;
    string newLine;
    vector<string> rf;

    string replacedWord;
    string replacingWord;
    vector<string> replacedFile;
    int start, end;

    string autor = "Барышников Илья";
    string binaryFileName;
    ofstream outBinary;
    string text;
    FileHeader writeHeader;

    FileHeader openHeader;
    ifstream inBinary;
    string oba;
    string obt;

    while (command != 0) {
        cout << programInfo;
        cout << "\nВведите действие: ";
        cin >> command;
        try {
            switch (command) {
            case 1:
                // Вывод содержимого файла
                if (filename.empty()) {
                    cout << "Введите имя файла: ";
                    cin >> filename;
                }
                rf = fileRead(filename);
                filePrint(filename, rf);
                break;
            case 2:
                // Меняем строку по номеру
                if (filename.empty()) {
                    cout << "Введите имя файла: ";
                    cin >> filename;
                }
                cout << "Введите номер строки для изменения: ";
                cin >> replacedLineNumber;
                fin.open(filename);

                if (fin.is_open()) {
                    vector<string> file;
                    while (getline(fin, line)) {
                        file.push_back(line);
                    }
                    fin.close();
                    cout << "Введите новую строку, заменяющую эту:\n";
                    cout << replacedLineNumber << " " << file[replacedLineNumber - 1] << "\n";
                    cin >> replacedLine;
                    file[replacedLineNumber - 1] = replacedLine;

                    fileWrite(filename, file);
                }
                else {
                    cout << "Ошибка открытия файла!\n";
                }
                break;
            case 3:
                // Вводим новую строку в файл
                if (filename.empty()) {
                    cout << "Введите имя файла: ";
                    cin >> filename;
                }
                rf = fileRead(filename);
                cout << "Введите новую строку: ";
                cin >> newLine;

                rf.push_back(newLine);
                fileWrite(filename, rf);
                break;
            case 4:
                // Меняем рабочий файл
                cout << "Введите имя файла: ";
                cin >> filename;
                fin.open(filename);
                if (!fin.is_open()) {
                    throw FileNotFoundError("Такого файла не существует");
                }
                fin.close();
                break;
            case 5:
                // Очищаем файл
                if (filename.empty()) {
                    cout << "Введите имя файла: ";
                    cin >> filename;
                }
                fin.open(filename);
                fin.clear();
                fin.close();
                break;
            case 6:
                // Заменяем все вхождения слов
                if (filename.empty()) {
                    cout << "Введите имя файла: ";
                    cin >> filename;
                }
                rf = fileRead(filename);
                cout << "Введите слово, которое хотите заменить: ";
                cin >> replacedWord;
                cout << "Введите заменяющее слово: ";
                cin >> replacingWord;
                for (int i = 0; i < rf.size(); ++i) {
                    while (rf[i].find(replacedWord) != -1) {
                        start = rf[i].find(replacedWord);
                        end = replacedWord.size();
                        rf[i].replace(start, end, replacingWord);
                    }
                }
                fileWrite(filename, rf);
                break;
            case 7:
                cout << "Введите название бинарного файла: ";
                cin >> binaryFileName;
                outBinary.open(binaryFileName, ios::binary);
                text = "";
                // собираем текст
                for (const auto& line : rf) {
                    text += line + '\n';
                }

                // заполняем структуру
                
                writeHeader.autorLength = autor.size();
                writeHeader.textLength = text.size();
                   
                // 1. пишем структуру
                outBinary.write((char*)&writeHeader, sizeof(writeHeader));

                // 2. пишем данные
                outBinary.write((autor + "\n").c_str(), writeHeader.autorLength);
                outBinary.write(text.c_str(), writeHeader.textLength);

                outBinary.close();
                break;
            case 8:
                cout << "Введите название бинарного файла: ";
                cin >> filename;

                if (getFileExtension(filename) != "bin") {
                    throw FileExtensionError("Файл должен быть с расширением .bin");
                }

                inBinary.open(filename, ios::binary);

                if (!inBinary.is_open()) {
                    throw FileNotFoundError("Не удалось открыть бинарный файл");
                }

                FileHeader header;

                // читаем header
                inBinary.read((char*)&header, sizeof(header));

                // читаем автора
                oba = string(header.autorLength, '\0');
                inBinary.read(&oba[0], header.autorLength);

                // читаем текст
                obt = string(header.textLength, '\0');
                inBinary.read(&obt[0], header.textLength);

                inBinary.close();

                cout << "Автор: " << oba << endl;
                cout << "Текст:\n" << obt << endl;

                break;
            case 9:
                cout << "Введите название создаваемого файла: ";
                cin >> filename;
                rf.clear();
                fileWrite(filename, rf, true);
                break;
            default:
                cout << "Выберите корректное действие";
            }
            
               
        }
        catch (FileNotFoundError& e) {
            cout << e.what() << "\n";
        }
        catch (FileExtensionError& e) {
            cout << e.what() << "\n";
        }
    }
};
