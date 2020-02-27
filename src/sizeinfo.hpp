#ifndef SIZEINFO_HPP
#define SIZEINFO_HPP

#include <QObject>


class SizeInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(quint64 total READ total WRITE setTotal NOTIFY totalChanged)
    Q_PROPERTY(quint64 free READ free WRITE setFree NOTIFY freeChanged)
    Q_PROPERTY(quint64 used READ used WRITE setUsed NOTIFY usedChanged)
public:
    explicit SizeInfo(QObject *parent = nullptr);
    SizeInfo(const quint64 &total, const quint64 &free, const quint64 &used, QObject *parent = nullptr);
    SizeInfo(const SizeInfo &other);

    SizeInfo &operator=(const SizeInfo &other);

    static SizeInfo calculate(const QString &devPath, QObject *parent = nullptr);

    quint64 total() const;
    quint64 free() const;
    quint64 used() const;

public slots:
    void setTotal(const quint64 &total);
    void setFree(const quint64 &free);
    void setUsed(const quint64 &used);

signals:
    void totalChanged(const quint64 &total);
    void freeChanged(const quint64 &free);
    void usedChanged(const quint64 &used);

private:
    quint64 m_total;
    quint64 m_free;
    quint64 m_used;
};

#endif // SIZEINFO_HPP
