#include "KVPair.h"

const string &KVPair::getKey() const {
    return key;
}

void KVPair::setKey(const string &key) {
    KVPair::key = key;
}

const string &KVPair::getValue() const {
    return value;
}

void KVPair::setValue(const string &value) {
    KVPair::value = value;
}

KVPair::KVPair(const string &key, const string &value) {
    setKey(key);
    setValue(value);
}

bool KVPair::operator==(const KVPair &other) const {
    return other.getKey() == this->getKey() && other.getValue() == this->getValue();
}

bool KVPair::operator<(const KVPair &other) const {
    return this->getKey() < other.getKey();
}

std::ostream &operator<<(std::ostream &stream, const KVPair &kvPair) {
    stream << "(" << kvPair.getKey() << ", " << kvPair.getValue() << ")";
    return stream;
}
