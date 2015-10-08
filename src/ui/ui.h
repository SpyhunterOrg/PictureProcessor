#pragma once
#include <QObject>

class Test : public QObject
{
    Q_OBJECT
        Q_PROPERTY(QString author READ author /*WRITE setAuthor NOTIFY authorChanged*/)
public:
    
    QString author();
};