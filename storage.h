#ifndef STORAGE_H
#define STORAGE_H

#include <QFile>
#include <QString>
#include <utility>

class Storage {
  private:
    QString filename;

  public:
    inline explicit Storage(QString filename) : filename(std::move(filename)) {}

    inline operator double() const {
        QFile file(filename);
        if (!file.open(QIODevice::ReadOnly)) {
            return 0;
        }
        return file.readAll().toDouble();
    }

    inline Storage& operator=(double value) {
        QFile file(filename);
        if (!file.open(QIODevice::WriteOnly)) {
            return *this;
        }
        file.write(QByteArray::number(value));
        return *this;
    }
};

#endif  // STORAGE_H
