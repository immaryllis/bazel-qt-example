#ifndef FLASHCARD_H
#define FLASHCARD_H

#include <QString>

class Flashcard {
   public:
    Flashcard(const QString& word, const QString& definition, bool ifLearnt = false);

    QString getWord() const;
    QString getDefinition() const;
    bool isLearnt() const;
    void setLearnt(bool learnt);
    void setWord(QString& newWord);
    void setDefinition(QString& newDefinition);

   private:
    QString word;
    QString definition;
    bool ifLearnt;
};

#endif // FLASHCARD_H
