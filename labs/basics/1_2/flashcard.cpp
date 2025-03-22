#include "flashcard.h"

Flashcard::Flashcard(const QString& word, const QString& definition, bool ifLearnt)
    : word(word), definition(definition), ifLearnt(ifLearnt) {}

QString Flashcard::getWord() const {
    return word;
}

QString Flashcard::getDefinition() const {
    return definition;
}

bool Flashcard::isLearnt() const {
    return ifLearnt;
}

void Flashcard::setLearnt(bool learnt) {
    ifLearnt = learnt;
}

void Flashcard::setWord(QString& newWord) {
    word = newWord;
}

void Flashcard::setDefinition(QString& newDefinition) {
    definition = newDefinition;
}
