#ifndef DANMUCOMMENT_H
#define DANMUCOMMENT_H
#include <QtCore>
#include <QtGui>
struct DanmuComment
{
    DanmuComment():time(0),originTime(0),blockBy(-1),mergedList(nullptr),m_parent(nullptr){}
    ~DanmuComment(){if(mergedList)delete mergedList;}

    enum DanmuType
    {
        Rolling,
        Top,
        Bottom,
        UNKNOW
    };
    enum FontSizeLevel
    {
        Normal,
        Small,
        Large
    };
    void setType(int mode)
    {
        switch (mode)
        {
        case 1:
        case 2:
        case 3:
            type=Rolling;
            break;
        case 4:
            type=Bottom;
            break;
        case 5:
            type=Top;
            break;
        default:
            type=Rolling;
            break;
        }
    }
    QString text;
    QString sender;
    int color;
    DanmuType type;
    FontSizeLevel fontSizeLevel;
    qint64 date;
    int time;
    int originTime;
    int blockBy;
    int source;

    QList<QSharedPointer<DanmuComment> > *mergedList;
    DanmuComment *m_parent;
    QVariantMap toMap() const {return {{"text", text}, {"time", originTime}, {"color", color}, {"fontsize", fontSizeLevel}, {"date", QString::number(date)}, {"type", type}};}
};
QDataStream &operator<<(QDataStream &stream, const DanmuComment &danmu);
QDataStream &operator>>(QDataStream &stream, DanmuComment &danmu);

Q_DECLARE_OPAQUE_POINTER(DanmuComment *)
struct SimpleDanmuInfo
{
    int time;
    int originTime;
    QString text;
};
class DanmuDrawInfo
{
public:
    int width;
    int height;
    int useCount;
    GLuint texture;
    GLfloat l,r,t,b;
    //QMutex useCountLock;
    //QImage *img=nullptr;
    //~DanmuDrawInfo(){if(img)delete img;}
};
class DanmuObject
{
private:
    static DanmuObject *head;
    static int poolCount;
    DanmuObject *next=nullptr;
public:   
    DanmuDrawInfo *drawInfo;
    QSharedPointer<DanmuComment> src;
    float x;
    float y;
    float extraData;
     ~DanmuObject();
    void *operator new(size_t sz);
    void  operator delete(void * p);
    static void DeleteObjPool();
};
/*
struct MatchInfo
{
    bool success;
    bool error;
    QString errorInfo;
    QString poolID;
    QString fileHash;
    struct DetailInfo
    {
        QString animeTitle;
        QString title;
        //int episodeId;
    };
    QList<DetailInfo> matches;
};
QDataStream &operator<<(QDataStream &stream, const MatchInfo &match);
QDataStream &operator>>(QDataStream &stream, MatchInfo &match);
QDataStream &operator<<(QDataStream &stream, const MatchInfo::DetailInfo &md);
QDataStream &operator>>(QDataStream &stream, MatchInfo::DetailInfo &md);
*/
struct BlockRule
{
    enum Field
    {
        DanmuText,
        DanmuColor,
        DanmuSender
    };
    enum Relation
    {
        Contain,
        Equal,
        NotEqual
    };
    int id;
    int blockCount;
    Field blockField;
    Relation relation;
    bool isRegExp;
    bool enable;
    bool usePreFilter;
    QString name;
    QString content;
    QScopedPointer<QRegExp> re;
    bool blockTest(DanmuComment *comment);
    BlockRule(const QString &ruleContent, Field field, Relation r);
    BlockRule() = default;
};
typedef QList<QPair<QSharedPointer<DanmuComment>,DanmuDrawInfo *> > PrepareList;
struct DanmuEvent
{
    int start;
    int duration;
    QString description;
};
struct DanmuSource
{
    // from script----
    QString title;
    QString desc;
    QString scriptData;
    QString scriptId;
    //---------
    int id = -1;
    int delay = 0;
    int count = 0;
    int duration = 0;
    bool show = true;

    QList<QPair<int,int> >timelineInfo;
    void setTimeline(const QString &timelineStr);
    QString timelineStr() const;

    QVariantMap toMap() const {return {{"title", title}, {"desc", desc}, {"data", scriptData}, {"duration", duration}, {"delay", delay}};}
    QString durationStr() const
    {
        int min=duration/60;
        int sec=duration-min*60;
        return QString("%1:%2").arg(min, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));
    }
};
QDataStream &operator<<(QDataStream &stream, const DanmuSource &src);
QDataStream &operator>>(QDataStream &stream, DanmuSource &src);
Q_DECLARE_OPAQUE_POINTER(DanmuSource *)

#endif // DANMUCOMMENT_H
