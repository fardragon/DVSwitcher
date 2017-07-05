#ifndef FOCUSHOOK_H
#define FOCUSHOOK_H

#include <QObject>

class FocusHook : public QObject
{
	Q_OBJECT
public:
	explicit FocusHook(QObject *parent = nullptr);

signals:

public slots:
};

#endif // FOCUSHOOK_H